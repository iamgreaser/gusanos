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

	sendClientPacket("HONK HONK", 9);
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

void ZComUnofficial_Connection::sendClientPacket(const char *data, size_t len)
{
	ssize_t result = sendto(m_sockfd, data, len, 0, (sockaddr *)m_client_sockaddr, m_client_sockaddr_len);
	assert(result >= 0);
}

size_t ZComUnofficial_Connection::recvClientPacket(char *data, size_t *len)
{
	// TODO!
	return 0;
}

