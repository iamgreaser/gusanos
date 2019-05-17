#ifndef OMFG_UDP_H
#define OMFG_UDP_H

#include <ctime>
#include "util/sockets.h"

namespace UDP
{

struct Socket
{
	enum Error
	{
		ErrorNone,
		ErrorSend,
		ErrorRecv,
		ErrorTimeout,
	};

	Socket(int s_, int timeOut_ = 10)
	: s(s_), error(ErrorNone), timeOut(timeOut_)
	{
		resetTimer();
	}

	void checkTimeout()
	{
		if(time(0) > t + timeOut)
			error = ErrorTimeout;
	}

	void resetTimer()
	{
		t = time(0);
	}

	bool think();

	bool readPacket();

	Error getError() { return error; }

	~Socket();

	void close();

protected:
	int s;
	Error error;
	time_t t;
	int timeOut;
};

}

#endif //OMFG_TCP_H
