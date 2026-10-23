#include "PacketFrame.hpp"


PacketFrame::PacketFrame():mHeader{},
			   mCrc{0}
{
	mPayload.reserve(MaxLen);
					   
}
bool PacketFrame::putMagic1(const uint8_t byte)
{
	bool rtn{false};
	if(isMagic1Valid(byte))
	{
		mHeader.magic1 = byte;
		rtn = true;
	}
	return(rtn);
}

bool PacketFrame::putMagic2(const uint8_t byte)
{
	bool rtn{false};
	if(isMagic2Valid(byte))
	{
		mHeader.magic2 = byte;
		rtn = true;
	}
	return(rtn);

}

bool PacketFrame::putMagic3(const uint8_t byte)
{
	bool rtn{false};
	if(isMagic3Valid(byte))
	{
		mHeader.magic3 = byte;
		rtn = true;
	}
	return(rtn);
}

bool PacketFrame::putMagic4(const uint8_t byte)
{
	bool rtn{false};
	if(isMagic4Valid(byte))
	{
		mHeader.magic4 = byte;
		rtn = true;
	}
	return(rtn);
}

bool PacketFrame::putLen(const uint32_t lenVal)
{
	bool rtn{false};
	if(isLengthValid(lenVal))
	{
		mHeader.len = lenVal;
		rtn = true;
	}
	return(rtn);
}

bool PacketFrame::putPayloadByte(const uint8_t byte)
{
	bool rtn{false};
	if(mPayload.size() < mHeader.len)
	{
		mPayload.push_back(byte);
		rtn = true;
	}
	return(rtn);
}

bool PacketFrame::putCRC(const uint32_t crc)
{
	bool rtn{true};
	mCrc = crc;
	//always true for now
	return(rtn);

}
