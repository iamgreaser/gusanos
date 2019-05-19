#include "game.h"

#include "base_worm.h"
#include "worm.h"
#include "part_type.h"
#include "exp_type.h"
#include "weapon_type.h"
#include "base_object.h"
#include "player/player.h"
#include "player/player_options.h"
#include "particle.h"
#include "explosion.h"
#include "exp_type.h"
#include "level_effect.h"
#include "level.h"
#include "gconsole.h"
#include "game_actions.h"
#include "player/base_player.h"
#include "gfx.h"
#include "sprite_set.h"
#include "util/macros.h"
#include "util/log.h"
#ifndef DEDSERV
#include "sfx.h"
#include "sound.h"
#include "sound1d.h"
#include "font.h"
#include "menu.h"
#include "input/keyboard.h"
#include "input/mouse.h"
#include "player/player_input.h"
#include "viewport.h"
#endif //DEDSERV
#include "player/player_ai.h"
#include "script.h"
#include "ninjarope.h"
#include "hash_table.h"

#include "loaders/gusanos.h"
#include "loaders/lierox.h"
#include "loaders/liero.h"
#include "loaders/losp.h"
#include "glua.h"
#include "lua/bindings.h"

#include <allegro.h>
#include <string>
#include <algorithm>
#include <list>
#include <iostream>
#include <sstream> //TEMP

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/path.hpp>

namespace fs = boost::filesystem;

//using namespace std; // Conflicting
using std::string;
using std::list;
using std::vector;
using std::cerr;
using std::cout;
using std::endl;

namespace
{
	enum NetEvents
	{
		eHole = 0,
		// Add here
		LuaEvent,
		//RegisterLuaEvents, //oobsol33t
		NetEventsCount,
	};

	std::list<LevelEffectEvent> appliedLevelEffects;

	std::string nextMod;
	fs::path    m_modPath;
	std::string m_modName;
	fs::path    m_defaultPath;
	bool loaded;
	HashTable<std::string, unsigned long> stringToIndexMap;
	std::vector<std::string> indexToStringMap;
	
	boost::uint32_t getWeaponCRC()
	{
		boost::uint32_t v = 0;
		foreach(i, game.weaponList)
		{
			v ^= (*i)->crc;
			++v;
		}
		
		return v;
	}
}

Game game;

string mapCmd(const list<string> &args)
{
	if (!args.empty())
	{
		string tmp = *args.begin();
		std::transform(tmp.begin(), tmp.end(), tmp.begin(), (int(*)(int)) tolower);
		/*
		if(!game.changeLevelCmd( tmp ))
			return "ERROR LOADING MAP";
		*/
		//mq_queue(game.msg, Game::ChangeLevel, tmp);
		game.changeLevelCmd( tmp );
		return "";
	}
	return "MAP <MAPNAME> : LOAD A MAP";
}

struct MapIterGetText
{
	template<class IteratorT>
	std::string const& operator()(IteratorT i) const
	{
		return i->first;
	}
};

string mapCompleter(Console* con, int idx, std::string const& beginning)
{
	if(idx != 0)
		return beginning;
		
	return shellComplete(
		levelLocator.getMap(),
		beginning.begin(),
		beginning.end(),
		MapIterGetText(),
		ConsoleAddLines(*con)
	);
}

string gameCmd(const list<string> &args)
{
	if (!args.empty())
	{
		string tmp = *args.begin();
		std::transform(tmp.begin(), tmp.end(), tmp.begin(), (int(*)(int)) tolower);
		if(!game.setMod( tmp ))
			return "MOD " + tmp + " NOT FOUND";
		return "THE GAME WILL CHANGE THE NEXT TIME YOU CHANGE MAP";
	}
	return "GAME <MODNAME> : SET THE MOD TO LOAD THE NEXT MAP CHANGE";
}

struct GameIterGetText
{
	template<class IteratorT>
	std::string const& operator()(IteratorT i) const
	{
		return *i;
	}
};

string gameCompleter(Console* con, int idx, std::string const& beginning)
{
	if(idx != 0)
		return beginning;
		
	return shellComplete(
		game.modList,
		beginning.begin(),
		beginning.end(),
		GameIterGetText(),
		ConsoleAddLines(*con)
	);
}

