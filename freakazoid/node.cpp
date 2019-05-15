#include "zoidcom.h"

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
