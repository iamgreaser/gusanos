#include "posspd_replicator.h"

#include <zoidcom.h>
#include <iostream>
#include "util/vec.h"
#include <utility>
using std::cerr;
using std::endl;

PosSpdReplicator::PosSpdReplicator(ZCom_ReplicatorSetup *_setup,
	Vec *pos, Vec *spd)
: ZCom_ReplicatorBasic(_setup)
, m_posPtr(pos)
, m_spdPtr(spd)
, m_repCount(0)
{
	m_flags |= ZCOM_REPLICATOR_INITIALIZED;
}

bool PosSpdReplicator::checkState()
{
	bool s = (( m_posCmp.x != m_posPtr->x ) || ( m_posCmp.y != m_posPtr->y));
	m_posCmp = *m_posPtr;
	return s;
}

void PosSpdReplicator::packData(ZCom_BitStream *_stream)
{
	_stream->addFloat(m_posPtr->x,32);
	_stream->addFloat(m_posPtr->y,32);

	if ( m_repCount >= speedRepTime )
	{
		m_repCount = 0;
		_stream->addBool(true);
		_stream->addFloat(m_spdPtr->x,speedPrec);
		_stream->addFloat(m_spdPtr->y,speedPrec);
	}else
	{
		++m_repCount;
		_stream->addBool(false);
	}

}

void PosSpdReplicator::unpackData(ZCom_BitStream *_stream, bool _store, zU32 _estimated_time_sent)
{
	if (_store)
	{
		m_posPtr->x = _stream->getFloat(32);
		m_posPtr->y = _stream->getFloat(32);

		if ( _stream->getBool() )
		{
			m_spdPtr->x = _stream->getFloat(speedPrec);
			m_spdPtr->y = _stream->getFloat(speedPrec);
		}

	}
	else 
	{
		_stream->getFloat(32);
		_stream->getFloat(32);

		if ( _stream->getBool() )
		{
			_stream->getFloat(speedPrec);
			_stream->getFloat(speedPrec);
			// For some reason this stores anyway?!
			//*m_spdPtr = diffEncoding.decode<Vec>(*_stream);
		}
	}
}

void* PosSpdReplicator::peekData()
{
	assert(getPeekStream());
	Vec *retVec = new Vec;
	retVec->x = getPeekStream()->getFloat(32);
	retVec->y = getPeekStream()->getFloat(32);
	peekDataStore(retVec);
	
	return (void*) retVec;
}

void PosSpdReplicator::clearPeekData()
{
	Vec *buf = (Vec*) peekDataRetrieve();
	delete buf;
};

/*Vec VectorReplicator::getLastUpdate()
{
	return m_posCmp;
}*/
