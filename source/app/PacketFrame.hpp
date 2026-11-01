#pragma once
#include <cstdint>
#include <vector>

//Packet Definition:
// u8 magic1
// u8 magic2
// u8 magic3
// u8 magic4
// u32 length: length of the payload
// u8* payload the length of of the payload is defined by the length field with a maximum of 1024
// u32 crc: crc of the entire packet NOT including the CRC 

struct PacketHeader
{
	uint8_t magic1; //CE
	uint8_t magic2; //EC
	uint8_t magic3; //DE
	uint8_t magic4; //ED
	uint32_t len; //data len Max is 1024
};

//Defines the packet frame
class PacketFrame
{
public:
	static constexpr uint8_t Magic1{0xCE};
	static constexpr uint8_t Magic2{0xEC};
	static constexpr uint8_t Magic3{0xDE};
	static constexpr uint8_t Magic4{0xED};
	static constexpr uint32_t MaxLen{1024};

	PacketFrame();

	constexpr bool isMagic1Valid(const uint8_t byte){return(Magic1==byte);}
	constexpr bool isMagic2Valid(const uint8_t byte){return(Magic2==byte);}
        constexpr bool isMagic3Valid(const uint8_t byte){return(Magic3==byte);}
	constexpr bool isMagic4Valid(const uint8_t byte){return(Magic4==byte);}
	constexpr bool isLengthValid(const uint32_t len){return(len < MaxLen);}
	bool isPayloadFull(){return(mPayload.size() >= mHeader.len);}

	bool putMagic1(const uint8_t byte);
	bool putMagic2(const uint8_t byte);
	bool putMagic3(const uint8_t byte);
	bool putMagic4(const uint8_t byte);
	bool putLen(const uint32_t lenVal);
	bool putPayloadByte(const uint8_t byte);
	bool putCRC(const uint32_t crc);
        void reset();
	std::vector<uint8_t> getPayload(void){return(mPayload);}
private:
	struct PacketHeader mHeader;
	std::vector<uint8_t> mPayload;
	uint32_t mCrc; //hash of the entire packet
};
