#include "base_player.h"
#include "worm/worm.h"
#include "ninjarope.h"
#include "player_options.h"
#include "objects_list.h"
#include "gconsole.h"
#include "weapon_type.h"
#include "util/log.h"

#include "glua.h"
#include "lua/bindings-game.h"
#include "game.h"

//#include <allegro.h>
#include <list>

using std::list;
using std::vector;

BasePlayer::Stats::~Stats()
{
	if(luaData)
		lua.destroyReference(luaData);
}

/*
LuaReference BasePlayer::metaTable()
{
	return LuaBindings::playerMetaTable;
}
*/

BasePlayer::BasePlayer(shared_ptr<PlayerOptions> options, BaseWorm* worm)
: m_options(options), stats(new Stats), deleteMe(false)
, m_worm(NULL)
, colour(options->colour)
, team(options->team)
, luaData(0), local(false), deleted(false)
{
	localChangeName(m_options->name);
	m_options->clearChangeFlags();
	if(worm)
		assignWorm(worm);
}

LuaReference BasePlayer::getLuaReference()
{
	assert(!deleted);
	if(luaReference)
		return luaReference;
	else
	{
		lua.pushFullReference(*this, LuaBindings::BasePlayerMetaTable);
		luaReference = lua.createReference();
		return luaReference;
	}
}

void BasePlayer::pushLuaReference()
{
	lua.push(getLuaReference());
}

void BasePlayer::deleteThis()
{
	EACH_CALLBACK(i, playerRemoved)
	{
		(lua.call(*i), getLuaReference())();
	}
	
	deleted = true; // We set this after the callback loop otherwise getLuaReference() will think the player is already deleted
	
	for (Grid::iterator objIter = game.objects.beginAll(); objIter; ++objIter)
	{
		objIter->removeRefsToPlayer(this);
	}

	removeWorm();

	if(luaReference)
	{
		lua.destroyReference(luaReference);
		luaReference.reset();
	}
	else
	{
		delete this;
	}
}



BasePlayer::~BasePlayer()
{
	m_worm = 0;
}

void BasePlayer::removeWorm()
{
	if ( m_worm )
	{
		m_worm->deleteMe = true;
		if ( m_worm->getNinjaRopeObj() ) 
			m_worm->getNinjaRopeObj()->deleteMe = true;
		m_worm = 0;
	}
}

void BasePlayer::think()
{
	subThink();

	if ( m_options->nameChanged() )
	{
		changeName_(m_options->name);
	}
	
	if( m_options->colorChanged() )
	{
		changeColor_(m_options->colour);
	}
	
	if( m_options->teamChanged() )
	{
		changeTeam_(m_options->team);
	}
	
	EACH_CALLBACK(i, playerUpdate)
	{
		(lua.call(*i), getLuaReference())();
	}
}

bool nameIsTaken( const std::string& name )
{
	list<BasePlayer*>::iterator playerIter;
	for ( playerIter = game.players.begin(); playerIter != game.players.end(); playerIter++ )
	{
		if ( (*playerIter)->m_name == name )
		{
			return true;
		}
	}
	return false;
}

void BasePlayer::changeName( std::string const& name)
{
	m_options->changeName(name);
}

void BasePlayer::localChangeName(std::string const& name, bool forceChange)
{
	if(m_name == name)
		return;
	
	string nameToUse = name;
	
	if(!forceChange)
	{
		int nameSuffix = 0;
		while ( nameIsTaken( nameToUse ) )
		{
			++nameSuffix;
			nameToUse = name + "(" + cast<string>(nameSuffix) + ")";
		}
	}
	
	if(!m_name.empty())
		console.addLogMsg( "* " + m_name + " CHANGED NAME TO " + nameToUse );

	m_name = nameToUse;
}

void BasePlayer::changeName_( const std::string& name )
{
	localChangeName(name);
}

void BasePlayer::changeColor_( int colour_ )
{
	colour = colour_;
}

void BasePlayer::changeTeam_( int team_ )
{
	team = team_;
}

void BasePlayer::sendChatMsg( std::string const& message )
{
	game.displayChatMsg( m_name, message );
}

void BasePlayer::selectWeapons( vector< WeaponType* > const& weaps )
{
	if ( m_worm )
	{
		m_worm->setWeapons( weaps );
	}
}

void BasePlayer::assignWorm(BaseWorm* worm)
{
	m_worm = worm;
	worm->assignOwner( this );
}