string addbotCmd(const list<string> &args)
{
	int team = -1;
	list<string>::const_iterator i = args.begin();
	if(i != args.end())
	{
		team = cast<int>(*i);
		++i;
	}
	game.addBot(team);
	return "";
}

#if 0
string connectCmd(const list<string> &args)
{
	if ( !args.empty() )
	{
#ifndef DISABLE_ZOIDCOM
		network.connect( *args.begin() );
#endif
		return "";
	}
	return "CONNECT <HOST_ADDRESS> : JOIN A NETWORK SERVER";
}

string rConCmd(const list<string> &args)
{
	if ( !args.empty() && network.isClient() )
	{
		
		list<string>::const_iterator iter = args.begin();
		string tmp = *iter++;
		for (; iter != args.end(); ++iter )
		{
			tmp += " \"" + *iter + '"';
		}
		game.sendRConMsg( tmp );
		return "";
	}
	return "";
}

string rConCompleter(Console* con, int idx, std::string const& beginning)
{
	if(idx != 0)
		return beginning;
		
	return con->completeCommand(beginning);
}
#endif

BasePlayer* findPlayerByName(std::string const& name)
{
	//BasePlayer* player2Kick = 0;
	//for ( std::list<BasePlayer*>::iterator iter = game.players.begin(); iter != game.players.end(); iter++)
	foreach(iter, game.players)
	{
		if ( (*iter)->m_name == name )
		{
			return *iter;
		}
	}
	
	return 0;
}

#if 0
string banCmd(list<string> const& args)
{
	if ( !network.isClient() && !args.empty() )
	{
		if ( BasePlayer* player = findPlayerByName(*args.begin()))
		if ( !player->local )
		{
			network.ban( player->getConnectionID() );
			return "PLAYER BANNED";
		}
		return "PLAYER NOT FOUND OR IS LOCAL";
	}
	return "BAN <PLAYER_NAME> : BANS THE PLAYER WITH THE SPECIFIED NAME";
}

string kickCmd(const list<string> &args)
{
	if ( !network.isClient() && !args.empty() )
	{
		if ( BasePlayer* player2Kick = findPlayerByName(*args.begin()))
		if ( !player2Kick->local )
		{
			player2Kick->deleteMe = true;
			network.kick( player2Kick->getConnectionID() );
			return "PLAYER KICKED";
		}
		return "PLAYER NOT FOUND OR IS LOCAL";
	}
	return "KICK <PLAYER_NAME> : KICKS THE PLAYER WITH THE SPECIFIED NAME";
}
#endif

struct BasePlayerIterGetText
{
	template<class IteratorT>
	std::string const& operator()(IteratorT i) const
	{
		return (*i)->m_name;
	}
};

string kickCompleter(Console* con, int idx, std::string const& beginning)
{
	if(idx != 0)
		return beginning;
		
	return shellComplete(
		game.players,
		beginning.begin(),
		beginning.end(),
		BasePlayerIterGetText(),
		ConsoleAddLines(*con)
	);
}

