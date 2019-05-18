/*

Freakazoid: Free Software Zoidcom replacement
Because the original library no longer exists.

*/
#ifndef _FREAKAZOID_ZOIDCOM_H
#define _FREAKAZOID_ZOIDCOM_H
#include <cstdint>
#include <map>
#include <string>
#include <vector>

typedef uint8_t zU8;
typedef int32_t zS32;
typedef uint32_t zU32;
typedef float zFloat;

typedef int32_t ZCom_ClassID;
typedef int32_t ZCom_ConnID;
typedef int32_t ZCom_FileTransID;
typedef int32_t ZCom_InterceptID;
typedef int32_t ZCom_NodeID;

// INTERNAL
constexpr int ZComUnofficial_Base_For_ClassID = 0x0;
constexpr int ZComUnofficial_Base_For_ConnID = 0x1;
constexpr int ZComUnofficial_Base_For_FileTransID = 0x2;
constexpr int ZComUnofficial_Base_For_InterceptID = 0x3;
constexpr int ZComUnofficial_Base_For_NodeID = 0x4;
constexpr int ZComUnofficial_IDCounter_Step = 0x10;
extern int ZComUnofficial_IDCounter;

// not a public API, but one added specifically for Freakazoid
void ZoidCom_debugMessage(char const* msg);

#define ZCOM_FTRANS_ID_BITS 32

constexpr int32_t ZCom_Invalid_ID = 0;

class ZCom_BitStream;
class ZCom_Control;
class ZCom_Node;
class ZCom_NodeReplicationInterceptor;
class ZCom_Replicator;
class ZCom_ReplicatorSetup;

class ZComUnofficial_ConnectionStats;
class ZComUnofficial_Connection;
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
	eZCom_ClosedDisconnect = 0,
	eZCom_ClosedReconnect,
	eZCom_ClosedTimeout,
} eZCom_CloseReason;

typedef enum eZCom_ConnectResult_enum
{
	eZCom_ConnAccepted = 0,
	eZCom_ConnNopeThatDidntWork, // not an actual name, but we need something just in case --GM
} eZCom_ConnectResult;

typedef enum eZCom_Event_enum
{
	eZCom_EventInit = 0,
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
	eZCom_Unreliable = 0,
	eZCom_ReliableUnordered,
	eZCom_ReliableOrdered,
} eZCom_SendMode;

typedef enum eZCom_ZoidResult_enum
{
	eZCom_ZoidEnabled = 0,
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

protected:
	std::string m_IP;
};

class ZCom_BitStream
{
	friend class ZComUnofficial_Connection;
public:
	ZCom_BitStream();
	~ZCom_BitStream();
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

private:
	std::vector<char *> m_staticStrings;
	std::vector<uint8_t> m_buffer;
	std::size_t m_bufferReadPointer;
};

class ZComUnofficial_ClassSettings
{
public:
	~ZComUnofficial_ClassSettings();
	std::string className;
	int classFlags;
};

typedef enum eZComUnofficial_PacketType_enum
{
	eZCom_PacketInformalDisconnect = 0x01,
	eZCom_PacketPart = 0x02,
	eZCom_PacketJoinRequest = 0x03,
	eZCom_PacketJoinAccept = 0x04,
} eZComUnofficial_PacketType;

class ZComUnofficial_Connection
{
public:
	enum ConnState {
		ConnDead = 0,
		ConnDyingNoAddress,
		ConnHosting,
		ConnPreConnecting,
		ConnConnecting,
		ConnJoined,
	};

	ZComUnofficial_Connection(ZCom_Control* control, int udpPort);
	ZComUnofficial_Connection(ZCom_Control* control, ZCom_Address addr, ZCom_BitStream* joinBitStream);
	~ZComUnofficial_Connection();
	void think(void);

protected:
	void sendClientPacket(const char *data, size_t len);
	void sendClientPacket(const char *data, size_t len, void* other_sockaddr, size_t other_sockaddr_len);
	size_t recvClientPacket(char *data, size_t len, void* other_sockaddr, unsigned int* other_sockaddr_len);

private:
	ZCom_Address m_addr;
	std::string m_addrIP;
	std::string m_addrPortStr;
	ZCom_BitStream* m_joinBitStream;
	int m_sockfd;

	void *m_client_sockaddr;
	size_t m_client_sockaddr_len;
	ZCom_Control* m_control;
	ConnState m_state;
};

class ZCom_Control
{
public:
	ZCom_Control();
	~ZCom_Control();
	void Shutdown(void);
	void ZCom_Connect(ZCom_Address addr, ZCom_BitStream* bitStream);
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

	virtual void ZCom_cbConnectionClosed( ZCom_ConnID _id, eZCom_CloseReason _reason, ZCom_BitStream &_reasondata ) {}
	virtual bool ZCom_cbConnectionRequest( ZCom_ConnID  _id, ZCom_BitStream &_request, ZCom_BitStream &_reply ){return false;}
	virtual void ZCom_cbConnectionSpawned( ZCom_ConnID _id ) {}
	virtual void ZCom_cbConnectResult( ZCom_ConnID _id, eZCom_ConnectResult _result, ZCom_BitStream &_reply ) {}
	virtual void ZCom_cbDataReceived( ZCom_ConnID id, ZCom_BitStream &data) {}
	virtual void ZCom_cbDiscovered( const ZCom_Address & _addr, ZCom_BitStream &_reply )  {}
	virtual bool ZCom_cbDiscoverRequest( const ZCom_Address &_addr, ZCom_BitStream &_request, ZCom_BitStream &_reply ) {return false;}
	virtual void ZCom_cbNodeRequest_Dynamic( ZCom_ConnID _id, ZCom_ClassID _requested_class, ZCom_BitStream *_announcedata, eZCom_NodeRole _role, ZCom_NodeID _net_id ) {}
	virtual void ZCom_cbNodeRequest_Tag( ZCom_ConnID _id, ZCom_ClassID _requested_class, ZCom_BitStream *_announcedata, eZCom_NodeRole _role, zU32 _tag ) {}
	virtual bool ZCom_cbZoidRequest( ZCom_ConnID _id, zU8 _requested_level, ZCom_BitStream &_reason ) {return false;}
	virtual void ZCom_cbZoidResult(ZCom_ConnID _id, eZCom_ZoidResult _result, zU8 _new_level, ZCom_BitStream &_reason) {}

protected:
	std::map<ZCom_ClassID, ZComUnofficial_ClassSettings*> classIDMap;
	std::map<std::string, ZCom_ClassID> classNameToIDMap;
	ZComUnofficial_Connection* m_clientConnection;
	ZComUnofficial_Connection* m_serverConnection;
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
	ZCom_Node(void);
	ZCom_Node(ZCom_NodeID nodeID, eZCom_NodeRole nodeRole);
	~ZCom_Node();
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
	void setOwner(ZCom_ConnID connID, bool unk002);
	void setReplicationInterceptor(ZCom_NodeReplicationInterceptor* nodeReplicationInterceptor);
private:
	ZCom_NodeID m_nodeID;
	eZCom_NodeRole m_nodeRole;
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
	~ZoidCom();

	bool Init(void);
	void setLogLevel(int level);
};

#endif /* ifdef _FREAKAZOID_ZOIDCOM_H */
