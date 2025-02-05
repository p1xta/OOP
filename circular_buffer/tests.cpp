#include <gtest/gtest.h>
#include "CircularBuffer.h"

TEST(BufferTest, Initialization) {
    CircularBuffer buffer_10(10);
    CircularBuffer empty_buffer(0);
    CircularBuffer full_a_buffer(15, 'a');

    EXPECT_EQ(buffer_10.capacity(), 10);
    EXPECT_EQ(buffer_10.size(), 0);
    EXPECT_EQ(full_a_buffer.size(), 15);
    EXPECT_EQ(empty_buffer.empty(), true);
    EXPECT_EQ(full_a_buffer.full(), true);

    full_a_buffer.clear();
    EXPECT_EQ(full_a_buffer.size(), 0);
}

TEST(BufferTest, PopAndPush) {
    CircularBuffer buffer_10(10);
    CircularBuffer empty_buffer(0);
    CircularBuffer full_a_buffer(15, 'a');
    buffer_10.push_back('a');
    buffer_10.push_back('b');
    buffer_10.push_back('c');
    buffer_10.push_back('d');
    EXPECT_EQ(buffer_10[0], 'a');
    EXPECT_EQ(buffer_10[1], 'b');
    EXPECT_EQ(buffer_10[3], 'd');
    EXPECT_EQ(buffer_10.at(2), 'c');
    EXPECT_THROW(buffer_10.at(13), std::out_of_range);
    EXPECT_EQ(buffer_10.back_index(), 4);
    EXPECT_EQ(buffer_10.front_index(), 0);
    EXPECT_EQ(full_a_buffer[0], 'a');
    
    EXPECT_EQ(buffer_10.size(), 4);
    EXPECT_EQ(full_a_buffer.size(), 15);
    
    full_a_buffer.push_front('x');
    EXPECT_EQ(full_a_buffer.front_index(), 14);
    EXPECT_EQ(full_a_buffer[14], 'x');
    full_a_buffer.pop_back();
    EXPECT_EQ(full_a_buffer.size(), 14);
    full_a_buffer.pop_front();
    EXPECT_EQ(full_a_buffer.size(), 13);
    full_a_buffer.push_back('x');
    EXPECT_EQ(full_a_buffer.back(), 'x'); 
} 

TEST (BufferTest, Linearization) {
    CircularBuffer buffer_10(10);

    buffer_10.push_back('a');
    buffer_10.push_back('b');
    buffer_10.push_back('c');
    buffer_10.push_back('d');
    buffer_10.push_back('e');

    EXPECT_TRUE(buffer_10.is_linearized());
    buffer_10.push_front('f');
    buffer_10.push_front('g');
    EXPECT_FALSE(buffer_10.is_linearized());
    buffer_10.linearize();
    EXPECT_TRUE(buffer_10.is_linearized()); 
}

TEST(BufferTest, PropertyMethods) {
    CircularBuffer buffer_10(10);

    buffer_10.push_back('a');
    buffer_10.push_back('b');
    buffer_10.push_back('c');
    buffer_10.push_back('d');
    buffer_10.push_back('e');
    buffer_10.push_front('f');
    buffer_10.push_front('g');
    buffer_10.push_front('h');
    buffer_10.pop_back();
    buffer_10.pop_front();

    EXPECT_EQ(buffer_10.size(), 6);
    EXPECT_EQ(buffer_10.capacity(), 10);
    EXPECT_EQ(buffer_10.reserve(), 4);
    EXPECT_FALSE(buffer_10.full());
    EXPECT_FALSE(buffer_10.empty());
    buffer_10.set_capacity(15);
    EXPECT_EQ(buffer_10.capacity(), 15);
    EXPECT_EQ(buffer_10.front(), 'g');
    EXPECT_EQ(buffer_10.back(), 'd');
}

TEST(BufferTest, Copying) { 
    CircularBuffer buffer_10(10);

    buffer_10.push_back('a');
    buffer_10.push_back('b');
    buffer_10.push_back('c');
    buffer_10.push_back('d');
    buffer_10.push_back('e');
    buffer_10.push_back('f');
    buffer_10.push_back('g');
    buffer_10.push_back('h');

    CircularBuffer new_buffer(buffer_10);
    EXPECT_TRUE(new_buffer == buffer_10);
    new_buffer.clear();
    new_buffer = buffer_10;
    EXPECT_TRUE(new_buffer == buffer_10);
}

