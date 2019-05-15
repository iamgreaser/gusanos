#ifndef DEDSERV

#include <allegro.h>

#include "sfx.h"
#include "gconsole.h"
#include "base_object.h"
#include "util/macros.h"
#include <boost/assign/list_inserter.hpp>
using namespace boost::assign;

#include <vector>
#include <list>
#include <boost/utility.hpp>

#include <cstring>
#include <cstdint>
#include <cstdio>
#include <cassert>
#include <cmath>
#include <vorbis/vorbisfile.h>

using namespace std;

Sfx sfx;

namespace
{
	bool m_initialized = false;
	
	std::list< std::pair< int, BaseObject* > > chanObject;
	std::map< int, Vec > chanPosMap;
	std::map< int, float > chanLoudness;
	std::vector<Listener*> listeners;
	
	int m_volume;
	int m_listenerDistance;
	int m_outputMode = -1;
}

void volume( int oldValue )
{
	if (sfx) sfx.volumeChange();
}

Sfx::Sfx()
{
}

Sfx::~Sfx()
{
}

void Sfx::init()
{
	// Select a driver
	
	//FSOUND_SetOutput(m_outputMode);
	reserve_voices(64, 0);
	if(m_outputMode == -1)
	{
		detect_digi_driver(DIGI_AUTODETECT);
		install_sound(DIGI_AUTODETECT, MIDI_NONE, NULL);
	}
	else
	{
		detect_digi_driver(m_outputMode);
		install_sound(m_outputMode, MIDI_NONE, NULL);
	}

	registerSampleFormats();
	
	/* We select default driver for now
	int numDrivers = FSOUND_GetNumDrivers();
	// TODO: Desired driver here: string desiredDriver(); 
	int selectedDriver = 0;
	
	for(int i = 0; i < numDrivers; ++i)
	{
		const char* driverName = FSOUND_GetDriverName(i);
		console.addLogMsg(string("* FMOD DRIVER ") + cast<string>(i) + string(": ") + driverName);
		if(string(driverName).find(desiredDriver) >= 0)
		{
			selectedDriver = i;
			break;
		}
	}
	
	FSOUND_SetDriver(selectedDriver);*/

	//FSOUND_Init(44100, 32, 0);
	//FSOUND_3D_SetDistanceFactor(20);
	//FSOUND_3D_SetRolloffFactor(2);
	volumeChange();
	
	//console.addLogMsg(string("* FMOD LIB INITIALIZED, USING DRIVER ") + FSOUND_GetDriverName(FSOUND_GetDriver()));
	//console.addLogMsg(string("* SOUND INITIALIZED, USING DRIVER ") + FSOUND_GetDriverName(FSOUND_GetDriver()));
	console.addLogMsg(string("* SOUND INITIALIZED, USING WHATEVER YOU SET UP IN allegro.cfg"));
	m_initialized = true;
}

void Sfx::shutDown()
{
	remove_sound();
}

void Sfx::registerInConsole()
{
	{
		EnumVariable::MapType outputModes;
		insert(outputModes)
			("AUTO", DIGI_AUTODETECT)
			("NOSFX", DIGI_NONE)
#ifdef WINDOWS
			("WINMM", DIGI_WAVOUTID(0))
			("DSOUND", DIGI_DIRECTX(0))
#else //ifdef LINUX
			//("A3D", FSOUND_OUTPUT_A3D) // Is this Linux, Windows or both?
			("OSS", DIGI_OSS)
			("ESD", DIGI_ESD)
			("ARTS", DIGI_ARTS) // OMEGALUL --GM
			("ALSA", DIGI_ALSA)
			("JACK", DIGI_JACK)
			//("ASIO", FSOUND_OUTPUT_ASIO) //What's this
#endif

#if 0 //These aren't useful at the moment
			("XBOX", FSOUND_OUTPUT_XBOX)
			("PS2", FSOUND_OUTPUT_PS2)
			("MAC", FSOUND_OUTPUT_MAC)
			("GC", FSOUND_OUTPUT_GC)
#endif
		;

		console.registerVariables()
			("SFX_VOLUME", &m_volume, 255, volume)
			("SFX_LISTENER_DISTANCE", &m_listenerDistance, 20)
			(new EnumVariable("SFX_OUTPUT_MODE", &m_outputMode, -1, outputModes))
		;
		
		// NOTE: When/if adding a callback to sfx variables, make it do nothing if
		// sfx.operator bool() returns false.
	}
}

