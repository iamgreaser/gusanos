/*

Freakazoid: Free Software Zoidcom replacement
Because the original library no longer exists.

*/
#ifndef _FREAKAZOID_ZOIDCOM_H
#define _FREAKAZOID_ZOIDCOM_H
#include <cstdint>
#include <string>

typedef uint8_t zU8;
typedef int32_t zS32;
typedef uint32_t zU32;

typedef int32_t ZCom_ClassID;
typedef int32_t ZCom_ConnID;
typedef int32_t ZCom_InterceptID;
typedef int32_t ZCom_NodeID;

constexpr int32_t ZCom_Invalid_ID = -1;

class ZCom_BitStream;
class ZCom_Node;
class ZCom_NodeReplicationInterceptor;
class ZCom_Replicator;
class ZCom_ReplicatorSetup;

#define ZCOM_REPLICATOR_INITIALIZED 0x00000001

#define ZCOM_REPFLAG_MOSTRECENT 0x00000001
#define ZCOM_REPFLAG_RARELYCHANGED 0x00000002
#define ZCOM_REPFLAG_INTERCEPT 0x00000004

#define ZCOM_REPRULE_AUTH_2_ALL 0x01

typedef enum eZCom_Event_enum
{
	eZCom_EventInit,
	eZCom_EventRemoved,
	eZCom_EventUser,
} eZCom_Event;

typedef enum eZCom_NodeRole_enum
{
	eZCom_RoleUndefined = 0,

	eZCom_RoleAuthority,
	eZCom_RoleProxy,

} eZCom_NodeRole;

typedef enum eZCom_SendMode_enum
{
	eZCom_Unreliable,
	eZCom_ReliableUnordered,
	eZCom_ReliableOrdered,
} eZCom_SendMode;

class ZCom_BitStream
{
public:
	void addBitStream(ZCom_BitStream* bitStream);
	void addInt(int data, int bits);
	void addString(std::string data);
	int getInt(int bits);
};

class ZCom_Control
{
public:
	void ZCom_sendData(ZCom_NodeID nodeID, ZCom_BitStream* bitStream, eZCom_SendMode sendMode);
};

class ZCom_Node
{
public:
	void addReplicationInt(zS32* data, int bits, bool unk001, zU32 flags, zU8 rules);
	void addReplicator(ZCom_Replicator* replicator, bool unk001);
	void applyForZoidLevel(int level);
	void beginReplicationSetup(int unk001);
	bool checkEventWaiting(void);
	void endReplicationSetup(void);
	ZCom_BitStream* getNextEvent(eZCom_Event* event, eZCom_NodeRole* remoteRole, ZCom_ConnID* connID);
	eZCom_NodeRole getRole(void);
	bool registerNodeDynamic(ZCom_ClassID classID, ZCom_Control* zControl);
	bool registerNodeUnique(ZCom_ClassID classID, eZCom_NodeRole nodeRole, ZCom_Control* zControl);
	bool registerRequestedNode(ZCom_ClassID classID, ZCom_Control* zControl);
	void sendEvent(eZCom_SendMode sendMode, zU8 rules, ZCom_BitStream* data);
	void sendEventDirect(eZCom_SendMode sendMode, ZCom_BitStream* data, ZCom_ConnID connID);
	void setAnnounceData(ZCom_BitStream* bitStream);
	void setEventNotification(bool unk001, bool unk002);
	void setReplicationInterceptor(ZCom_NodeReplicationInterceptor* nodeReplicationInterceptor);

};

class ZCom_NodeReplicationInterceptor
{

};

class ZCom_Replicator
{
public:
	ZCom_ReplicatorSetup* getSetup(void);

};

class ZCom_ReplicatorBasic : public ZCom_Replicator
{
public:
	int m_flags = 0;
	ZCom_BitStream* getPeekStream(void);
	void* peekDataRetrieve(void);
};

class ZCom_ReplicatorSetup
{
public:
	ZCom_ReplicatorSetup(zU32 flags, zU8 rules, int someType, int unk004, int probablyATimeoutInMilliseconds);
	ZCom_InterceptID getInterceptID(void);

};
#endif /* ifdef _FREAKAZOID_ZOIDCOM_H */
