#include "util/udp.h"
#include "util/sockets.h"

namespace UDP
{

Socket::~Socket()
{
	close();
}

void Socket::close()
{
	if(s)
	{
#ifdef WINDOWS
		closesocket(s);
#else
		::close(s);
#endif
		s = 0;
	}
}


bool Socket::think()
{
	return error != ErrorNone;
}

bool Socket::readPacket()
{
	char staticBuffer[2048];
	const int bufferSize = sizeof(staticBuffer);
	sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);

	ssize_t r = recvfrom(s, staticBuffer, bufferSize, 0, (sockaddr*)&addr, &addrlen);
	if(r != -1)
	{
		if(r == 0)
		{
			// ...what?
		}

		// TODO: handle received packet
		resetTimer();
	}
	else if(sockError() != EWOULDBLOCK)
	{
		error = ErrorRecv;
		return true; // Error
	}
	else
	{
		checkTimeout();
	}
	
	return error != ErrorNone;
}

// TODO: send stuff

}

