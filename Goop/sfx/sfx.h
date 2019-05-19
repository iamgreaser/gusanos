#ifndef SFX_H
#define SFX_H

#ifdef DEDSERV
#error "Can't use this in dedicated server"
#endif //DEDSERV

#include "util/vec.h"


class BaseObject;

struct Listener
{
	Vec pos;
	Vec spd;
};

class Sfx
{
public:
		
	Sfx();
	~Sfx();
	
	void init();
	void shutDown();
	void registerInConsole();
	void calculateVolumes( float loudness, float x, float y, int *outLoudness, int *outPanning );
	void think();
	void setChanObject( int chan, BaseObject* obj );
	void setChanPos( int chan, float x, float y );
	void setChanLoudness( int chan, float loudness );
	void clearChanPos( int chan );
	void clear();
	Listener* newListener();
	void freeListener(Listener* listener);
	void volumeChange();
	
	operator bool(); // Returns true if it's safe to use this object

protected:
	static SAMPLE* loadOggFile(const char *filename);
	void registerSampleFormats();

};

extern Sfx sfx;

#endif // _GFX_H_