void Options::registerInConsole()
{
	console.registerVariables()
		("SV_NINJAROPE_SHOOT_SPEED", &ninja_rope_shootSpeed, 2)
		("SV_NINJAROPE_PULL_FORCE", &ninja_rope_pullForce, 0.031)
		//("SV_NINJAROPE_PULL_FORCE", &ninja_rope_pullForce, (4.0f / 3.0f) * (70.0f * 70.0f / (100.0f * 100.0f * 16.0f)))
		
		//("SV_NINJAROPE_START_DISTANCE", &ninja_rope_startDistance, 20)
		("SV_NINJAROPE_START_DISTANCE", &ninja_rope_startDistance, 4000.0f / 16.f - 1.f)
		("SV_NINJAROPE_MAX_LENGTH", &ninja_rope_maxLength, 2000.f)

		("SV_WORM_MAX_SPEED", &worm_maxSpeed, 0.45)
		("SV_WORM_ACCELERATION", &worm_acceleration, 0.03)
		("SV_WORM_AIR_ACCELERATION_FACTOR", &worm_airAccelerationFactor, 1.f)
		//("SV_WORM_FRICTION", &worm_friction, 0.02)
		("SV_WORM_FRICTION", &worm_friction, pow(0.89, 0.7))
		("SV_WORM_AIR_FRICTION", &worm_airFriction, 1.f)
		("SV_WORM_GRAVITY", &worm_gravity, 0.009)
		("SV_WORM_DISABLE_WALL_HUGGING", &worm_disableWallHugging, 0)
		//("SV_WORM_BOUNCE_QUOTIENT", &worm_bounceQuotient, 0.3)
		("SV_WORM_BOUNCE_QUOTIENT", &worm_bounceQuotient, 0.333)
		("SV_WORM_BOUNCE_LIMIT", &worm_bounceLimit, 2)
		//("SV_WORM_BOUNCE_LIMIT", &worm_bounceLimit, 0.56875f)
		
		("SV_WORM_JUMP_FORCE", &worm_jumpForce, 0.6)
		("SV_WORM_WEAPON_HEIGHT", &worm_weaponHeight, 5)
		("SV_WORM_HEIGHT", &worm_height, 9)
		("SV_WORM_WIDTH", &worm_width, 3)
		("SV_WORM_MAX_CLIMB", &worm_maxClimb, 4)
 		("SV_WORM_BOX_RADIUS", &worm_boxRadius, 2)
		("SV_WORM_BOX_TOP", &worm_boxTop, 3)
		("SV_WORM_BOX_BOTTOM", &worm_boxBottom, 4)

		("SV_MAX_RESPAWN_TIME", &maxRespawnTime, -1 )
		("SV_MIN_RESPAWN_TIME", &minRespawnTime, 100 )
		("SV_TEAM_PLAY", &teamPlay, 0)

		("HOST", &host, 0)
		
		("SV_MAX_WEAPONS", &maxWeaponsVar, 5)
		("CL_SPLITSCREEN", &splitScreenVar, 0)
			
		("RCON_PASSWORD", &rConPassword, "" )
		("CL_SHOW_MAP_DEBUG", &showMapDebug, 0 )
		("CL_SHOW_DEATH_MESSAGES", &showDeathMessages, true )
		("CL_LOG_DEATH_MESSAGES", &logDeathMessages, false )
	;
	maxWeapons = 5;
	splitScreen = false;
	
	console.registerCommands()
		("MAP", mapCmd, mapCompleter)
		("GAME", gameCmd, gameCompleter)
		("ADDBOT", addbotCmd)
		//("CONNECT", connectCmd)
		//("RCON", rConCmd, rConCompleter)
		//("KICK", kickCmd, kickCompleter)
		//("BAN", banCmd, kickCompleter)
	;
}

Game::Game()
{
	NRPartType = NULL;
	deathObject = NULL;
	loaded = false;
}

Game::~Game()
{

}

void Game::parseCommandLine(int argc, char** argv)
{
	for(int i = 0; i < argc; ++i)
	{
		const char* arg = argv[i];
		if(arg[0] == '-')
		{
			switch(arg[1])
			{
				case 'c':
					if(++i >= argc)
						return;
						
					console.parseLine(argv[i]);
				break;
			}
		}
	}
}

void Game::init(int argc, char** argv)
{
	allegro_init();
	install_timer();

	levelLocator.registerLoader(&GusanosLevelLoader::instance);
	levelLocator.registerLoader(&LieroXLevelLoader::instance);
	levelLocator.registerLoader(&LieroLevelLoader::instance);
	
#ifndef DEDSERV
	fontLocator.registerLoader(&GusanosFontLoader::instance);
	fontLocator.registerLoader(&LOSPFontLoader::instance);
	fontLocator.registerLoader(&LieroFontLoader::instance);

	xmlLocator.registerLoader(&XMLLoader::instance);
	gssLocator.registerLoader(&GSSLoader::instance);
#endif

	scriptLocator.registerLoader(&LuaLoader::instance);
	
	LuaBindings::init();

	m_defaultPath = "default";
	m_modPath = "default";
	m_modName = "default";
	setMod("default");
	refreshResources("default");

	console.init();
#ifndef DEDSERV
	OmfgGUI::menu.init();
	
	sfx.registerInConsole();
#endif
	gfx.registerInConsole();
	options.registerInConsole();

	for ( size_t i = 0; i< MAX_LOCAL_PLAYERS; ++i)
	{
		shared_ptr<PlayerOptions> options(new PlayerOptions);
		options->registerInConsole(i);
		playerOptions.push_back(options);
	}
	
#ifndef DEDSERV
	console.executeConfig("config.cfg");
#else
	console.executeConfig("config-ded.cfg");
#endif

	parseCommandLine(argc, argv);
	
	gfx.init();
#ifndef DEDSERV
	sfx.init();

	keyHandler.init();
	mouseHandler.init();
#endif

	registerGameActions();
#ifndef DEDSERV
	registerPlayerInput();
#endif
}

