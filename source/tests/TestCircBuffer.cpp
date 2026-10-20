#include <gtest/gtest.h>
#include "CircBuffer.hpp" // Adjust filename/path if necessary

class CircBufferTest : public ::testing::Test {
protected:
    // Small capacity of 3 makes wrap-around and full conditions easy to trigger
    CircBuffer<int, 3> buffer;
};

// Initial State
TEST_F(CircBufferTest, InitialStateIsEmpty) {
    int val = 0;
    EXPECT_FALSE(buffer.pop(val));
}

// Basic Push and Pop
TEST_F(CircBufferTest, SinglePushAndPop) {
    EXPECT_TRUE(buffer.push(42));
    
    int val = 0;
    EXPECT_TRUE(buffer.pop(val));
    EXPECT_EQ(val, 42);
    
    // Buffer should be empty again
    EXPECT_FALSE(buffer.pop(val));
}

// FIFO Ordering
TEST_F(CircBufferTest, FIFOOrdering) {
    EXPECT_TRUE(buffer.push(10));
    EXPECT_TRUE(buffer.push(20));
    EXPECT_TRUE(buffer.push(30));

    int val = 0;
    EXPECT_TRUE(buffer.pop(val));
    EXPECT_EQ(val, 10);

    EXPECT_TRUE(buffer.pop(val));
    EXPECT_EQ(val, 20);

    EXPECT_TRUE(buffer.pop(val));
    EXPECT_EQ(val, 30);
}

// Capacity Enforcement (Full Buffer)
TEST_F(CircBufferTest, EnforcesFullCapacity) {
    EXPECT_TRUE(buffer.push(1));
    EXPECT_TRUE(buffer.push(2));
    EXPECT_TRUE(buffer.push(3));
    
    // 4th push should fail because capacity is 3
    EXPECT_FALSE(buffer.push(4));
}

// Wrap-Around Logic
TEST_F(CircBufferTest, HandlesWrapAroundCorrectly) {
    // Fill the buffer
    buffer.push(1);
    buffer.push(2);
    buffer.push(3);
    
    // Pop two items to advance the read index
    int val = 0;
    buffer.pop(val); // Removes 1
    buffer.pop(val); // Removes 2
    
    // Push two more items to force wrap-around past index N-1
    EXPECT_TRUE(buffer.push(4));
    EXPECT_TRUE(buffer.push(5));
    
    // Verify FIFO order across the wrap boundary
    EXPECT_TRUE(buffer.pop(val));
    EXPECT_EQ(val, 3);
    
    EXPECT_TRUE(buffer.pop(val));
    EXPECT_EQ(val, 4);
    
    EXPECT_TRUE(buffer.pop(val));
    EXPECT_EQ(val, 5);
    
    // Should be empty now
    EXPECT_FALSE(buffer.pop(val));
}

// qReset Behavior
TEST_F(CircBufferTest, ResetClearsBufferState) {
    buffer.push(100);
    buffer.push(200);
    
    buffer.reset();
    
    int val = 0;
    EXPECT_FALSE(buffer.pop(val));
    
    // Verify we can push normally after a reset
    EXPECT_TRUE(buffer.push(999));
    EXPECT_TRUE(buffer.pop(val));
    EXPECT_EQ(val, 999);
}

// Verify peek inspects the item without removing it
TEST_F(CircBufferTest, PeekInspectsWithoutRemoving) {
    int val = 0;

    // Empty buffer peek should fail
    EXPECT_FALSE(buffer.peek(val));

    // Push an item
    EXPECT_TRUE(buffer.push(42));

    // Peek should succeed and show the item
    EXPECT_TRUE(buffer.peek(val));
    EXPECT_EQ(val, 42);

    // Peek *again* to prove the item is still in the buffer
    int val2 = 0;
    EXPECT_TRUE(buffer.peek(val2));
    EXPECT_EQ(val2, 42);

    // Now pop it and verify it finally leaves the buffer
    EXPECT_TRUE(buffer.pop(val));
    EXPECT_EQ(val, 42);

    // Buffer should now be empty
    EXPECT_FALSE(buffer.peek(val));
}
