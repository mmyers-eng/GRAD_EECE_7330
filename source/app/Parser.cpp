#include <Parser.hpp>
#include <cstdio>
Parser::Parser()
{
	reset();
}

void Parser::parseByte(const uint8_t byte)
{
	switch(state)
	{
	case ParserState::MAGIC1:
		if(pkt.putMagic1(byte))
		{
			state = ParserState::MAGIC2;
		}
		break;
	case ParserState::MAGIC2:
		if(pkt.putMagic2(byte))
		{
			state = ParserState::MAGIC3;
		}
		else
		{
			reset();
		}
		break;
	case ParserState::MAGIC3:
		if(pkt.putMagic3(byte))
		{
			state = ParserState::MAGIC4;
		}
		else
		{
			reset();
		}
		break;
	case ParserState::MAGIC4:
		if(pkt.putMagic4(byte))
		{
			state = ParserState::LEN;
		}
		else
		{
			reset();
		}
		break;
	case ParserState::LEN:
		len |= static_cast<uint32_t>(byte) << (lenCount * 8);
		lenCount++;
		if(lenCount > 3)
		{
			if(pkt.putLen(len))
			{
				state = ParserState::PAYLOAD;
			}
			else
			{
				//length invalid
				reset();
			}
		}
		break;
	case ParserState::PAYLOAD:
		if(pkt.putPayloadByte(byte))
		{
			if(pkt.isPayloadFull())
			{
				state = ParserState::CRC;
			}
		}
		else
		{
			reset();
		}
		break;
	case ParserState::CRC:
		crc |= static_cast<uint32_t>(byte) << (crcCount * 8);
		crcCount++;
		if(crcCount > 3)
		{
			if(pkt.putCRC(crc))
			{
				pktBuffer.push(pkt);
				reset();
			}
			else
			{
				//CRC failed
				reset();
			}
		}
		break;
	default:
		break;
	}
}

bool Parser::getPacket(PacketFrame& packet)
{
	return(pktBuffer.pop(packet));
}

void Parser::reset()
{
	state = ParserState::MAGIC1;
	lenCount = 0;
	crcCount = 0;
	len = 0;
	crc = 0;
	pkt.reset();
}
