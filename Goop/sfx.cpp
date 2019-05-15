#ifndef DEDSERV

#include "sfx.h"
#include "gconsole.h"
#include "base_object.h"
#include "util/macros.h"
#include <boost/assign/list_inserter.hpp>
using namespace boost::assign;

#include <vector>
#include <list>
#include <boost/utility.hpp>

using namespace std;

Sfx sfx;

namespace
{
	bool m_initialized = false;
	
	std::list< std::pair< int, BaseObject* > > chanObject;
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

void Sfx::think()
{
	for (size_t i = 0; i < listeners.size(); ++i )
	{
		//FSOUND_3D_Listener_SetCurrent(i,listeners.size());
		float pos[3] = { listeners[i]->pos.x, listeners[i]->pos.y, -m_listenerDistance };
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
			float pos[3] = { obj->second->pos.x, obj->second->pos.y, 0 };
			//FSOUND_3D_SetAttributes(obj->first, pos, NULL);
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
}
	
void Sfx::clear()
{
	chanObject.clear();
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

#endif
