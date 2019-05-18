#include "zoidcom.h"

#include <cassert>
#include <cstdlib>
#include <cstring>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

using std::string;
using std::to_string;

// Server
ZComUnofficial_Connection::ZComUnofficial_Connection(ZCom_Control* control, int udpPort)
	: m_control(control)
	, m_state(ConnHosting)
	, m_joinBitStream(NULL)
	, m_client_sockaddr(NULL)
	, m_client_sockaddr_len(0)
{
	ZoidCom_debugMessage(string("Starting host on UDP port " + to_string(udpPort)).c_str());

	// Make a base host
	struct addrinfo ai_hints;
	struct addrinfo* ai_result = NULL;
	memset(&ai_hints, 0, sizeof(ai_hints));

	ai_hints.ai_family = AF_UNSPEC;
	ai_hints.ai_socktype = SOCK_STREAM;
	ai_hints.ai_protocol = 0;

	int did_getaddrinfo = getaddrinfo(
		"0.0.0.0", // IPv4
		//"::", // IPv6
		to_string(udpPort).c_str(),
		&ai_hints,
		&ai_result);

	if ( did_getaddrinfo != 0 )
	{
		ZoidCom_debugMessage((string("Failed to resolve a name for server binding: ") + string(gai_strerror(did_getaddrinfo))).c_str());

		m_state = ConnDyingNoAddress;
		return;
	}

	// Create the socket
	ZoidCom_debugMessage((string("Resolved address for ") + string(ai_result->ai_family == AF_INET ? "IPv4" : ai_result->ai_family == AF_INET6 ? "IPv6" : "???")).c_str());
	m_sockfd = socket(ai_result->ai_family, SOCK_DGRAM, 0);
	assert(m_sockfd >= 1);
	int reuseaddr = 1;
	int did_set_reuseaddr = setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int));
	assert(did_set_reuseaddr == 0);

	// Bind it
	int did_bind = bind(m_sockfd, ai_result->ai_addr, ai_result->ai_addrlen);
	assert(did_bind == 0);

	// Free the remaining address info
	freeaddrinfo(ai_result);

	ZoidCom_debugMessage(string("Hosting on UDP port " + to_string(udpPort)).c_str());
}

// Client
ZComUnofficial_Connection::ZComUnofficial_Connection(ZCom_Control* control, ZCom_Address addr, ZCom_BitStream* joinBitStream)
	: m_control(control)
	, m_state(ConnPreConnecting)
	, m_joinBitStream(NULL)
	, m_client_sockaddr(NULL)
	, m_client_sockaddr_len(0)
{
	string rawIP = addr.getIP();
	string outputIP = "???";
	string outputPort = "???";

	// Sanitise and split IP
	size_t posOpen = rawIP.find("[");
	size_t posClosed = rawIP.find("]");
	size_t posColon;
	if ( posOpen != string::npos && posClosed != string::npos )
	{
		posColon = rawIP.find(":", posClosed);
	}
	else
	{
		posColon = rawIP.find(":");
	}

	if ( posColon == string::npos )
	{
		// No port, so... OK, we need some way to fail.
		assert(!"we should have been given a port in the connection string!");
	}
	else
	{
		// Split the IP and the port.
		if ( posOpen != string::npos && posClosed != string::npos )
		{
			outputIP = rawIP.substr(posOpen + 1, posClosed - (posOpen + 1));
		}
		else
		{
			outputIP = rawIP.substr(0, posColon);

		}

		outputPort = rawIP.substr(posColon+1);
	}

	ZoidCom_debugMessage(string("Establishing connection to " + outputIP + ", port " + outputPort).c_str());

	// Resolve the IP

	struct addrinfo ai_hints;
	struct addrinfo* ai_result = NULL;
	memset(&ai_hints, 0, sizeof(ai_hints));

	ai_hints.ai_family = AF_UNSPEC;
	ai_hints.ai_socktype = SOCK_STREAM;
	ai_hints.ai_protocol = 0;

	int did_getaddrinfo = getaddrinfo(
		outputIP.c_str(),
		outputPort.c_str(),
		&ai_hints,
		&ai_result);

	if ( did_getaddrinfo != 0 )
	{
		ZoidCom_debugMessage((string("Failed to resolve a name for connecting: ") + string(gai_strerror(did_getaddrinfo))).c_str());

		m_state = ConnDyingNoAddress;
		return;
	}

	// Duplicate the address info
	ZoidCom_debugMessage((string("Resolved address for ") + string(ai_result->ai_family == AF_INET ? "IPv4" : ai_result->ai_family == AF_INET6 ? "IPv6" : "???")).c_str());
	m_client_sockaddr = (void *)malloc(ai_result->ai_addrlen);
	memcpy(m_client_sockaddr, ai_result->ai_addr, ai_result->ai_addrlen);
	m_client_sockaddr_len = ai_result->ai_addrlen;

	// Create the socket
	m_sockfd = socket(ai_result->ai_family, SOCK_DGRAM, 0);
	assert(m_sockfd >= 1);

	// Free the remaining address info
	freeaddrinfo(ai_result);

	// Duplicate the join bitstream
	m_joinBitStream = joinBitStream->Duplicate();
	assert(m_joinBitStream != NULL);
	assert(m_joinBitStream != joinBitStream);

	//
	ZoidCom_debugMessage(string("Ready to connect to " + outputIP).c_str());
	m_state = ConnConnecting;

	std::vector<uint8_t> payload;

	// latest_sent_index
	payload.push_back(0x00);
	payload.push_back(0x00);
	// latest_received_index
	payload.push_back(0x00);
	payload.push_back(0x00);
	// hole_count
	payload.push_back(0x00);

	// Packet at index 0x0001
	uint16_t packet_index = 0x0001;
	size_t packet_length = m_joinBitStream->m_buffer.size();
	payload.push_back((packet_index&0xFF));
	payload.push_back(((packet_index>>8)&0xFF));
	payload.push_back(eZCom_PacketJoinRequest);
	for(size_t i = 0; i < packet_length; i++)
	{
		payload.push_back(m_joinBitStream->m_buffer[i]);
	}

	sendClientPacket((const char *)(payload.data()), payload.size());
}

