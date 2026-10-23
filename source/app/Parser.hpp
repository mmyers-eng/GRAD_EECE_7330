#pragma once
//Interface class for a packet parsing system
#include <cstdint>
#include <PacketFrame.hpp>
#include <CircBuffer.hpp>
class Parser
{
public:
	enum class ParserState
	{
		MAGIC1,
		MAGIC2,
		MAGIC3,
		MAGIC4,
		LEN,
		PAYLOAD,
		CRC
	};
	/**
	 * @brief Parser
	 * 
	 * @details constructor
	 *
	 * @contract calls the reset function
	 */	
	Parser();	
	/**
	 * @brief parseByte
	 * 
	 * @details parses an input byte
	 * 
	 * @param[in] byte the byte to parse 
	 */
	void parseByte(const uint8_t byte);
	/**
	 * @brief getPacket
	 * 
	 * @details gets a finished packet from the circular buffer
	 * 
	 * @param[out] packet 
	 * @return true only if a packet was obtained. 
	 */
	bool getPacket(PacketFrame& packet);
private:
	/**
	 * @brief reset
	 * 
	 * @details resets the parser to the original parse state
	 * @return void 
	 */
	void reset(void);

	ParserState state;
	CircBuffer<PacketFrame, 3> pktBuffer;
	PacketFrame pkt;
	uint32_t lenCount;
	uint32_t payloadCount;
	uint32_t len;
};