void Sfx::calculateVolumes( float loudness, float x, float y, int *outLoudness, int *outPanning )
{
	// TODO: use OpenAL, because I really can't be arsed mixing this myself --GM

	float volAccumL = 0.0f;
	float volAccumR = 0.0f;

	// Assuming a 60 degree angle for left and right,
	// 30 degrees off centre.
	constexpr float panX = 0.5f; // sin(30 deg)
	constexpr float panY = 0.0f;
	constexpr float panZ = 0.707f; // roughly sqrt(2)/2, or cos(30 deg)

	const float panDist = m_listenerDistance;

	for (size_t i = 0; i < listeners.size(); ++i )
	{
		// Get distance
		float dx = x - listeners[i]->pos.x;
		float dy = y - listeners[i]->pos.y;
		float dz = m_listenerDistance;
		float dist2 = dx*dx + dy*dy + dz*dz;
		float dist = sqrtf(dist2);

		// Normalise
		float ndx = dx / dist;
		float ndy = dy / dist;
		float ndz = dz / dist;

		//cerr << "norm " << i << ": " << ndx << ", " << ndy << ", " << ndz << " x " << dist << endl;

		// Compute dot products for each channel
		float facL = (ndx*-panX + ndy*panY + ndz*panZ);
		float facR = (ndx*+panX + ndy*panY + ndz*panZ);

		// Calculate base loudness
		float baseLoudness = loudness/dist;
		if ( baseLoudness > 1.0f )
		{
			baseLoudness = 1.0f;
		}

		// Now multiply by the factors and accumulate
		volAccumL += baseLoudness*facL;
		volAccumR += baseLoudness*facR;
	}

	// Allegro 4's sound system likely doesn't have a concept of a negative volume.
	// So we're going to take the absolute value of these.
	// (clamping at zero tends to make things "overpanned" --GM)

	if ( volAccumL < 0.0f )
	{
		volAccumL = -volAccumL;
	}

	if ( volAccumR < 0.0f )
	{
		volAccumR = -volAccumR;
	}

	// And now we calculate volume and panning.
	float maxVol = (volAccumL > volAccumR ? volAccumL : volAccumR);
	*outLoudness = (int)(255*maxVol);

	if ( volAccumR >= volAccumL )
	{
		*outPanning = (int)(255 - 127*volAccumL/volAccumR);
	}
	else
	{
		*outPanning = (int)(127*volAccumR/volAccumL);
	}

	//cerr << "convert at " << x << ", " << y << ", " << m_listenerDistance << " to " << volAccumL << ", " << volAccumR << " to V=" << *outLoudness << ", P=" << *outPanning << endl;
}

void Sfx::think()
{
	for (size_t i = 0; i < listeners.size(); ++i )
	{
		//FSOUND_3D_Listener_SetCurrent(i,listeners.size());
		//float pos[3] = { listeners[i]->pos.x, listeners[i]->pos.y, -m_listenerDistance };
		//FSOUND_3D_Listener_SetAttributes(pos,NULL,0,0,1,0,1,0);
	}
	
	//Update 3d channel that follow objects positions
	/*
	list< pair< int, BaseObject* > >::iterator obj, next;
	for ( obj = chanObject.begin(); obj != chanObject.end(); obj = next)
	{
		next = boost::next(obj);

	*/
	foreach_delete(obj, chanObject)
	{

		if( !obj->second
		||  obj->second->deleteMe
		||  voice_get_position( obj->first ) == -1 )
		{
			if( voice_check( obj->first ) != NULL )
			{
				release_voice( obj->first );
			}
			chanObject.erase(obj);
		}
		else
		{
			if ( chanLoudness.find( obj->first ) != chanLoudness.end() )
			{
				float loudness = chanLoudness.at( obj->first );
				int chanLoudness = 255*loudness/100.0f;
				int chanPanning = 128;
				sfx.calculateVolumes( loudness, obj->second->pos.x, obj->second->pos.y, &chanLoudness, &chanPanning );
				const int rampTime = 2; // this was good enough for ImpulseTracker
				voice_ramp_volume( obj->first, rampTime, chanLoudness );
				voice_sweep_pan( obj->first, rampTime, chanPanning );
				//cerr << "sweeping vol " << chanLoudness << ", pan " << chanPanning << endl;
				//FSOUND_3D_SetAttributes(obj->first, pos, NULL);
			}
		}
	}

/*
	//Check for deleted objects
	for ( obj = chanObject.begin(); obj != chanObject.end(); )
	{
		if ( obj->second && ( obj->second->deleteMe || !FSOUND_IsPlaying( obj->first ) ) )
		{
			list< pair< int, BaseObject* > >::iterator tmp = obj;
			obj++;
			chanObject.erase(tmp);
		}
		else
			obj++;
	}*/

}

void Sfx::setChanObject(int chan, BaseObject* object)
{
	chanObject.push_back( pair< int, BaseObject* > ( chan, object ) );
	chanPosMap.erase(chan);
}