#if 0
void Game::sendLuaEvent(LuaEventDef* event, eZCom_SendMode mode, zU8 rules, ZCom_BitStream* userdata, ZCom_ConnID connID)
{
	if(!m_node) return;
	
	ZCom_BitStream* data = new ZCom_BitStream;
	addEvent(data, LuaEvent);
	data->addInt(event->idx, 8);
	if(userdata)
	{
		data->addBitStream(userdata);
	}
	if(!connID)
		m_node->sendEvent(mode, rules, data);
	else
		m_node->sendEventDirect(mode, data, connID);
}
#endif

void Game::think()
{
	mq_process_messages(msg)
		mq_case(ChangeLevel)
			// Network is disconnected

			refreshLevels();
			if(!levelLocator.exists(data.level))
				break;
			if(!changeLevel( data.level, false ))
				break;

			runInitScripts();

			// All this is temporal, dont be scared ;D
			if ( loaded && level.isLoaded() ) 
			{
				if(true)
				{
					BaseWorm* worm = addWorm(true);
					addPlayer ( OWNER, -1, worm );
				}
				if(options.splitScreen)
				{
					BaseWorm* worm = addWorm(true);
					addPlayer ( OWNER, -1, worm );
				}
			}
		mq_end_case()
	mq_end_process_messages()

#ifndef DEDSERV
	if(!messages.empty())
	{
		int size = messages.size();
		int step = 1 + (size / 3);
		if((messages.front().timeOut -= step) < 0)
			messages.erase(messages.begin());
	}
#endif

	level.think();
}

void Game::applyLevelEffect( LevelEffect* effect, int x, int y )
{
	level.applyEffect( effect, x, y );
}

void Game::loadWeapons()
{
	fs::path path( m_modPath );
	path /= "weapons";
	
	if ( fs::exists( path ) )
	{
		fs::directory_iterator end_itr;
		
		for( fs::directory_iterator iter(path); iter != end_itr; ++iter)
		{
			if( !is_directory(*iter) )
			{
				if ( fs::extension(*iter) == ".wpn")
				{
					WeaponType* weapon = new WeaponType;
					weapon->load(*iter);
					weaponList.push_back(weapon);
				}
			}
		}
		
		WeaponOrder comp;
		std::sort(weaponList.begin(), weaponList.end(), comp); 
		
		for ( size_t i = 0; i < weaponList.size(); ++i )
		{
			weaponList[i]->setIndex(i);
		}
	}
};

void Game::loadMod(bool doLoadWeapons)
{
	options.maxWeapons = options.maxWeaponsVar;
	options.splitScreen = ( options.splitScreenVar != 0 );
	console.loadResources();
	gfx.loadResources();
	
	NRPartType = partTypeList.load("ninjarope.obj");
	deathObject = partTypeList.load("death.obj");
	digObject = partTypeList.load("wormdig.obj");
#ifndef DEDSERV
	chatSound = sound1DList.load("chat.ogg");
	infoFont = fontLocator.load("minifont");
	assert( infoFont != NULL );
#endif
	assert( NRPartType != NULL || !"by the way, this is for the ninjarope object" );
	if(doLoadWeapons)
	{
		loadWeapons();
		if (weaponList.size() > 0 )
		{
			loaded = true;
		}
		else
		{
			loaded = false;
			console.addLogMsg("ERROR: NO WEAPONS FOUND IN MOD FOLDER");
		}
	}
	console.executeConfig("mod.cfg");
	
	if(!loaded)
		error(ErrorModLoading);
}

void Game::runInitScripts()
{
	Script* modScript = scriptLocator.load(m_modName);
	if(!modScript) modScript = scriptLocator.load("common");
	if(modScript)
	{
		LuaReference ref = modScript->createFunctionRef("init");
		(lua.call(ref))();
	}

	Script* levelScript = scriptLocator.load("map_" + level.getName());
	if(levelScript)
	{
		LuaReference ref = levelScript->createFunctionRef("init");
		(lua.call(ref))();
	}
	levelEffectList.indexate();
	partTypeList.indexate();
}

