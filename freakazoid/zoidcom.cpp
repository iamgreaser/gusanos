// THIS FILE WILL DEFINITELY GET SPLIT UP AT SOME POINT.
#include <cassert>
#include <iostream>
#include <string>

#include "zoidcom.h"

void dumb_logger(char const* msg)
{
	std::cerr << "FREAKAZOID: " << msg << std::endl;
}

void (*ZoidCom_fn_logger)(char const* msg) = dumb_logger;

void ZoidCom_debugMessage(char const* msg)
{
	ZoidCom_fn_logger(msg);
}

// FIXME: ZCom_Address stubs
ZComUnofficial_IP ZCom_Address::getIP(void) const
{
	return m_IP;
}

void ZCom_Address::setAddress(eZComUnofficial_AddressType addressType, int unk002, std::string addressBody)
{
	assert(addressType == eZCom_AddressUDP);
	assert(unk002 == 0);
	ZoidCom_debugMessage((std::string("ZCom_Address::setAddress() called with address \"") + addressBody + std::string("\"")).c_str());
	m_IP = addressBody;
}


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
ZoidCom::ZoidCom()
{
	std::cerr << "freakazoid called w/o debug logger callback" << std::endl;
}

ZoidCom::ZoidCom(void (*fn_logger)(char const* msg))
{
	std::cerr << "freakazoid called with a debug logger callback" << std::endl;
	ZoidCom_fn_logger = fn_logger;
}

ZoidCom::~ZoidCom()
{
	ZoidCom_debugMessage("ZoidCom destructor called");
}

bool ZoidCom::Init(void)
{
	ZoidCom_debugMessage("Init called");

	return true;
}

void ZoidCom::setLogLevel(int level)
{
	ZoidCom_debugMessage((std::string("Set log level to ") + std::to_string(level)).c_str());
}
