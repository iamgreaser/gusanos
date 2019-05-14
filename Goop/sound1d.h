#ifndef SOUND1D_H
#define SOUND1D_H

#ifdef DEDSERV
#error "Can't use this in dedicated server"
#endif //DEDSERV

#include "resource_list.h"
#include "glua.h"
#include <string>
#include <boost/filesystem/path.hpp>
#include <allegro.h>
namespace fs = boost::filesystem;

class Sound1D : public LuaObject
{
	public:
		
	Sound1D();
	~Sound1D();
	
	bool load(fs::path const& filename);
	void play( float volume = 1,float pitch = 1,float volumeVariation = 0,float pitchVariation = 0);
	
	private:
	
	SAMPLE * m_sound;
	
};

extern ResourceList<Sound1D> sound1DList;

#endif // _SOUND1D_H_