TEST(BufferTest, Rotation) {
    CircularBuffer buffer_10(10);

    buffer_10.push_back('a');
    buffer_10.push_back('b');
    buffer_10.push_back('c');
    buffer_10.push_back('d');
    buffer_10.push_back('e');
    buffer_10.push_front('f');
    buffer_10.push_front('g');
    buffer_10.push_front('h');

    buffer_10.rotate(4);
    EXPECT_TRUE(buffer_10[4] == 'h');
    EXPECT_TRUE(buffer_10[5] == 'g');
    EXPECT_TRUE(buffer_10[6] == 'f');
    EXPECT_TRUE(buffer_10[7] == 'a');
    EXPECT_TRUE(buffer_10[8] == 'b');
    EXPECT_TRUE(buffer_10[9] == 'c');
    EXPECT_TRUE(buffer_10[0] == 'd');
    EXPECT_TRUE(buffer_10[1] == 'e');

}

TEST(BufferTest, Insertion) {
    CircularBuffer buffer_10(10);

    buffer_10.push_back('a');
    buffer_10.push_back('b');
    buffer_10.push_back('c');
    buffer_10.push_back('d');
    buffer_10.push_back('e');
    buffer_10.push_front('f');
    buffer_10.push_front('g');
    buffer_10.push_front('h');

    buffer_10.insert(3, 'x');
    EXPECT_TRUE(buffer_10.at(3) == 'x');
    
    EXPECT_TRUE(buffer_10[0] == 'a');
    EXPECT_TRUE(buffer_10[1] == 'b');
    EXPECT_TRUE(buffer_10[2] == 'c');
    EXPECT_TRUE(buffer_10[3] == 'x');
    EXPECT_TRUE(buffer_10[4] == 'd');
    EXPECT_TRUE(buffer_10[5] == 'e');
    EXPECT_TRUE(buffer_10[7] == 'h');
    EXPECT_TRUE(buffer_10[8] == 'g');
    EXPECT_TRUE(buffer_10[9] == 'f');
    EXPECT_EQ(buffer_10.reserve(), 1);
    buffer_10.push_back('i');
    EXPECT_TRUE(buffer_10.full());
    EXPECT_THROW(buffer_10.insert(5), std::overflow_error);
}

TEST(BufferTest, Erase) {
    CircularBuffer buffer_10(10);

    buffer_10.push_back('a');
    buffer_10.push_back('b');
    buffer_10.push_back('c');
    buffer_10.push_back('d');
    buffer_10.push_back('e');
    buffer_10.push_front('f');
    buffer_10.push_front('g');
    buffer_10.push_front('h');
    buffer_10.set_capacity(15);
    EXPECT_EQ(buffer_10.capacity(), 15);
    buffer_10.resize(10, 'x');
    EXPECT_EQ(buffer_10.at(5), 'x');
    buffer_10.erase(1, 4); // removes b, c, d
    EXPECT_EQ(buffer_10[12], 'h');
    EXPECT_EQ(buffer_10[13], 'g');
    EXPECT_EQ(buffer_10[14], 'f');
    EXPECT_EQ(buffer_10[0], 'a');
    EXPECT_EQ(buffer_10[1], 'e');
    EXPECT_EQ(buffer_10[2], 'x');
    EXPECT_EQ(buffer_10[3], 'x'); 
}

TEST(BufferTest, Swaping) {
    CircularBuffer buffer_1(5);

    buffer_1.push_back('a');
    buffer_1.push_back('b');
    buffer_1.push_back('c');
    buffer_1.push_front('f');

    CircularBuffer buffer_2(5);

    buffer_2.push_back('y');
    buffer_2.push_back('z');
    buffer_2.push_front('x');

    buffer_1.swap(buffer_2);

    EXPECT_EQ(buffer_1[4], 'x');
    EXPECT_EQ(buffer_1[0], 'y');
    EXPECT_EQ(buffer_1[1], 'z');

    EXPECT_EQ(buffer_2[0], 'a');
    EXPECT_EQ(buffer_2[1], 'b');
    EXPECT_EQ(buffer_2[2], 'c');
    EXPECT_EQ(buffer_2[4], 'f');
}

TEST(BufferTest, Operators) {
    CircularBuffer buffer_1(10);
    buffer_1.push_back('a');
    buffer_1.push_back('b');
    buffer_1.push_back('c');
    buffer_1.push_front('f');
    buffer_1.push_front('g');
    buffer_1.push_front('h');

    CircularBuffer buffer_2(10);
    buffer_2.push_back('a');
    buffer_2.push_back('b');
    buffer_2.push_back('c');
    buffer_2.push_front('f');
    buffer_2.push_front('g');

    EXPECT_TRUE(buffer_1 != buffer_2);
    EXPECT_FALSE(buffer_1 == buffer_2);

    buffer_2.push_front('h');

    EXPECT_TRUE(buffer_1 == buffer_2);
    EXPECT_FALSE(buffer_1 != buffer_2);
}

/* TEST (BufferTest, Destruction) {
    {
        CircularBuffer full_a_buffer(15, 'a');
        EXPECT_EQ(full_a_buffer.size(), 15);
        EXPECT_EQ(full_a_buffer.full(), true);
    }
} */