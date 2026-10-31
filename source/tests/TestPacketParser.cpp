#include <gtest/gtest.h>
#include "Parser.hpp"
#include "PacketFrame.hpp"

// Test fixture for Parser tests
class ParserTest : public ::testing::Test {
protected:
    Parser parser;
};

// Test that feeding the correct magic header sequence transitions the parser successfully
TEST_F(ParserTest, ParsesValidMagicHeaderSequence) {
    // Magic bytes: 0xCE, 0xEC, 0xDE, 0xED
    parser.parseByte(0xCE);
    parser.parseByte(0xEC);
    parser.parseByte(0xDE);
    parser.parseByte(0xED);

    // Feed a length of 0 (4 bytes for uint32_t len, assuming little-endian or target byte order)
    // Adjust depending on how your putLen / len parsing expects bytes
    parser.parseByte(0x01);
    parser.parseByte(0x00);
    parser.parseByte(0x00);
    parser.parseByte(0x00);

    //0xaa is the payload
    parser.parseByte(0xAA);

    // Feed a 4-byte CRC placeholder if length is 0 and payload is empty
    parser.parseByte(0xAA);
    parser.parseByte(0xBB);
    parser.parseByte(0xCC);
    parser.parseByte(0xDD);

    PacketFrame extractedPacket;
    bool success = parser.getPacket(extractedPacket);
    
    EXPECT_TRUE(success);
}

// Test that invalid magic bytes cause the parser to reset or reject the stream gracefully
TEST_F(ParserTest, RejectsInvalidMagicByte) {
    // Incorrect initial magic byte
    parser.parseByte(0xFF);
    parser.parseByte(0xEC);
    parser.parseByte(0xDE);
    parser.parseByte(0xED);

    PacketFrame extractedPacket;
    EXPECT_FALSE(parser.getPacket(extractedPacket));
}

// Test empty buffer behavior
TEST_F(ParserTest, ReturnsFalseWhenNoPacketAvailable) {
    PacketFrame extractedPacket;
    EXPECT_FALSE(parser.getPacket(extractedPacket));
}
