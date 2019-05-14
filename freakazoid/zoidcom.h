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
typedef float zFloat;

typedef int32_t ZCom_ClassID;
typedef int32_t ZCom_ConnID;
typedef int32_t ZCom_FileTransID;
typedef int32_t ZCom_InterceptID;
typedef int32_t ZCom_NodeID;

#define ZCOM_FTRANS_ID_BITS 32

constexpr int32_t ZCom_Invalid_ID = 0;

class ZCom_BitStream;
class ZCom_Node;
class ZCom_NodeReplicationInterceptor;
class ZCom_Replicator;
class ZCom_ReplicatorSetup;

class ZComUnofficial_ConnectionStats;
typedef std::string ZComUnofficial_IP; // What, you think we're going to lock this down to IPv4? --GM

#define ZCOM_CLASSFLAG_ANNOUNCEDATA 0x00000001

#define ZCOM_REPLICATOR_INITIALIZED 0x00000001

#define ZCOM_REPFLAG_MOSTRECENT 0x00000001
#define ZCOM_REPFLAG_RARELYCHANGED 0x00000002
#define ZCOM_REPFLAG_INTERCEPT 0x00000004

#define ZCOM_REPRULE_AUTH_2_ALL 0x01
#define ZCOM_REPRULE_AUTH_2_PROXY 0x02
#define ZCOM_REPRULE_OWNER_2_AUTH 0x04
#define ZCOM_REPRULE_AUTH_2_OWNER 0x08
#define ZCOM_REPRULE_NONE 0x10

typedef enum eZCom_CloseReason_enum
{
	eZCom_ClosedDisconnect,
	eZCom_ClosedReconnect,
	eZCom_ClosedTimeout,
} eZCom_CloseReason;

typedef enum eZCom_ConnectResult_enum
{
	eZCom_ConnAccepted,
	eZCom_ConnNopeThatDidntWork, // not an actual name, but we need something just in case --GM
} eZCom_ConnectResult;

typedef enum eZCom_Event_enum
{
	eZCom_EventInit,
	eZCom_EventRemoved,
	eZCom_EventUser,
	eZCom_EventFile_Incoming,
	eZCom_EventFile_Data,
	eZCom_EventFile_Complete,
} eZCom_Event;

typedef enum eZCom_NodeRole_enum
{
	eZCom_RoleUndefined = 0,

	eZCom_RoleAuthority,
	eZCom_RoleProxy,
	eZCom_RoleOwner,

} eZCom_NodeRole;

typedef enum eZCom_SendMode_enum
{
	eZCom_Unreliable,
	eZCom_ReliableUnordered,
	eZCom_ReliableOrdered,
} eZCom_SendMode;

typedef enum eZCom_ZoidResult_enum
{
	eZCom_ZoidEnabled,
	eZCom_ZoidNopeThatDidntWork, // not an actual name, but we need something just in case --GM
} eZCom_ZoidResult;

typedef enum eZComUnofficial_AddressType_enum
{
	eZCom_AddressLocal,
	eZCom_AddressUDP,
} eZComUnofficial_AddressType;

typedef enum eZComUnofficial_BlockingType_enum
{
	eZCom_NoBlock,
} eZComUnofficial_BlockingType;

class ZCom_Address
{
public:
	ZComUnofficial_IP getIP(void) const;
	void setAddress(eZComUnofficial_AddressType addressType, int unk002, std::string addressBody);
};

class ZCom_BitStream
{
public:
	void addBitStream(ZCom_BitStream* bitStream);
	void addBool(bool data);
	void addFloat(zFloat data, int fracbits);
	void addInt(zU32 data, int bits);
	void addSignedInt(zS32 data, int bits);
	void addString(std::string data);
	ZCom_BitStream* Duplicate(void);
	bool getBool(void);
	zFloat getFloat(int fracbits);
	zU32 getInt(int bits);
	zS32 getSignedInt(int bits);
	//std::string getStringStatic(void);
	const char *getStringStatic(void);
};

