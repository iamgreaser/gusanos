#include "bindings-network.h"

#include "luaapi/types.h"
#include "luaapi/macros.h"
#include "luaapi/classes.h"

#include "game.h"
#include "player/base_player.h"
#include "base_worm.h"
#include "particle.h"
#include "util/sockets.h"
#include "util/log.h"
#include "util/tcp.h"

#include <iostream>
#include <memory>
#include <utility>
#include <list>
#include <cstring>
using std::cerr;
using std::endl;
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
using boost::lexical_cast;

namespace LuaBindings
{
	
LuaReference SocketMetaTable;
LuaReference ZCom_BitStreamMetaTable;
LuaReference LuaGameEventMetaTable, LuaPlayerEventMetaTable, LuaWormEventMetaTable, LuaParticleEventMetaTable;


class LuaSocket : public TCP::Socket
{
public:
	LuaSocket( /*LuaReference ref, int s_, LuaContext context_, LuaReference recvCallback_*/)
	: TCP::Socket(10*60) //, context(context_), recvCallback(recvCallback_)
	, dataSender(0) //, luaReference(ref)
	{
	}
	
	void send(char const* p, size_t len)
	{
		char* m = new char[len];
		memcpy(m, p, len);
		sendQueue.push_back(std::make_pair(m, m+len));
	}
	
	void think()
	{
		if(error)
		{
			cerr << "Error!" << endl;
			error = ErrorNone;
		}
		
		TCP::Socket::think();
		
		if(connected)
		{
			if(dataSender) // We're still sending data
			{
				if(dataSender->resume())
				{
					// Send done or errored
					delete dataSender; dataSender = 0;
					delete[] sendQueue.front().first;
					sendQueue.pop_front();
				}
			}
			else if(!sendQueue.empty())
			{
				std::pair<char*, char*>& t = sendQueue.front();
				
				if(!(dataSender = TCP::Socket::send(t.first, t.second)))
				{
					delete[] t.first;
					sendQueue.pop_front();
				}
			}
			
			if(readChunk())
				return;
		}
	}
	
	~LuaSocket()
	{
		delete dataSender;
		foreach(i, sendQueue)
		{
			delete[] i->first;
		}
	}
	
	char const* begin()
	{
		return dataBegin;
	}
	
	char const* end()
	{
		return dataEnd;
	}
	
private:
	TCP::Socket::ResumeSend* dataSender;
	std::list<std::pair<char*, char*> > sendQueue;
	//LuaContext context;
	//LuaReference recvCallback;
	LuaReference luaReference;
};

/*! tcp_connect(addr, port)
	
	Returns a TCPSocket object connecting to //addr// on port //port//.
*/
int l_tcp_connect(lua_State* L)
{
	LuaContext context(L);
	
	char const* addr = lua_tostring(context, 1);
	if(!addr) return 0;
	int port = lua_tointeger(context, 2);
	
	//lua_pushvalue(context, 3);
	//LuaReference recvCallback = context.createReference();
	
	std::auto_ptr<Sockets::Addrinfo> hp(Sockets::resolveHost( addr, port ));
	
	if(!hp.get())
		return 0;

	LuaSocket* socket = (LuaSocket*)lua_newuserdata(context, sizeof(LuaSocket));
	//lua_pushvalue(context, -1);
	//LuaSocket* sock = new (space) LuaSocket(context.createReference(), s, context, recvCallback);
	new (socket) LuaSocket();

	socket->socketNonBlock();
	if(!socket->connect(hp.get()))
	{
		lua_pop(context, 1);
		return 0;
	}

	context.push(SocketMetaTable);
	lua_setmetatable(context, -2);

	return 1;
}

/*! TCPSocket:send(data)
	
	Sends //data// on the socket.
*/
LMETHOD(LuaSocket, tcp_send,
	size_t len;
	char const* s = lua_tolstring(context, 2, &len);
	if(!s) return 0;
	p->send(s, len);
	return 0;
)

/*! TCPSocket:think()
	
	Processes data sending and recieving.
	
	If there's data recieved, it's returned as a string.
	
	If there's no data recieved, nil is returned. 
	
	Must be called regularly for data sends and connection to finish.
*/
LMETHOD(LuaSocket, tcp_think,
	p->think();
	if(p->begin() == p->end())
		return 0;
	DLOG((void *)p->begin() << ", " << (void *)p->end() << ", size: " << (p->end() - p->begin()));
	lua_pushlstring(context, p->begin(), p->end() - p->begin());
	return 1;
)

LMETHOD(LuaSocket, tcp_destroy,
	p->~LuaSocket();
	return 0;
)

//! version any

void initNetwork(LuaContext& context)
{
	context.functions()
		("tcp_connect", l_tcp_connect)
	;
	
	CLASSM(Socket,
		("__gc", l_tcp_destroy)
	,
		("send", l_tcp_send)
		("think", l_tcp_think)
	)

	lua_pushboolean(context, true);
	lua_setfield(context, LUA_GLOBALSINDEX, "AUTH");
}

}
