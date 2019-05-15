#include "zoidcom.h"

#include <cassert>
#include <cstdlib>

//
// If we ensure that everything is byte-aligned,
// we can simplify the implementation.
//
// We're also not going to care about bits because we can use variable types.
//

ZCom_BitStream::ZCom_BitStream()
{

}

ZCom_BitStream::~ZCom_BitStream()
{

}

//
// Here's our variable encoding!
//
// For unsigned ints:
// 0aaaaaaa - a
// 1aaaaaaa 0bbbbbbb - ba
// 1aaaaaaa 1bbbbbbb 0ccccccc - cba
// 1aaaaaaa 1bbbbbbb 1ccccccc 0ddddddd - dcba
// 1aaaaaaa 1bbbbbbb 1ccccccc 1ddddddd 0000eeee - edcba
//
// For signed ints we rotate left in two's complement
// and then do a one's complement inversion. Or something like that.
// Basically, the bottom bit has the sign.

// First things first, the most fundamental type we'll be dealing with.
void ZCom_BitStream::addInt(zU32 data, int bits)
{
	// Truncate the data to the given bit range.
	data &= (1<<bits)-1;

	// Encode according to bit packing.
	while((data & ~0x7F) != 0)
	{
		m_buffer.push_back((data & 0x7F)|0x80);
		data >>= 7;
	}
	assert(0 <= data && data <= 0x7F);
	m_buffer.push_back(data);
}

// Next up, let's deal to these derived types!
void ZCom_BitStream::addBool(bool data)
{
	addInt(data ? 1 : 0, 1);
}

void ZCom_BitStream::addSignedInt(zS32 data, int bits)
{
	if ( data < 0 )
	{
		addInt((((zU32)(-1-data))<<1)|1, bits);
	}
	else
	{
		addInt((((zU32)data)<<1)|0, bits);
	}
}

// Floats are probably going to be a nuisance to pack here but oh well
void ZCom_BitStream::addFloat(zFloat data, int fracbits)
{
	// We're ignoring fracbits for now.
	zS32 intData = *reinterpret_cast<zS32 *>(&data);

	if ( intData < 0 )
	{
		intData ^= 0x7FFFFFFF;
	}

	addSignedInt(intData, 32);
}

// Now we have some text!
void ZCom_BitStream::addString(std::string data)
{
	addInt((zU32)data.length(), 32);
	for(std::size_t i = 0; i < data.length(); i++)
	{
		addInt(data[i], 8);
	}
}

void ZCom_BitStream::addBitStream(ZCom_BitStream* bitStream)
{
	// Concatenate it all
	m_buffer.insert(m_buffer.end(), bitStream->m_buffer.begin(), bitStream->m_buffer.end());
}

ZCom_BitStream* ZCom_BitStream::Duplicate(void)
{
	ZCom_BitStream* result = new ZCom_BitStream();
	result->addBitStream(this);

	return result;
}

zU32 ZCom_BitStream::getInt(int bits)
{
	// Decode according to bit packing.
	zU32 data = 0;
	int leftShift = 0;
	assert(m_bufferReadPointer+1 <= m_buffer.size());
	while((m_buffer[m_bufferReadPointer] & 0x80) != 0)
	{
		m_buffer.push_back((data & 0x7F)|0x80);
		assert(leftShift < 28);
		data |= (m_buffer[m_bufferReadPointer] & 0x7F) << leftShift;
		leftShift += 7;
		m_bufferReadPointer++;
		assert(m_bufferReadPointer+1 <= m_buffer.size());
	}
	data |= (m_buffer[m_bufferReadPointer] & 0x7F) << leftShift;
	m_bufferReadPointer++;

	// Truncate the data to the given bit range.
	data &= (1<<bits)-1;

	return data;
}

bool ZCom_BitStream::getBool(void)
{
	zU32 intData = getInt(1);

	assert(0 <= intData && intData <= 1);

	return ( intData != 0 );
}

zS32 ZCom_BitStream::getSignedInt(int bits)
{
	zU32 uintData = getInt(bits);

	if ( (uintData & 1) != 0 )
	{
		return (-1)-(zS32)(uintData >> 1);
	}
	else
	{
		return (zS32)(uintData >> 1);
	}
}

zFloat ZCom_BitStream::getFloat(int fracbits)
{
	zS32 intData = getInt(32);

	return *reinterpret_cast<zFloat *>(&intData);
}

// TODO: Add a bunch of const char*s I can use which get thrown out in the destructor
const char *ZCom_BitStream::getStringStatic(void) { return ""; }
