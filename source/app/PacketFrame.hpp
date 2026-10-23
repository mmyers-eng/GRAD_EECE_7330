#pragma once
#include <cstdint>
#include <vector>

struct PacketHeader
{
	uint8_t magic1; //CE
	uint8_t magic2; //EC
	uint8_t magic3; //DE
	uint8_t magic4; //ED
	uint32_t len; //data len Max is 2048
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

	bool putMagic1(const uint8_t byte);
	bool putMagic2(const uint8_t byte);
	bool putMagic3(const uint8_t byte);
	bool putMagic4(const uint8_t byte);
	bool putLen(const uint32_t lenVal);
	bool putPayloadByte(const uint8_t byte);
	bool putCRC(const uint32_t crc);

private:
	struct PacketHeader mHeader;
	std::vector<uint8_t> mPayload;
	uint32_t mCrc; //hash of the entire packet
};