class ZCom_Control
{
public:
	void Shutdown(void);
	void ZCom_Connect(ZCom_Address addr, ZCom_BitStream *bitStream);
	void ZCom_Disconnect(ZCom_ConnID connID, ZCom_BitStream* bitStream);
	void ZCom_disconnectAll(ZCom_BitStream* bitStream);
	ZComUnofficial_ConnectionStats ZCom_getConnectionStats(ZCom_ConnID connID);
	const ZCom_Address* ZCom_getPeer(ZCom_ConnID connID);
	bool ZCom_initSockets(bool unk001, int udpPort, int unk003, int unk004);
	void ZCom_processInput(eZComUnofficial_BlockingType blockingType);
	void ZCom_processOutput(void);
	ZCom_ClassID ZCom_registerClass(std::string className, int classFlags);
	void ZCom_requestDownstreamLimit(ZCom_ConnID connID, int bpp, int pps);
	void ZCom_requestZoidMode(ZCom_ConnID connID, int unk002);
	void ZCom_sendData(ZCom_NodeID nodeID, ZCom_BitStream* bitStream, eZCom_SendMode sendMode);
	void ZCom_setControlID(int unk001);
	void ZCom_setDebugName(std::string name);
	void ZCom_setUpstreamLimit(int bpp, int pps);
	void ZCom_simulateLag(int unk001, float lag);
	void ZCom_simulateLoss(int unk001, float loss); // For the record, this game is older than Loss. --GM
};

class ZCom_FileTransInfo
{
public:
	std::string path;
	float bps;
	size_t transferred;
	size_t size;
};

class ZCom_Node
{
public:
	void acceptFile(ZCom_ConnID connID, ZCom_FileTransID fileTransID, int unk003, bool actuallyAccept);
	void addReplicationFloat(zFloat* data, int unk002, zU32 flags, zU8 rules);
	void addReplicationInt(zS32* data, int bits, bool unk003, zU32 flags, zU8 rules);
	void addReplicationInt(zS32* data, int bits, bool unk003, zU32 flags, zU8 rules, zU32 unk006);
	void addReplicator(ZCom_Replicator* replicator, bool unk002);
	void applyForZoidLevel(int level);
	void beginReplicationSetup(int unk001);
	bool checkEventWaiting(void);
	void endReplicationSetup(void);
	ZCom_FileTransInfo getFileInfo(ZCom_ConnID connID, ZCom_FileTransID fileTransID);
	ZCom_NodeID getNetworkID(void);
	ZCom_BitStream* getNextEvent(eZCom_Event* event, eZCom_NodeRole* remoteRole, ZCom_ConnID* connID);
	eZCom_NodeRole getRole(void);
	bool registerNodeDynamic(ZCom_ClassID classID, ZCom_Control* zControl);
	bool registerNodeUnique(ZCom_ClassID classID, eZCom_NodeRole nodeRole, ZCom_Control* zControl);
	bool registerRequestedNode(ZCom_ClassID classID, ZCom_Control* zControl);
	void removeFromZoidLevel(int level);
	void sendEvent(eZCom_SendMode sendMode, zU8 rules, ZCom_BitStream* data);
	void sendEventDirect(eZCom_SendMode sendMode, ZCom_BitStream* data, ZCom_ConnID connID);
	ZCom_FileTransID sendFile(const char* fileName, int unk002, ZCom_ConnID connID, int unk004, float unk005);
	void setAnnounceData(ZCom_BitStream* bitStream);
	void setEventNotification(bool unk001, bool unk002);
	void setInterceptID(ZCom_InterceptID interceptID);
	void setOwner(float lol, bool unk002);
	void setReplicationInterceptor(ZCom_NodeReplicationInterceptor* nodeReplicationInterceptor);

};

class ZCom_NodeReplicationInterceptor
{

};

class ZCom_Replicator
{
public:
	ZCom_ReplicatorSetup* getSetup(void);
	void* peekData(void);
	void peekDataStore(void* data);

};

class ZCom_ReplicatorBasic : public ZCom_Replicator
{
public:
	ZCom_ReplicatorBasic(ZCom_ReplicatorSetup* setup);

	int m_flags = 0;
	ZCom_BitStream* getPeekStream(void);
	void* peekDataRetrieve(void);
};

class ZCom_ReplicatorSetup
{
public:
	ZCom_ReplicatorSetup(zU32 flags, zU8 rules);
	ZCom_ReplicatorSetup(zU32 flags, zU8 rules, int someType, int unk004, int probablyATimeoutInMilliseconds);

	ZCom_InterceptID getInterceptID(void);

};

class ZComUnofficial_ConnectionStats
{
public:
	int avg_ping;
};

class ZoidCom
{
public:
	ZoidCom();
	ZoidCom(void (*fn_logger)(char const* msg));

	bool Init(void);
	void setLogLevel(int level);
private:
};

#endif /* ifdef _FREAKAZOID_ZOIDCOM_H */