void Game::reset(ResetReason reason)
{
	// Delete all players
	for ( list<BasePlayer*>::iterator iter = players.begin(); iter != players.end(); ++iter)
	{
		(*iter)->deleteThis();
	}
	players.clear();
	localPlayers.clear();
	
	// Delete all objects
#ifdef USE_GRID
	objects.clear();
#else
	for ( ObjectsList::Iterator iter = objects.begin(); (bool)iter; ++iter)
	{
		(*iter)->deleteThis();
	}
	objects.clear();
#endif

	appliedLevelEffects.clear();
	
	level.unload();
	
	if(reason != LoadingLevel)
	{
		EACH_CALLBACK(i, gameEnded)
		{
			(lua.call(*i), static_cast<int>(reason))();
		}
	}
}

void Game::unload()
{
	//cerr << "Unloading..." << endl;
	loaded = false;
#ifndef DEDSERV
	OmfgGUI::menu.destroy();
	sfx.clear();
#endif
	
	console.clearTemporaries();
	
	reset(LoadingLevel);
/*
	// Delete all players
	for ( list<BasePlayer*>::iterator iter = players.begin(); iter != players.end(); ++iter)
	{
		(*iter)->deleteThis();
	}
	players.clear();
	localPlayers.clear();
	
	// Delete all objects
#ifdef USE_GRID
	objects.clear();
#else
	for ( ObjectsList::Iterator iter = objects.begin(); (bool)iter; ++iter)
	{
		(*iter)->deleteThis();
	}
	objects.clear();
#endif

	appliedLevelEffects.clear();
	
	level.unload();
*/
	for ( vector<WeaponType*>::iterator iter = weaponList.begin(); iter != weaponList.end(); ++iter)
	{
		luaDelete(*iter);
	}
	weaponList.clear();
	
	partTypeList.clear();
	expTypeList.clear();
#ifndef DEDSERV
	soundList.clear();
	sound1DList.clear();
#endif
	spriteList.clear();
	levelEffectList.clear();

#ifndef DEDSERV
	fontLocator.clear();
	xmlLocator.clear();
	gssLocator.clear();
#endif
	scriptLocator.clear();

	lua.reset();
	luaCallbacks = LuaCallbacks(); // Reset callbacks
	LuaBindings::init();
#ifndef DEDSERV
	OmfgGUI::menu.clear();
#endif
}

bool Game::isLoaded()
{
	return loaded;
}

void Game::refreshResources(fs::path const& levelPath)
{
#ifndef DEDSERV
	fontLocator.addPath(levelPath / "fonts");
	fontLocator.addPath(m_defaultPath / "fonts");
	fontLocator.addPath(fs::path(nextMod) / "fonts");
	fontLocator.refresh();

	xmlLocator.addPath(m_defaultPath / "gui");
	xmlLocator.addPath(fs::path(nextMod) / "gui");
	xmlLocator.refresh();
	
	gssLocator.addPath(m_defaultPath / "gui");
	gssLocator.addPath(fs::path(nextMod) / "gui");
	gssLocator.refresh();
#endif
	
	scriptLocator.addPath(levelPath / "scripts");
	scriptLocator.addPath(m_defaultPath / "scripts");
	scriptLocator.addPath(fs::path(nextMod) / "scripts");
	scriptLocator.refresh();
	
	// These are added in reverse order compared to
	// the resource locator paths! Fix maybe?
	partTypeList.addPath(levelPath / "objects");
	partTypeList.addPath(fs::path(nextMod) / "objects");
	partTypeList.addPath(m_defaultPath / "objects");
	
	expTypeList.addPath(levelPath / "objects");
	expTypeList.addPath(fs::path(nextMod) / "objects");
	expTypeList.addPath(m_defaultPath / "objects");
	
#ifndef DEDSERV
	soundList.addPath(levelPath / "sounds");
	soundList.addPath(fs::path(nextMod) / "sounds");
	soundList.addPath(m_defaultPath / "sounds");
	
	sound1DList.addPath(levelPath / "sounds");
	sound1DList.addPath(fs::path(nextMod) / "sounds");
	sound1DList.addPath(m_defaultPath / "sounds");
#endif
	
	spriteList.addPath(levelPath / "sprites");
	spriteList.addPath(fs::path(nextMod) / "sprites");
	spriteList.addPath(m_defaultPath / "sprites");
	
	levelEffectList.addPath(levelPath / "mapeffects");
	levelEffectList.addPath(fs::path(nextMod) / "mapeffects");
	levelEffectList.addPath(m_defaultPath / "mapeffects");
	
	refreshMods();
}