void Sfx::setChanPos(int chan, float x, float y)
{
	chanPosMap.emplace(chan, Vec(x, y));
}

void Sfx::setChanLoudness(int chan, float loudness)
{
	chanLoudness.emplace(chan, loudness);
}

void Sfx::clearChanPos(int chan)
{
	chanPosMap.erase(chan);
	chanLoudness.erase(chan);
}

void Sfx::clear()
{
	chanObject.clear();
	chanPosMap.clear();
	chanLoudness.clear();
}

Listener* Sfx::newListener()
{
	listeners.push_back( new Listener );
	return listeners.back();
}

void Sfx::freeListener(Listener* listener)
{
	vector<Listener*>::iterator i;
	for ( i = listeners.begin(); i != listeners.end(); ++i )
	{
		if ( listener == *i )
		{
			delete *i;
			listeners.erase(i);
			break;
		}
	}
}

void Sfx::volumeChange()
{
	set_volume(m_volume, 0);
}

Sfx::operator bool()
{
	return m_initialized;
} 

SAMPLE* Sfx::loadOggFile(const char *filename)
{
	//cerr << "Attempting to load ogg file \"" << string(filename) << "\"" << endl;

	OggVorbis_File vf;
	int didOpen = ov_fopen(filename, &vf);
	if( didOpen != 0 )
	{
		//cerr << "Error: Could not load file with libvorbisfile (error = " << didOpen << ")" << endl;
		return NULL;
	}

	long streamCount = ov_streams(&vf);
	if( streamCount != 1 )
	{
		cerr << "ERROR: Expected exactly 1 Ogg Vorbis bitstream, got " << streamCount << " instead" << endl;
		ov_clear(&vf);
		return NULL;
	}

	vorbis_info *vi = ov_info(&vf, 0);
	if( !vi )
	{
		cerr << "ERROR: Could not fetch vorbis_info structure" << endl;
		ov_clear(&vf);
		return NULL;
	}
	if( vi->channels != 1 && vi->channels != 2 )
	{
		cerr << "ERROR: Only mono or stereo samples are supported, this file has " << vi->channels << " channels" << endl;
		ov_clear(&vf);
		return NULL;
	}

	ogg_int64_t totalSamples = ov_pcm_total(&vf, 0);
	if( totalSamples < 0 )
	{
		cerr << "ERROR: Could not fetch total PCM sample count for Ogg Vorbis bitstream: " << totalSamples << endl;
		ov_clear(&vf);
		return NULL;
	}

	//cerr << "Vorbis version = " << vi->version << ", channels = " << vi->channels << ", rate = " << vi->rate << "Hz, samples = " << totalSamples << endl;

	SAMPLE* sample = create_sample(16, (vi->channels == 2 ? 1 : 0), vi->rate, totalSamples);
	if( !sample )
	{
		cerr << "ERROR: Could not create Allegro SAMPLE for Ogg Vorbis file" << endl;
		ov_clear(&vf);
		return NULL;
	}

	memset(sample->data, 0, totalSamples*sizeof(int16_t)*vi->channels);

	int byteOffset = 0;
	int totalBytesToRead = totalSamples*sizeof(int16_t)*vi->channels;
	int bytesToRead = totalBytesToRead;
	int currentBitstream = 0;
	while(bytesToRead > 0)
	{
		assert(byteOffset < totalBytesToRead);
		char *bufBase = (char *)(((uint8_t *)sample->data) + byteOffset);

		//long bytesActuallyRead = ov_read(&vf, bufBase, bytesToRead, 0, 2, 1, &currentBitstream);
		// For some reason Allegro expects unsigned samples?!
		long bytesActuallyRead = ov_read(&vf, bufBase, bytesToRead, 0, 2, 0, &currentBitstream);

		if(bytesActuallyRead < 0)
		{
			cerr << "ERROR: Read failed at byte offset " << byteOffset << ": " << bytesActuallyRead << endl;
			destroy_sample(sample);
			ov_clear(&vf);
			return NULL;
		}

		if(bytesActuallyRead == 0)
		{
			// This shouldn't happen, but just
			cerr << "WARNING: Premature EOF found at byte offset " << byteOffset << endl;
			break;
		}

		assert((long)bytesToRead >= bytesActuallyRead);
		bytesToRead -= (int)bytesActuallyRead;
		byteOffset += (int)bytesActuallyRead;
	}
	assert(bytesToRead >= 0);

	ov_clear(&vf);
	return sample;
}

void Sfx::registerSampleFormats()
{
	register_sample_file_type("ogg", loadOggFile, NULL);
}

#endif
