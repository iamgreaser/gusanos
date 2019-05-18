#include <cstring>

#include "util/sockets.h"

namespace Sockets
{

char* copyCString(char* p)
{
	int l = strlen(p);
	char* n = new char[l + 1];
	memcpy(n, p, l + 1);
	return n;
}

char* copyString(char* p, size_t l)
{
	char* n = new char[l + 1];
	memcpy(n, p, l + 1);
	return n;
}

char** copyList(char** p)
{
	int l = 0;
	for(; p[l]; ++l)
		/* nothing */;
	
	char** n = new char*[l + 1];
	for(int i = 0; i < l; ++i)
	{
		n[i] = copyCString(p[i]);
	}
	
	n[l] = 0;
	
	return n;
}

char** copyListL(char** p, size_t len)
{
	int l = 0;
	for(; p[l]; ++l)
		/* nothing */;
	
	char** n = new char*[l + 1];
	for(int i = 0; i < l; ++i)
	{
		n[i] = copyString(p[i], len);
	}
	
	n[l] = 0;
	
	return n;
}

void deleteList(char** p)
{
	for(int i = 0; p[i]; ++i)
		delete[] p[i];
	delete[] p;
}

Addrinfo::Addrinfo(addrinfo const* p)
{
	ai_flags = p->ai_flags;
	ai_family = p->ai_family;
	ai_socktype = p->ai_socktype;
	ai_protocol = p->ai_protocol;
	ai_addrlen = p->ai_addrlen;
	ai_addr = (struct sockaddr *)malloc(p->ai_addrlen);
	memcpy((void *)ai_addr, (void *)p->ai_addr, (size_t)p->ai_addrlen);
	if( p->ai_canonname )
	{
		size_t canonlen = strlen(p->ai_canonname)+1;
		ai_canonname = (char *)malloc(canonlen);
		memset((void *)ai_canonname, 0, canonlen);
		strncpy((char *)ai_canonname, (char *)p->ai_canonname, canonlen-1);
	}
	else
	{
		ai_canonname = NULL;
	}
	ai_next = NULL; // Don't link
}

Addrinfo::~Addrinfo()
{
	if ( ai_addr )
	{
		free(ai_addr);
		ai_addr = NULL;
	}

	if ( ai_canonname )
	{
		free(ai_canonname);
		ai_canonname = NULL;
	}
}

Addrinfo* resolveHost(std::string const& name, int const& port)
{
	addrinfo* p = NULL;
	int gaiError = getaddrinfo( name.c_str(), std::to_string(port).c_str(), NULL, &p);
	
	if ( gaiError != 0 )
	{
		return 0; // ERROR
	}
	
	Addrinfo* result = new Addrinfo(p);
	freeaddrinfo(p);

	return result;
}

int socketNonBlock()
{
	int s;
	
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		return -1;

#ifdef WINDOWS
	unsigned long nonBlocking = 1;
	ioctlsocket(s, FIONBIO, &nonBlocking);
#else
	fcntl(s, F_SETFL, O_NONBLOCK);
#endif

	return s;
}

bool connect(int s, Addrinfo* ai)
{
	int r = connect(s, reinterpret_cast<sockaddr *>(ai->ai_addr), ai->ai_addrlen);

	if(r == -1 &&
#ifdef WINDOWS
			sockError() != EWOULDBLOCK)
#else
			sockError() != EINPROGRESS)
#endif
		return false; // ERROR

	return true;
}

}
