#include "zoidcom.h"

using std::size_t;

using std::string;
using std::to_string;

ZCom_Node::ZCom_Node(void)
	: m_nodeRole(eZCom_RoleUndefined)
{
	m_nodeID = ZComUnofficial_IDCounter + ZComUnofficial_Base_For_NodeID;
	ZComUnofficial_IDCounter += ZComUnofficial_IDCounter_Step;
}

ZCom_Node::ZCom_Node(ZCom_NodeID nodeID, eZCom_NodeRole nodeRole)
	: m_nodeID(nodeID)
	, m_nodeRole(nodeRole)
{
}

ZCom_Node::~ZCom_Node()
{
}

void ZCom_Node::acceptFile(ZCom_ConnID connID, ZCom_FileTransID fileTransID, int unk003, bool actuallyAccept)
{
	ZoidCom_debugMessage(("Node " + to_string((size_t)(void *)this) + ": Accept File").c_str());
}

void ZCom_Node::addReplicationFloat(zFloat* data, int unk002, zU32 flags, zU8 rules)
{
	ZoidCom_debugMessage(("Node " + to_string((size_t)(void *)this) + ": Add ReplicationFloat: " + to_string(*data) + ", unk002=" + to_string(unk002) + ", flags=" + to_string(flags) + ", rules=" + to_string(rules)).c_str());
}

void ZCom_Node::addReplicationInt(zS32* data, int bits, bool unk003, zU32 flags, zU8 rules)
{
	ZoidCom_debugMessage(("Node " + to_string((size_t)(void *)this) + ": Add ReplicationInt: " + to_string(*data) + ", bits=" + to_string(bits) + ", unk003=" + to_string(unk003) + ", flags=" + to_string(flags) + ", rules=" + to_string(rules)).c_str());
}

void ZCom_Node::addReplicationInt(zS32* data, int bits, bool unk003, zU32 flags, zU8 rules, zU32 unk006)
{
	ZoidCom_debugMessage(("Node " + to_string((size_t)(void *)this) + ": Add ReplicationInt: " + to_string(*data) + ", bits=" + to_string(bits) + ", unk003=" + to_string(unk003) + ", flags=" + to_string(flags) + ", rules=" + to_string(rules) + ", unk006=" + to_string(unk006)).c_str());
}

void ZCom_Node::addReplicator(ZCom_Replicator* replicator, bool unk002)
{
	ZoidCom_debugMessage(("Node " + to_string((size_t)(void *)this) + ": Add Replicator: " + to_string((size_t)(void *)replicator) + ", unk002=" + to_string(unk002)).c_str());
}

void ZCom_Node::applyForZoidLevel(int level)
{
	ZoidCom_debugMessage(("Node " + to_string((size_t)(void *)this) + ": Apply for Zoid level " + to_string(level)).c_str());
}

void ZCom_Node::beginReplicationSetup(int unk001)
{
	ZoidCom_debugMessage(("Node " + to_string((size_t)(void *)this) + ": Begin replication setup " + to_string(unk001)).c_str());
}

bool ZCom_Node::checkEventWaiting(void)
{
	// Unsurprisingly this is pretty spammy. --GM
	//ZoidCom_debugMessage(("Node " + to_string((size_t)(void *)this) + ": checkEventWaiting").c_str());
	return false;
}

void ZCom_Node::endReplicationSetup(void)
{
	ZoidCom_debugMessage(("Node " + to_string((size_t)(void *)this) + ": End replication setup").c_str());
}

ZCom_NodeID ZCom_Node::getNetworkID(void)
{
	ZoidCom_debugMessage(("Node " + to_string((size_t)(void *)this) + ": Get Network ID").c_str());
	return m_nodeID;
}

ZCom_BitStream* ZCom_Node::getNextEvent(eZCom_Event* event, eZCom_NodeRole* remoteRole, ZCom_ConnID* connID)
{
	ZoidCom_debugMessage(("Node " + to_string((size_t)(void *)this) + ": checkEventWaiting").c_str());
	return NULL;
}

eZCom_NodeRole ZCom_Node::getRole(void)
{
	ZoidCom_debugMessage(("Node " + to_string((size_t)(void *)this) + ": Get Role").c_str());
	return m_nodeRole;
}

bool ZCom_Node::registerNodeDynamic(ZCom_ClassID classID, ZCom_Control* zControl)
{
	ZoidCom_debugMessage(("Node " + to_string((size_t)(void *)this) + ": Register dynamic").c_str());
	return true;
}

bool ZCom_Node::registerNodeUnique(ZCom_ClassID classID, eZCom_NodeRole nodeRole, ZCom_Control* zControl)
{
	ZoidCom_debugMessage(("Node " + to_string((size_t)(void *)this) + ": Register unique, role=" + to_string(nodeRole)).c_str());
	return true;
}

bool ZCom_Node::registerRequestedNode(ZCom_ClassID classID, ZCom_Control* zControl)
{
	ZoidCom_debugMessage(("Node " + to_string((size_t)(void *)this) + ": Register requested").c_str());
	return true;
}

void ZCom_Node::removeFromZoidLevel(int level)
{
	ZoidCom_debugMessage(("Node " + to_string((size_t)(void *)this) + ": Remove from Zoid level " + to_string(level)).c_str());
}

void ZCom_Node::sendEvent(eZCom_SendMode sendMode, zU8 rules, ZCom_BitStream* data)
{
	ZoidCom_debugMessage(("Node " + to_string((size_t)(void *)this) + ": sendEvent: sendMode=" + to_string((size_t)(void *)sendMode) + ", rules=" + to_string(rules)).c_str());
}

void ZCom_Node::sendEventDirect(eZCom_SendMode sendMode, ZCom_BitStream* data, ZCom_ConnID connID)
{
	ZoidCom_debugMessage(("Node " + to_string((size_t)(void *)this) + ": sendEventDirect: sendMode=" + to_string((size_t)(void *)sendMode) + ", connID=" + to_string(connID)).c_str());
}

ZCom_FileTransID ZCom_Node::sendFile(const char* fileName, int unk002, ZCom_ConnID connID, int unk004, float unk005)
{
	ZoidCom_debugMessage(("Node " + to_string((size_t)(void *)this) + ": sendFile: \"" + string(fileName) + "\"").c_str());
	return ZCom_Invalid_ID;
}

void ZCom_Node::setAnnounceData(ZCom_BitStream* bitStream)
{
	ZoidCom_debugMessage(("Node " + to_string((size_t)(void *)this) + ": setAnnounceData").c_str());
}

void ZCom_Node::setEventNotification(bool unk001, bool unk002)
{
	ZoidCom_debugMessage(("Node " + to_string((size_t)(void *)this) + ": setEventNotification unk001=" + to_string(unk001) + ", unk002=" + to_string(unk002)).c_str());
}

void ZCom_Node::setInterceptID(ZCom_InterceptID interceptID)
{
	ZoidCom_debugMessage(("Node " + to_string((size_t)(void *)this) + ": setInterceptID " + to_string(interceptID)).c_str());
}

void ZCom_Node::setOwner(ZCom_ConnID connID, bool unk002)
{
	ZoidCom_debugMessage(("Node " + to_string((size_t)(void *)this) + ": setOwner " + to_string(connID) + " " + to_string(unk002)).c_str());
}

void ZCom_Node::setReplicationInterceptor(ZCom_NodeReplicationInterceptor* nodeReplicationInterceptor)
{
	ZoidCom_debugMessage(("Node " + to_string((size_t)(void *)this) + ": setReplicationInterceptor").c_str());
}