void Game::refreshLevels()
{
	levelLocator.clear();
	levelLocator.addPath(m_defaultPath / "maps");
	levelLocator.addPath(fs::path(nextMod) / "maps");
	levelLocator.refresh();
}

void Game::refreshMods()
{
	modList.clear();
	for( fs::directory_iterator i("."), e; i != e; ++i)
	{
		if( is_directory(*i) )
		{
			if ( fs::exists(*i / "weapons"))
			{
				modList.insert(i->path().filename().native()); // GUS FIXME: there's a good chance this is wrong! --GM
				//modList.insert(i->string());
			}
		}
	}
}

bool Game::changeLevelCmd(const std::string& levelName )
{
	/*
	if( network.isHost() && options.host )
		network.disconnect( Network::ServerMapChange );
	else
		network.disconnect();
	*/
	mq_queue(msg, ChangeLevel, levelName);

	return true;
}

bool Game::reloadModWithoutMap()
{
	unload();
	level.setName("");
	refreshResources("default");
	loadMod(false);
	runInitScripts();
	
	return true;
}

void Game::error(Error err)
{
	EACH_CALLBACK(i, gameError)
	{
		(lua.call(*i), static_cast<int>(err))();
	}
}

bool Game::hasLevel(std::string const& level)
{
	return levelLocator.exists(level);
}

bool Game::hasMod(std::string const& mod)
{
	return modList.find(mod) != modList.end();
}

bool Game::changeLevel(const std::string& levelName, bool refresh )
{
	if(refresh)
		refreshLevels();
		
	if(!levelLocator.exists(levelName))
	{
		error(ErrorMapNotFound);
		return false;
	}
		
	fs::path const& levelPath = levelLocator.getPathOf(levelName);
	
	unload();
	
	m_modName = nextMod;
	m_modPath = nextMod;

	level.setName(levelName);
	refreshResources(levelPath);
	//cerr << "Loading level" << endl;
	
	if(!levelLocator.load(&level, levelName))
	{
		reloadModWithoutMap();
		error(ErrorMapLoading);
		return false;
	}

#ifdef USE_GRID
	objects.resize(0, 0, level.width(), level.height());
#endif
	
	//cerr << "Loading mod" << endl;
	loadMod();
	
	return true;
}

#if 0
void Game::sendRConMsg( string const& message )
{
	ZCom_BitStream *req = new ZCom_BitStream;
	req->addInt(Network::RConMsg, 8);
	req->addString( options.rConPassword.c_str() );
	req->addString( message.c_str() );
	network.getZControl()->ZCom_sendData( network.getServerID(), req, eZCom_ReliableOrdered );
}
#endif

bool Game::setMod( const string& modname )
{
	if( fs::exists(modname) )
	{
		nextMod = modname;
	}
	else
	{
		nextMod = m_modName;
		return false;
	}
	
	refreshLevels();
	
	return true;
}

void Game::displayChatMsg( std::string const& owner, std::string const& message)
{
	console.addLogMsg('<' + owner + "> " + message);
	displayMessage(ScreenMessage(ScreenMessage::Chat, '{' + owner + "}: " + message, 800));
	
#ifndef DEDSERV
	if ( chatSound ) chatSound->play();
#endif
}

void Game::displayKillMsg( BasePlayer* killed, BasePlayer* killer )
{
	std::string str = "{" + killed->m_name + "} ";
	
	if(killed != killer)
	{
		str += "got killed by";
		if(killer)
			str += " {" + killer->m_name + '}';
		else
			str += " {\01305anonymous}";
	}
	else
	{
		str += "commited suicide";
	}
	
	if ( options.showDeathMessages ) displayMessage(ScreenMessage(ScreenMessage::Death, str, 400));
	if ( options.logDeathMessages ) console.addLogMsg(str);
}

void Game::displayMessage( ScreenMessage const& msg )
{
	messages.push_back(msg);
}

const string& Game::getMod()
{
	return m_modName;
}

fs::path const& Game::getModPath()
{
	return m_modPath;
}

fs::path const& Game::getDefaultPath()
{
	return m_defaultPath;
}

