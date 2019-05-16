#ifndef VECTOR_REPLICATOR_H
#define VECTOR_REPLICATOR_H

#include <zoidcom.h>

#include "util/vec.h"
#include <utility>
#include <stdexcept>

class VectorReplicator : public ZCom_ReplicatorBasic
{
private:
	
	Vec*	m_ptr;
	Vec	m_cmp;

public:

	VectorReplicator(ZCom_ReplicatorSetup *_setup, Vec *_data);
	
	// TODO: Implement this for safeness sake
	ZCom_Replicator* Duplicate(ZCom_Replicator *_dest)
	{
		if(_dest)
			*_dest = *this;
		else
			return new VectorReplicator(*this);
		return 0;
	}
	
	bool checkState();
	
	bool checkInitialState() { return true; }
	
	void packData(ZCom_BitStream *_stream);
	
	void unpackData(ZCom_BitStream *_stream, bool _store, zU32 _estimated_time_sent);
	
	void Process(eZCom_NodeRole _localrole, zU32 _simulation_time_passed) {}
	
	void* peekData();
	
	void clearPeekData();
	
	//const Vec& getLastUpdate();
	
private:
};

#endif