shared_ptr<PlayerOptions> BasePlayer::getOptions()
{
	return m_options;
}

#if 0
void BasePlayer::assignNetworkRole( bool authority )
{
	m_node = new ZCom_Node();
	if (!m_node)
	{
		allegro_message("ERROR: Unable to create player node.");
	}

	m_node->beginReplicationSetup(1);
		//m_node->addReplicationInt( (zS32*)&deaths, 32, false, ZCOM_REPFLAG_MOSTRECENT, ZCOM_REPRULE_AUTH_2_ALL , 0);
		m_node->setInterceptID( static_cast<ZCom_InterceptID>(WormID) );
		m_node->addReplicationInt( (zS32*)&m_wormID, 32, false, ZCOM_REPFLAG_MOSTRECENT | ZCOM_REPFLAG_INTERCEPT, ZCOM_REPRULE_AUTH_2_ALL , INVALID_NODE_ID);
	m_node->endReplicationSetup();

	m_interceptor = new BasePlayerInterceptor( this );
	m_node->setReplicationInterceptor(m_interceptor);

	m_isAuthority = authority;
	if( m_isAuthority)
	{
		m_node->setEventNotification(true, false); // Enables the eEvent_Init.
		if( !m_node->registerNodeDynamic(classID, network.getZControl() ) )
			allegro_message("ERROR: Unable to register player authority node.");
	}else
	{
		m_node->setEventNotification(false, true); // Same but for the remove event.
		if( !m_node->registerRequestedNode( classID, network.getZControl() ) )
		allegro_message("ERROR: Unable to register player requested node.");
	}

	m_node->applyForZoidLevel(1);
}
#endif


#if 0
void BasePlayer::sendSyncMessage( ZCom_ConnID id )
{
	ZCom_BitStream *data = new ZCom_BitStream;
	addEvent(data, SYNC);
	data->addInt(stats->kills, 32);
	data->addInt(stats->deaths, 32);
	data->addString( m_name.c_str() );
	data->addInt(colour, 24);
	data->addSignedInt(static_cast<int>(team), 8);
	data->addInt(static_cast<int>(m_options->uniqueID), 32);
	m_node->sendEventDirect(eZCom_ReliableOrdered, data, id);
}
#endif

void BasePlayer::baseActionStart ( BaseActions action )
{
	switch (action)
	{
		case LEFT:
		{
			if ( m_worm )
			{
				m_worm -> actionStart(Worm::MOVELEFT);
			}
		}
		break;
		
		case RIGHT:
		{
			if ( m_worm )
			{
				m_worm -> actionStart(Worm::MOVERIGHT);
			}
		}
		break;
		
		case FIRE:
		{
			if ( m_worm )
			{
				m_worm -> actionStart(Worm::FIRE);
			}
		}
		break;
		
		case JUMP:
		{
			if ( m_worm )
			{
				m_worm -> actionStart(Worm::JUMP);
			}
		}
		break;
		
		case NINJAROPE:
		{
			if ( m_worm )
			{
				m_worm->actionStart(Worm::NINJAROPE);
			}
		}
		break;
		
		case DIG:
		{
			if ( m_worm )
			{
				m_worm->dig();
			}
		}
		break;
		
		case RESPAWN:
		{
			if ( m_worm && !m_worm->isActive() )
			{
				m_worm->respawn();
			}
		}
		break;
		
		case ACTION_COUNT: break; // Do nothing
	}
}



void BasePlayer::baseActionStop ( BaseActions action )
{
	switch (action)
	{
		case LEFT:
		{
			if ( m_worm )
			{
				m_worm -> actionStop(Worm::MOVELEFT);
			}
		}
		break;
		
		case RIGHT:
		{
			if ( m_worm )
			{
				m_worm -> actionStop(Worm::MOVERIGHT);
			}
		}
		break;
		
		case FIRE:
		{
			if ( m_worm )
			{
				m_worm -> actionStop(Worm::FIRE);
			}
		}
		break;
		
		case JUMP:
		{
			if ( m_worm )
			{
				m_worm -> actionStop(Worm::JUMP);
			}
		}
		break;
		
		case NINJAROPE:
		{
			if ( m_worm )
			{
				m_worm->actionStop(Worm::NINJAROPE);
			}
		}
		
		case DIG: break; // Doesn't stop
			
		case RESPAWN: break; // Doesn't stop
		
		case ACTION_COUNT: break; // Do nothing
	}
}

