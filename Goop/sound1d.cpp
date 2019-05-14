#ifndef DEDSERV

#include "sound1d.h"

#include "sfx.h"
#include "resource_list.h"
#include "util/math_func.h"

#include <string>
#include <iostream>
#include <boost/filesystem/path.hpp>
namespace fs = boost::filesystem;

#include <allegro.h>
//#include "text.h"

//include <fmod.h>
#include "fake_fmod.h"

using namespace std;

ResourceList<Sound1D> sound1DList;

Sound1D::Sound1D()
{
	m_sound = NULL;
}

Sound1D::~Sound1D()
{
	if ( m_sound )  destroy_sample( m_sound );
}

bool Sound1D::load(fs::path const& filename)
{	
	// TEMPORARY MEASURE UNTIL WE GET AN ALLEGRO 4 OGG LOADER --GM
	std::string raw_filename = filename.native();
	std::size_t ogg_offs = raw_filename.find( ".ogg" );
	if ( ogg_offs != std::string::npos )
	{
		raw_filename = raw_filename.replace( ogg_offs, 4, ".wav" );
	}

	cerr << "Loading 1D sound: " << raw_filename << endl;
	m_sound = load_sample( raw_filename.c_str() );
	if ( m_sound )
	{
		return true;
	}
	return false;
}

void Sound1D::play(float volume,float pitch, float volumeVariation, float pitchVariation)
{
	if( m_sound ) 
	{
		//int chan = FSOUND_PlaySoundEx(FSOUND_FREE, m_sound, 0, 1);
		float rndPitch = pitch + midrnd()*pitchVariation;
		float rndVolume = volume + midrnd()*volumeVariation;
		int chan = play_sample(m_sound, 255*volume, 128, 1000*pitch, 0);
		if ( chan >= 0 )
		{
			//FSOUND_SetFrequency(chan, static_cast<int>(FSOUND_GetFrequency(chan) * rndPitch) );
			//FSOUND_SetVolume(chan, static_cast<int>(FSOUND_GetVolume(chan)*rndVolume) );
			//FSOUND_SetLoopMode( chan, FSOUND_LOOP_OFF );
			//FSOUND_SetPaused(chan, 0);
		}
	}
}

#endif
