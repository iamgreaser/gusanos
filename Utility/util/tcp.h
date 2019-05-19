#ifndef OMFG_TCP_H
#define OMFG_TCP_H

#include <ctime>
#include "util/sockets.h"

namespace TCP
{

class Socket : public Sockets::Socket
{
public:
	enum Error
	{
		ErrorNone,
		ErrorConnect,
		ErrorSend,
		ErrorRecv,
		ErrorDisconnect,
		ErrorTimeout,
	};

	struct ResumeSend
	{
		ResumeSend(Socket* sock_, char const* b_, char const* e_)
		: sock(sock_), b(b_), e(e_), error(ErrorNone)
		{
		}

		Socket* sock;
		char const* b;
		char const* e;
		Error error;

		bool resume();
	};

	Socket(int family, int timeOut_ = 10)
	: Sockets::Socket(family, SOCK_STREAM, 0)
	, connected(false), connecting(true)
	, error(ErrorNone), timeOut(timeOut_)
	, dataBegin(0), dataEnd(0)
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

	bool readChunk();

	ResumeSend* send(char const* b, char const* e);

	ResumeSend* send(ResumeSend* r, char const* b, char const* e);

	bool trySend(char const*& b, char const* e);

	Error getError() { return error; }

	~Socket();

protected:
	static int const bufferSize = 1024;
	bool connected;
	bool connecting;
	char staticBuffer[bufferSize];
	char const* dataBegin;
	char const* dataEnd;
	Error error;
	time_t t;
	int timeOut;
};

}

#endif //OMFG_TCP_H
