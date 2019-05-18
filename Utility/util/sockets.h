#ifndef OMFG_HTTP_SOCKETS_H
#define OMFG_HTTP_SOCKETS_H

#if defined(WINDOWS)
#define NOGDI
#include <winsock2.h>
#include <cstdlib>
typedef int socklen_t;

inline int sockError() { return WSAGetLastError(); }
const int EINPROGRESS = WSAEINPROGRESS;
const int EWOULDBLOCK = WSAEWOULDBLOCK;

#else //if !defined(OSK)
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

inline int sockError() { return errno; }

#endif

#include <string>
#include <cassert>

namespace Sockets
{

struct Addrinfo : public addrinfo
{
	Addrinfo(addrinfo const* p);
	
	~Addrinfo();
};

Addrinfo* resolveHost(std::string const& name, int const& port);

int socketNonBlock();

bool connect(int s, Addrinfo* addr);

}

#endif //OMFG_HTTP_SOCKETS_H
