// THIS FILE WILL DEFINITELY GET SPLIT UP AT SOME POINT.
#include "zoidcom.h"


// FIXME: ZCom_Address stubs
ZComUnofficial_IP ZCom_Address::getIP(void) const { return "::1"; }
void ZCom_Address::setAddress(eZComUnofficial_AddressType addressType, int unk002, std::string addressBody) {}


// FIXME: ZCom_BitStream stubs
void ZCom_BitStream::addBitStream(ZCom_BitStream* bitStream) {}
void ZCom_BitStream::addBool(bool data) {}
void ZCom_BitStream::addFloat(zFloat data, int fracbits) {}
void ZCom_BitStream::addInt(zU32 data, int bits) {}
void ZCom_BitStream::addSignedInt(zS32 data, int bits) {}
void ZCom_BitStream::addString(std::string data) {}
ZCom_BitStream* ZCom_BitStream::Duplicate(void) { return this; }
bool ZCom_BitStream::getBool(void) { return false; }
zFloat ZCom_BitStream::getFloat(int fracbits) { return 0.0f; }
zU32 ZCom_BitStream::getInt(int bits) { return 0; }
zS32 ZCom_BitStream::getSignedInt(int bits) { return 0; }
const char *ZCom_BitStream::getStringStatic(void) { return ""; }


// FIXME: ZCom_Control stubs
void ZCom_Control::Shutdown(void) {}
void ZCom_Control::ZCom_Connect(ZCom_Address addr, ZCom_BitStream *bitStream) {}
void ZCom_Control::ZCom_Disconnect(ZCom_ConnID connID, ZCom_BitStream* bitStream) {}
void ZCom_Control::ZCom_disconnectAll(ZCom_BitStream* bitStream) {}

ZComUnofficial_ConnectionStats ZCom_Control::ZCom_getConnectionStats(ZCom_ConnID connID)
{
	ZComUnofficial_ConnectionStats result;
	result.avg_ping = 350;
	return result;
}

const ZCom_Address* ZCom_Control::ZCom_getPeer(ZCom_ConnID connID) { return NULL; }

ZCom_FileTransInfo ZCom_Node::getFileInfo(ZCom_ConnID connID, ZCom_FileTransID fileTransID)
{
	ZCom_FileTransInfo result;
	result.path = "lol";
	result.bps = 56000.0f;
	result.transferred = 42;
	result.size = 100;
	return result;
}

bool ZCom_Control::ZCom_initSockets(bool unk001, int udpPort, int unk003, int unk004) { return true; }
void ZCom_Control::ZCom_processInput(eZComUnofficial_BlockingType blockingType) {}
void ZCom_Control::ZCom_processOutput(void) {}
ZCom_ClassID ZCom_Control::ZCom_registerClass(std::string className, int classFlags) { return ZCom_Invalid_ID; }
void ZCom_Control::ZCom_requestDownstreamLimit(ZCom_ConnID connID, int bpp, int pps) {}
void ZCom_Control::ZCom_requestZoidMode(ZCom_ConnID connID, int unk002) {}
void ZCom_Control::ZCom_sendData(ZCom_NodeID nodeID, ZCom_BitStream* bitStream, eZCom_SendMode sendMode) {}
void ZCom_Control::ZCom_setControlID(int unk001) {}
void ZCom_Control::ZCom_setDebugName(std::string name) {}
void ZCom_Control::ZCom_setUpstreamLimit(int bpp, int pps) {}
void ZCom_Control::ZCom_simulateLag(int unk001, float lag) {}
void ZCom_Control::ZCom_simulateLoss(int unk001, float loss) {}


