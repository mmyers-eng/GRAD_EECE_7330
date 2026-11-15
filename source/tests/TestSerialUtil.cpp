#include <gtest/gtest.h>
#include <cstring>
#include <vector>
#include "SerialUtil.hpp" // Adjust path as needed for your project structure

class SerialUtilTest : public ::testing::Test {
protected:
    SerialUtil util;
};

// Helper function to safely convert any float into its raw byte representation
std::vector<uint8_t> floatToBytes(float val) {
    std::vector<uint8_t> bytes(sizeof(float));
    std::memcpy(bytes.data(), &val, sizeof(float));
    return bytes;
}

// 1. Happy Path Test: Standard valid float conversion
TEST_F(SerialUtilTest, ConvertsValidFloatSuccessfully) {
    float expectedValue = 3.14159f;
    auto byteVec = floatToBytes(expectedValue);

    auto result = util.toFloat(byteVec.data(), byteVec.size());

    EXPECT_TRUE(result.first);
    EXPECT_FLOAT_EQ(result.second, expectedValue);
}

// 2. Happy Path Test: Negative float conversion
TEST_F(SerialUtilTest, ConvertsNegativeFloatSuccessfully) {
    float expectedValue = -42.5f;
    auto byteVec = floatToBytes(expectedValue);

    auto result = util.toFloat(byteVec.data(), byteVec.size());

    EXPECT_TRUE(result.first);
    EXPECT_FLOAT_EQ(result.second, expectedValue);
}

// 3. Happy Path Test: Zero and Special IEEE-754 floats
TEST_F(SerialUtilTest, ConvertsZeroFloatSuccessfully) {
    float expectedValue = 0.0f;
    auto byteVec = floatToBytes(expectedValue);

    auto result = util.toFloat(byteVec.data(), byteVec.size());

    EXPECT_TRUE(result.first);
    EXPECT_FLOAT_EQ(result.second, 0.0f);
}

// 4. Boundary/Edge Case Test: Buffer is too small (e.g., 3 bytes instead of 4)
TEST_F(SerialUtilTest, FailsOnTooSmallBuffer) {
    uint8_t smallBuffer[3] = {0x00, 0x00, 0x80};

    auto result = util.toFloat(smallBuffer, sizeof(smallBuffer));

    EXPECT_FALSE(result.first);
    EXPECT_FLOAT_EQ(result.second, 0.0f);
}

// 5. Boundary/Edge Case Test: Empty/Zero-size buffer
TEST_F(SerialUtilTest, FailsOnZeroSizeBuffer) {
    float dummyVal = 1.0f;
    auto byteVec = floatToBytes(dummyVal);

    auto result = util.toFloat(byteVec.data(), 0);

    EXPECT_FALSE(result.first);
    EXPECT_FLOAT_EQ(result.second, 0.0f);
}

// 6. Null Pointer Robustness Test: Passing a nullptr with valid or zero size
TEST_F(SerialUtilTest, FailsGracefullyOnNullptr) {
    auto result = util.toFloat(nullptr, 4);

    EXPECT_FALSE(result.first);
    EXPECT_FLOAT_EQ(result.second, 0.0f);
}

// 7. Over-sized Buffer Test: Passing more than sizeof(float) bytes (e.g., trailing payload bytes)
TEST_F(SerialUtilTest, HandlesOversizedBufferGracefully) {
    float expectedValue = 123.456f;
    auto byteVec = floatToBytes(expectedValue);
    
    // Append extra garbage bytes to simulate a wider serial packet stream
    byteVec.push_back(0xFF);
    byteVec.push_back(0xAA);

    // Depending on your implementation, this should either succeed by reading 
    // the first 4 bytes or reject the size mismatch. 
    // This test ensures it doesn't crash or trigger out-of-bounds reads.
    auto result = util.toFloat(byteVec.data(), byteVec.size());

    // Adjust expectation based on whether exact sizing or minimum sizing is enforced:
    // If your code requires buffSize == sizeof(float), change to EXPECT_FALSE.
    // If your code processes the first sizeof(float) bytes, change to EXPECT_TRUE.
}

// 8. Happy Path Test: Standard float packing into a correctly sized destination buffer
TEST_F(SerialUtilTest, PacksValidFloatSuccessfully) {
    float originalValue = 42.195f;
    uint8_t dstBuff[sizeof(float)] = {0};

    bool success = util.fromFloat(originalValue, dstBuff, sizeof(dstBuff));

    EXPECT_TRUE(success);

    // Verify round-trip conversion back to float using toFloat
    auto roundTrip = util.toFloat(dstBuff, sizeof(dstBuff));
    EXPECT_TRUE(roundTrip.first);
    EXPECT_FLOAT_EQ(roundTrip.second, originalValue);
}

// 9. Happy Path Test: Negative float packing
TEST_F(SerialUtilTest, PacksNegativeFloatSuccessfully) {
    float originalValue = -1024.5f;
    uint8_t dstBuff[sizeof(float)] = {0};

    bool success = util.fromFloat(originalValue, dstBuff, sizeof(dstBuff));

    EXPECT_TRUE(success);

    auto roundTrip = util.toFloat(dstBuff, sizeof(dstBuff));
    EXPECT_TRUE(roundTrip.first);
    EXPECT_FLOAT_EQ(roundTrip.second, originalValue);
}

// 10. Boundary/Edge Case Test: Destination buffer is too small (e.g., 3 bytes for a 4-byte float)
TEST_F(SerialUtilTest, FailsPackingWhenBufferTooSmall) {
    float originalValue = 3.14f;
    uint8_t dstBuff[3] = {0}; // Less than sizeof(float)

    bool success = util.fromFloat(originalValue, dstBuff, sizeof(dstBuff));

    EXPECT_FALSE(success);
}

// 11. Boundary/Edge Case Test: Zero-size destination buffer
TEST_F(SerialUtilTest, FailsPackingOnZeroSizeBuffer) {
    float originalValue = 3.14f;
    uint8_t dstBuff[4] = {0};

    bool success = util.fromFloat(originalValue, dstBuff, 0);

    EXPECT_FALSE(success);
}

// 12. Null Pointer Robustness Test: Passing a nullptr for dstBuff
TEST_F(SerialUtilTest, FailsGracefullyOnNullptrDestination) {
    float originalValue = 1.0f;

    bool success = util.fromFloat(originalValue, nullptr, sizeof(float));

    EXPECT_FALSE(success);
}

// 13. Oversized Buffer Test: Destination buffer is larger than required
TEST_F(SerialUtilTest, HandlesOversizedDestinationBufferSuccessfully) {
    float originalValue = 99.99f;
    // Destination buffer has extra trailing space (e.g., 8 bytes instead of 4)
    uint8_t dstBuff[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    bool success = util.fromFloat(originalValue, dstBuff, sizeof(dstBuff));

    // Validates that it packs successfully into the available space
    EXPECT_TRUE(success); 

    // Verify the first 4 bytes decode back correctly
    auto roundTrip = util.toFloat(dstBuff, sizeof(float));
    EXPECT_TRUE(roundTrip.first);
    EXPECT_FLOAT_EQ(roundTrip.second, originalValue);
}
