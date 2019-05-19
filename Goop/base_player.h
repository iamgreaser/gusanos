#ifndef BASE_PLAYER_H
#define BASE_PLAYER_H

#include <string>
//#include "vec.h"
#include "luaapi/types.h"
#include <stdexcept>
#include <boost/shared_ptr.hpp>
#include <vector>
using boost::shared_ptr;

struct PlayerOptions;
class BaseWorm;
class BasePlayerInterceptor;
class WeaponType;
struct LuaEventDef;

// Note: None of the BaseActions should assume a combination of keys.
// For example: Activating JUMP and CHANGE does nothing here ( instead 
// of shooting the Ninja Rope ) So key combinations should be created
// on the Player class instead. Because of that, all actions in the 
// BasePlayer class are direct ( they do nothing more and nothing less
// than what the name tells )

// Note2: All access to the worm class from a derivation of BasePlayer
// should pass by the BasePlayer class ( This is because the BasePlayer
// class will be responsible of the network part )

class BasePlayer
{
public:
	
	enum BaseActions
	{
		LEFT = 0,
		RIGHT,
		//UP,
		//DOWN,
		FIRE,
		JUMP,
		//CHANGE, // Probably useless Action
		NINJAROPE,
		DIG,
		RESPAWN,
		//
		ACTION_COUNT,
	};
	
	enum NetEvents
	{
		SYNC = 0,
		ACTION_STOP,
		ACTION_START,
		NAME_CHANGE,
		CHAT_MSG,
		COLOR_CHANGE,
		SELECT_WEAPONS,
		TEAM_CHANGE,
		LuaEvent,
		//
		EVENT_COUNT,
	};
	
	enum ReplicationItems
	{
		WormID,
		Other
	};
	
	struct Stats
	{
		Stats()
		: deaths(0), kills(0)
		{
		}
		
		~Stats();
		
		int deaths;
		int kills;
		LuaReference luaData;
	};
	
	//static LuaReference metaTable();
	
	BasePlayer(shared_ptr<PlayerOptions> options, BaseWorm* worm);
	virtual ~BasePlayer();
	
	void think();
	// subThink() gets called inside think() and its used to give the derivations
	// the ability to think without replacing the main BasePlayer::think().
	virtual void subThink() = 0;
#ifndef DEDSERV
	virtual void render() {}
#endif

	void assignWorm(BaseWorm* worm);
	void removeWorm();

	void sendChatMsg( std::string const& message );
	//void sendSyncMessage( ZCom_ConnID id ); // Its the initializing message that is sent to new clients that recieve the node.
	
	//void nameChangePetition(); // Asks the server to change the name to the one in the player options.
	
	void baseActionStart( BaseActions action );
	void baseActionStop( BaseActions action );
	
	void addKill();
	void addDeath();

	//void sendLuaEvent(LuaEventDef* event, eZCom_SendMode mode, zU8 rules, ZCom_BitStream* userdata, ZCom_ConnID connID);
	shared_ptr<PlayerOptions> getOptions();
	BaseWorm* getWorm() { return m_worm; }
	
	LuaReference getLuaReference();
	void pushLuaReference();
	virtual void deleteThis();
	
/*
	void* operator new(size_t count);

	void operator delete(void* block)
	{

	}
	
	void* operator new(size_t count, void* space)
	{
		return space;
	}
*/
	shared_ptr<Stats> stats;
	
	bool deleteMe;
	
	std::string m_name;
	int colour;
	int team;
	bool local;

	LuaReference luaData;
	
	void selectWeapons( std::vector< WeaponType* > const& weaps );
	
	void changeName( std::string const& name);
	
	// Changes the name only locally
	void localChangeName(std::string const& name, bool forceChange = false);
	
protected:
	LuaReference luaReference;
	
	//void addEvent(ZCom_BitStream* data, NetEvents event);
	//void addActionStart(ZCom_BitStream* data, BaseActions action);
	//void addActionStop(ZCom_BitStream* data, BaseActions action);
	
	void changeName_( const std::string& name ); // Changes the name and if its server it will tell all clients about it.	
	void changeColor_( int colour_ );
	void changeTeam_( int team_ );

	BaseWorm* m_worm;
	shared_ptr<PlayerOptions> m_options;

	bool deleted; //TEMP
};

#endif  // _BASE_PLAYER_H_
