#include <cstdint>

typedef uint32_t zU32;

#define ZCOM_REPLICATOR_INITIALIZED 0x00000001

class ZCom_BitStream;

class ZCom_BitStream
{
public:
	void addInt(int data, int bits);
	void getInt(int bits);
};

class eZCom_NodeRole
{

};

class ZCom_Replicator
{

};

class ZCom_ReplicatorBasic
{
public:
	int m_flags = 0;
	ZCom_BitStream* getPeekStream();
	void* peekDataRetrieve();
};

class ZCom_ReplicatorSetup
{

};
