#include "zoidcom.h"

ZComUnofficial_Connection::ZComUnofficial_Connection(ZCom_Address addr, ZCom_BitStream* joinBitStream)
{
	m_addr = addr;
	m_joinBitStream = joinBitStream->Duplicate();
}

ZComUnofficial_Connection::~ZComUnofficial_Connection()
{
	if ( m_joinBitStream )
	{
		delete m_joinBitStream;
	}
}