#if 0
BasePlayer* Game::findPlayerWithID( ZCom_NodeID ID )
{
	list<BasePlayer*>::iterator playerIter;
	for ( playerIter = game.players.begin(); playerIter != game.players.end(); ++playerIter)
	{
		if ( (*playerIter)->getNodeID() == ID )
		{
			return (*playerIter);
		}
	}
	return NULL;
}
#endif

void Game::insertExplosion( Explosion* explosion )
{
#ifdef USE_GRID
	game.objects.insert( explosion, Grid::NoColLayer, explosion->getType()->renderLayer);
#else
	game.objects.insert( NO_COLLISION_LAYER, explosion->getType()->renderLayer, explosion );
#endif
}

BasePlayer* Game::addPlayer( PLAYER_TYPE type, int team, BaseWorm* worm )
{
	BasePlayer* p = 0;
	switch(type)
	{
		case OWNER:
		{
			if ( localPlayers.size() >= MAX_LOCAL_PLAYERS ) allegro_message("OMFG Too much local players");
			Player* player = new Player( playerOptions[localPlayers.size()], worm );
#ifndef DEDSERV
			Viewport* viewport = new Viewport;
			if ( options.splitScreen )
			{
				viewport->setDestination(gfx.buffer,localPlayers.size()*160,0,160,240);
			}else
			{
				viewport->setDestination(gfx.buffer,0,0,320,240);
			}
			player->assignViewport(viewport);
#endif
			players.push_back( player );
			localPlayers.push_back( player );
			player->local = true;
			EACH_CALLBACK(i, localplayerInit)
			{
				(lua.call(*i), player->getLuaReference())();
			}
			p = player;
		}
		break;

		case AI:
		{
			PlayerAI* player = new PlayerAI(team, worm);
			players.push_back( player );
			p = player;
		}
	}

	if(p)
	{
		EACH_CALLBACK(i, playerInit)
		{
			(lua.call(*i), p->getLuaReference())();
		}
	}

	return p;
}

BaseWorm* Game::addWorm(bool isAuthority)
{
	BaseWorm* returnWorm = NULL;
	Worm* worm = new Worm();
	returnWorm = worm;
	if ( !returnWorm ) allegro_message("moo");
#ifdef USE_GRID
	objects.insertImmediately(returnWorm, Grid::WormColLayer, Grid::WormRenderLayer);
	objects.insertImmediately(returnWorm->getNinjaRopeObj(), 1, 1);
#else
	objects.insert(WORMS_COLLISION_LAYER,WORMS_RENDER_LAYER, returnWorm);
	objects.insert( 1,1, (BaseObject*)returnWorm->getNinjaRopeObj() );
#endif

	return returnWorm;
}

void Game::addBot(int team)
{
	if ( loaded && level.isLoaded() )
	{
		BaseWorm* worm = addWorm(true); 
		BasePlayer* player = addPlayer(AI, team, worm);
		//player->assignWorm(worm);
	}
}

unsigned long Game::stringToIndex(std::string const& str)
{
	HashTable<std::string, unsigned long>::iterator i = stringToIndexMap.find(str);
	if(i != stringToIndexMap.end())
		return i->second;
	i = stringToIndexMap.insert(str);
	int idx = indexToStringMap.size();
	i->second = idx;
	indexToStringMap.push_back(str);
	return idx;
}

std::string const& Game::indexToString(unsigned long idx)
{
	return indexToStringMap.at(idx);
}

std::string const& Game::getModName()
{
	return m_modName;
}

#if 0
void Game::addCRCs(ZCom_BitStream* req)
{
	req->addInt(partTypeList.crc(), 32);
	req->addInt(expTypeList.crc(), 32);
	req->addInt(getWeaponCRC(), 32);
	req->addInt(levelEffectList.crc(), 32);
}

bool Game::checkCRCs(ZCom_BitStream& data)
{
	boost::uint32_t particleCRC = data.getInt(32);
	boost::uint32_t expCRC = data.getInt(32);
	boost::uint32_t weaponCRC = data.getInt(32);
	boost::uint32_t levelEffectCRC = data.getInt(32);
	
	if(particleCRC != partTypeList.crc()
	|| expCRC != expTypeList.crc()
	|| weaponCRC != getWeaponCRC()
	|| levelEffectCRC != levelEffectList.crc())
	{
		return false;
	}
	
	return true;
}
#endif

/*
ZCom_Node* Game::getNode()
{
	return m_node;
}*/