ZComUnofficial_Connection::~ZComUnofficial_Connection()
{
	ZoidCom_debugMessage(string("Destroying socket").c_str());

	if ( m_joinBitStream != NULL )
	{
		delete m_joinBitStream;
		m_joinBitStream = NULL;
	}

	if ( m_sockfd != 0 )
	{
#ifdef WINDOWS
		closesocket(m_sockfd);
#else
		close(m_sockfd);
#endif
		m_sockfd = 0;
	}

	if ( m_client_sockaddr != NULL )
	{
		free(m_client_sockaddr);
		m_client_sockaddr = NULL;
	}

	ZoidCom_debugMessage(string("Destroyed socket").c_str());
}

void ZComUnofficial_Connection::sendClientPacket(const char *data, size_t len, void *other_sockaddr, size_t other_sockaddr_len)
{
	ssize_t result = sendto(m_sockfd, data, len, 0, (sockaddr *)other_sockaddr, other_sockaddr_len);
	assert(result >= 0);
}

void ZComUnofficial_Connection::sendClientPacket(const char *data, size_t len)
{
	ssize_t result = sendto(m_sockfd, data, len, 0, (sockaddr *)m_client_sockaddr, m_client_sockaddr_len);
	assert(result >= 0);
}

size_t ZComUnofficial_Connection::recvClientPacket(char *data, size_t len, void* other_sockaddr, unsigned int* other_sockaddr_len)
{
	ssize_t result = recvfrom(m_sockfd, data, len, MSG_DONTWAIT, (sockaddr *)other_sockaddr, other_sockaddr_len);
	if ( result < 0 )
	{
		int err = errno;
		if ( err == EAGAIN )
		{
			return 0;
		}
		else
		{
			assert ( !"I don't know how to deal with recvfrom errors that aren't EAGAIN!" );
		}
	}

	return (size_t)result;
}

void ZComUnofficial_Connection::think(void)
{
	while(true)
	{
		switch(m_state)
		{
			case ConnDead:
				return;

			case ConnDyingNoAddress: {
				ZCom_BitStream reply1;
				//ZCom_BitStream reply2;
				m_state = ConnDead;
				m_control->ZCom_cbConnectResult(0, eZCom_ConnNopeThatDidntWork, reply1);
				//m_control->ZCom_cbConnectionClosed(0, eZCom_ClosedDisconnect, reply2);
			} return;

			default:
				break;
		}

		uint8_t packet_buffer[2048];
		uint8_t other_sockaddr[256]; // because IPv6 is a thing and sockaddr isn't big enough --GM
		socklen_t other_sockaddr_len = sizeof(other_sockaddr);
		size_t packet_length = recvClientPacket((char *)packet_buffer, (size_t)sizeof(packet_buffer), (void *)other_sockaddr, &other_sockaddr_len);

		if ( packet_length == 0 )
		{
			break;
		}

		ZoidCom_debugMessage((string("Got packet of length ") + to_string(packet_length)).c_str());
		assert(packet_length >= 5);

		uint16_t latest_sent_index = (uint16_t)((packet_buffer[0])|(packet_buffer[1]<<8));
		uint16_t latest_received_index = (uint16_t)((packet_buffer[2])|(packet_buffer[3]<<8));
		uint8_t hole_count = packet_buffer[4];

		// TODO: handle holes
		int start_idx = 5 + (hole_count*3);
		assert(packet_length >= 5+(hole_count*3));
		if (start_idx != 5+(hole_count*3))
		{
			assert(packet_length >= 5+(hole_count*3)+3);
			uint16_t payload_index = (uint16_t)((packet_buffer[start_idx+0])|(packet_buffer[start_idx+1]<<8));
			uint8_t payload_type = packet_buffer[start_idx+2];
			size_t payload_length = packet_length - (start_idx+3);
			uint8_t* payload_data = &packet_buffer[start_idx+3];

			std::vector<uint8_t> payload;

			// latest_sent_index
			payload.push_back(0x00);
			payload.push_back(0x00);
			// latest_received_index
			payload.push_back(0x00);
			payload.push_back(0x00);
			// hole_count
			payload.push_back(0x00);

			// Packet at index 0x0000
			uint16_t packet_index = 0x0000;
			payload.push_back((packet_index&0xFF));
			payload.push_back(((packet_index>>8)&0xFF));
			payload.push_back(eZCom_PacketInformalDisconnect);

			sendClientPacket((const char *)(payload.data()), payload.size());
		}
	}
}
