#ifndef DEDSERV

#include "sound.h"

#include "sfx.h"
#include "resource_list.h"
#include "util/math_func.h"
#include "base_object.h"

#include <string>
#include <iostream>
#include <boost/filesystem/path.hpp>
namespace fs = boost::filesystem;

#include <allegro.h>
//#include "text.h"

using namespace std;

ResourceList<Sound> soundList;

Sound::Sound()
{
	m_sound = NULL;
}

Sound::~Sound()
{
	if ( m_sound )  destroy_sample( m_sound );
}

bool Sound::load(fs::path const& filename)
{
	std::string raw_filename = filename.native();
	//cerr << "Loading sound: " << raw_filename << endl;
	m_sound = load_sample( raw_filename.c_str() );
	if ( m_sound )
	{
		return true;
	}
	return false;
}

void Sound::play(float volume,float pitch, float volumeVariation, float pitchVariation)
{
	if( m_sound ) 
	{
		//int chan = FSOUND_PlaySoundEx(FSOUND_FREE, m_sound, 0, 1);
		float rndPitch = pitch + rnd()*pitchVariation - pitchVariation / 2;
		float rndVolume = pitch + rnd()*volumeVariation - volumeVariation / 2; // FIXME: yeah this is probably a typo
		int chan = play_sample(m_sound, 255*rndVolume, 128, 1000*rndPitch, 0);
		if ( chan >= 0 )
		{
			sfx.clearChanPos( chan );
			//FSOUND_SetFrequency(chan, static_cast<int>(FSOUND_GetFrequency(chan) * rndPitch) );
			//FSOUND_SetVolume(chan, static_cast<int>(FSOUND_GetVolume(chan)*rndVolume) );
			//FSOUND_SetLoopMode( chan, FSOUND_LOOP_OFF );
			//FSOUND_SetPaused(chan, 0);
		}
	}
}

void Sound::play2D(const Vec& pos, float loudness, float pitch, float pitchVariation)
{
	if( m_sound ) 
	{
		//int chan = FSOUND_PlaySoundEx(FSOUND_FREE, m_sound, NULL, 1);
		//float _pos[3] = { pos.x, pos.y, 0 };
		float rndPitch = pitch + rnd()*pitchVariation - pitchVariation / 2;
		int chanLoudness = 255*loudness/100.0f;
		int chanPanning = 128;
		sfx.calculateVolumes(loudness, pos.x, pos.y, &chanLoudness, &chanPanning);
		int chan = play_sample(m_sound, chanLoudness, chanPanning, 1000*rndPitch, 0);
		if ( chan >= 0 )
		{
			sfx.setChanPos( chan, pos.x, pos.y );
			sfx.setChanLoudness( chan, loudness );
			//FSOUND_3D_SetAttributes(chan, _pos, NULL);
			//FSOUND_SetFrequency(chan, static_cast<int>(FSOUND_GetFrequency(chan) * rndPitch) );
			//FSOUND_3D_SetMinMaxDistance(chan, loudness, 10000.0f);
			//FSOUND_SetLoopMode( chan, FSOUND_LOOP_OFF );
			//FSOUND_SetPaused(chan, 0);
		}
	}
}

void Sound::play2D(BaseObject* obj, float loudness, float pitch, float pitchVariation)
{
	if( m_sound ) 
	{
		//int chan = FSOUND_PlaySoundEx(FSOUND_FREE, m_sound, NULL, 1);
		//float pos[3] = { obj->pos.x, obj->pos.y, 0 };
		float rndPitch = pitch + rnd()*pitchVariation - pitchVariation / 2;
		int chanLoudness = 255*loudness/100.0f;
		int chanPanning = 128;
		sfx.calculateVolumes(loudness, obj->pos.x, obj->pos.y, &chanLoudness, &chanPanning);
		int chan = play_sample(m_sound, chanLoudness, chanPanning, 1000*rndPitch, 0);
		if ( chan >= 0 )
		{
			//FSOUND_3D_SetAttributes(chan, pos, NULL);
			sfx.setChanObject( chan, obj );
			sfx.setChanLoudness( chan, loudness );
			//FSOUND_SetFrequency(chan, static_cast<int>(FSOUND_GetFrequency(chan) * rndPitch) );
			//FSOUND_3D_SetMinMaxDistance(chan, loudness, 10000.0f);
			//FSOUND_SetLoopMode( chan, FSOUND_LOOP_OFF );
			//FSOUND_SetPaused(chan, 0);
		}
	}
}

#endif