// FIXME: ZCom_Node stubs
void ZCom_Node::acceptFile(ZCom_ConnID connID, ZCom_FileTransID fileTransID, int unk003, bool actuallyAccept) {}
void ZCom_Node::addReplicationFloat(zFloat* data, int unk002, zU32 flags, zU8 rules) {}
void ZCom_Node::addReplicationInt(zS32* data, int bits, bool unk003, zU32 flags, zU8 rules) {}
void ZCom_Node::addReplicationInt(zS32* data, int bits, bool unk003, zU32 flags, zU8 rules, zU32 unk006) {}
void ZCom_Node::addReplicator(ZCom_Replicator* replicator, bool unk002) {}
void ZCom_Node::applyForZoidLevel(int level) {}
void ZCom_Node::beginReplicationSetup(int unk001) {}
bool ZCom_Node::checkEventWaiting(void) { return false; }
void ZCom_Node::endReplicationSetup(void) {}
ZCom_NodeID ZCom_Node::getNetworkID(void) { return ZCom_Invalid_ID; }
ZCom_BitStream* ZCom_Node::getNextEvent(eZCom_Event* event, eZCom_NodeRole* remoteRole, ZCom_ConnID* connID) { return NULL; }
eZCom_NodeRole ZCom_Node::getRole(void) { return eZCom_RoleUndefined; }
bool ZCom_Node::registerNodeDynamic(ZCom_ClassID classID, ZCom_Control* zControl) { return true; }
bool ZCom_Node::registerNodeUnique(ZCom_ClassID classID, eZCom_NodeRole nodeRole, ZCom_Control* zControl) { return true; }
bool ZCom_Node::registerRequestedNode(ZCom_ClassID classID, ZCom_Control* zControl) { return true; }
void ZCom_Node::removeFromZoidLevel(int level) {}
void ZCom_Node::sendEvent(eZCom_SendMode sendMode, zU8 rules, ZCom_BitStream* data) {}
void ZCom_Node::sendEventDirect(eZCom_SendMode sendMode, ZCom_BitStream* data, ZCom_ConnID connID) {}
ZCom_FileTransID ZCom_Node::sendFile(const char* fileName, int unk002, ZCom_ConnID connID, int unk004, float unk005) { return ZCom_Invalid_ID; }
void ZCom_Node::setAnnounceData(ZCom_BitStream* bitStream) {}
void ZCom_Node::setEventNotification(bool unk001, bool unk002) {}
void ZCom_Node::setInterceptID(ZCom_InterceptID interceptID) {}
void ZCom_Node::setOwner(float lol, bool unk002) {}
void ZCom_Node::setReplicationInterceptor(ZCom_NodeReplicationInterceptor* nodeReplicationInterceptor) {}


// FIXME: ZCom_Replicator stubs
ZCom_ReplicatorSetup* ZCom_Replicator::getSetup(void) { return NULL; }
void* ZCom_Replicator::peekData(void) { return NULL; }
void ZCom_Replicator::peekDataStore(void *data) {}


// FIXME: ZCom_ReplicatorBasic stubs
ZCom_ReplicatorBasic::ZCom_ReplicatorBasic(ZCom_ReplicatorSetup* setup) {}
ZCom_BitStream* ZCom_ReplicatorBasic::getPeekStream(void) { return NULL; }
void* ZCom_ReplicatorBasic::peekDataRetrieve(void) { return NULL; }


// FIXME: ZCom_ReplicatorSetup stubs
ZCom_ReplicatorSetup::ZCom_ReplicatorSetup(zU32 flags, zU8 rules) {}
ZCom_ReplicatorSetup::ZCom_ReplicatorSetup(zU32 flags, zU8 rules, int someType, int unk004, int probablyATimeoutInMilliseconds) {}
ZCom_InterceptID ZCom_ReplicatorSetup::getInterceptID(void) { return ZCom_Invalid_ID; }


// FIXME: ZoidCom stubs
ZoidCom::ZoidCom() {}
ZoidCom::ZoidCom(void (*fn_logger)(char const* msg)) {}
bool ZoidCom::Init(void) { return true; }
void ZoidCom::setLogLevel(int level) {}
