#include "zoidcom.h"

#include <string>

ZComUnofficial_ClassSettings::~ZComUnofficial_ClassSettings()
{
	ZoidCom_debugMessage("ZComUnofficial_ClassSettings destructor called");
}

//

ZCom_Control::ZCom_Control()
	: m_clientConnection(NULL)
	, m_serverConnection(NULL)
{
	ZoidCom_debugMessage("ZCom_Control constructor called");
}

ZCom_Control::~ZCom_Control()
{
	ZoidCom_debugMessage("ZCom_Control destructor called");

	for(std::map<ZCom_ClassID, ZComUnofficial_ClassSettings*>::iterator it = classIDMap.begin(); it != classIDMap.end(); it++)
	{
		delete it->second;
		it->second = NULL;
	}
	classIDMap.clear();
	classNameToIDMap.clear();

	ZoidCom_debugMessage("Cleaned up");
}

void ZCom_Control::Shutdown(void)
{
	ZoidCom_debugMessage("ZCom_Control::Shutdown called");
}

void ZCom_Control::ZCom_Connect(ZCom_Address addr, ZCom_BitStream *bitStream)
{
	ZCom_BitStream bs;
	ZoidCom_debugMessage((std::string("ZCom_Connect called with IP ") + addr.getIP()).c_str());

	if ( m_clientConnection )
	{
		delete m_clientConnection;
		m_clientConnection = NULL;
	}

	m_clientConnection = new ZComUnofficial_Connection(this, addr, bitStream);

	//bs.addInt(0, 8); // "Refused"
	//bs.addString("default");
	//bs.addString("poo");
	//ZCom_cbConnectResult(ZCom_Invalid_ID, eZCom_ConnAccepted, bs);

	//ZCom_cbConnectionClosed(ZCom_Invalid_ID, eZCom_ClosedTimeout, bs);
}

void ZCom_Control::ZCom_Disconnect(ZCom_ConnID connID, ZCom_BitStream* bitStream)
{
	ZoidCom_debugMessage((std::string("ZCom_Disconnect called with connID ") + std::to_string(connID)).c_str());
}

void ZCom_Control::ZCom_disconnectAll(ZCom_BitStream* bitStream)
{
	ZoidCom_debugMessage(std::string("ZCom_disconnectAll called").c_str());
}

ZComUnofficial_ConnectionStats ZCom_Control::ZCom_getConnectionStats(ZCom_ConnID connID)
{
	ZoidCom_debugMessage(std::string("ZCom_getConnectionStats called").c_str());
	ZComUnofficial_ConnectionStats result;
	result.avg_ping = 350;
	return result;
}

const ZCom_Address* ZCom_Control::ZCom_getPeer(ZCom_ConnID connID)
{
	ZoidCom_debugMessage((std::string("ZCom_getPeer called with connID ") + std::to_string(connID)).c_str());

	return NULL;
}

ZCom_FileTransInfo ZCom_Node::getFileInfo(ZCom_ConnID connID, ZCom_FileTransID fileTransID)
{
	ZoidCom_debugMessage((std::string("ZCom_getPeer called with connID ") + std::to_string(connID) + std::string(", fileTransID ") + std::to_string(fileTransID)).c_str());
	ZCom_FileTransInfo result;
	result.path = "lol";
	result.bps = 56000.0f;
	result.transferred = 42;
	result.size = 100;
	return result;
}

bool ZCom_Control::ZCom_initSockets(bool unk001, int udpPort, int unk003, int unk004)
{
	ZoidCom_debugMessage((std::string("ZCom_initSockets called with unk001 ") + std::to_string(unk001) + std::string(", udpPort ") + std::to_string(udpPort) + std::string(", unk003 ") + std::to_string(unk003) + std::string(", unk004 ") + std::to_string(unk004)).c_str());

	if ( udpPort == 0 )
	{
		// Client - handle connection in another function
		return true;
	}
	else
	{
		// Server
		if ( m_serverConnection )
		{
			delete m_serverConnection;
			m_serverConnection = NULL;
		}

		m_serverConnection = new ZComUnofficial_Connection(this, udpPort);
		return true;
	}
}

void ZCom_Control::ZCom_processInput(eZComUnofficial_BlockingType blockingType)
{
	// we're assuming non-blocking here as I have no idea what the blocking variant is called here --GM
	//ZoidCom_debugMessage((std::string("ZCom_processInput called")).c_str());
}

void ZCom_Control::ZCom_processOutput(void)
{
	//ZoidCom_debugMessage((std::string("ZCom_processOutput called")).c_str());
}

ZCom_ClassID ZCom_Control::ZCom_registerClass(std::string className, int classFlags)
{
	ZoidCom_debugMessage((std::string("ZCom_registerClass called with className ") + className + std::string(", classFlags ") + std::to_string(classFlags)).c_str());

	// If the name is taken, FIXME: work out what to do
	ZCom_ClassID id = 1;
	ZComUnofficial_ClassSettings* classSettings = NULL;
	if ( classNameToIDMap.find( className ) != classNameToIDMap.end() )
	{
		id = classNameToIDMap.at( className );
		classSettings = classIDMap.at( id );
	}
	else
	{
		id = ZComUnofficial_IDCounter + ZComUnofficial_Base_For_ClassID;
		ZComUnofficial_IDCounter += ZComUnofficial_IDCounter_Step;
		classSettings = new ZComUnofficial_ClassSettings();
		classIDMap.emplace( id, classSettings );
	}

	classSettings->className = className;
	classSettings->classFlags = classFlags;
	classNameToIDMap.emplace(className, id);

	return id;
}

void ZCom_Control::ZCom_requestDownstreamLimit(ZCom_ConnID connID, int bpp, int pps) {}
void ZCom_Control::ZCom_requestZoidMode(ZCom_ConnID connID, int unk002) {}
void ZCom_Control::ZCom_sendData(ZCom_NodeID nodeID, ZCom_BitStream* bitStream, eZCom_SendMode sendMode) {}
void ZCom_Control::ZCom_setControlID(int unk001) {}
void ZCom_Control::ZCom_setDebugName(std::string name) {}
void ZCom_Control::ZCom_setUpstreamLimit(int bpp, int pps) {}
void ZCom_Control::ZCom_simulateLag(int unk001, float lag) {}
void ZCom_Control::ZCom_simulateLoss(int unk001, float loss) {}

