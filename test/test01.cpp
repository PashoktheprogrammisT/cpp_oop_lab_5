#include <gtest/gtest.h>
#include "../include/stack.hpp"
#include "../include/fixed_vector_memory_resource.hpp"

TEST(StackTest, IntStackOperations) {
    fixed_vector_memory_resource pool(1024);
    Stack<int> stack(&pool);
    
    EXPECT_TRUE(stack.empty());
    EXPECT_EQ(stack.size(), 0);
    
    stack.push(10);
    EXPECT_FALSE(stack.empty());
    EXPECT_EQ(stack.size(), 1);
    EXPECT_EQ(stack.top(), 10);
    
    stack.push(20);
    stack.push(30);
    EXPECT_EQ(stack.size(), 3);
    EXPECT_EQ(stack.top(), 30);
    
    stack.pop();
    EXPECT_EQ(stack.size(), 2);
    EXPECT_EQ(stack.top(), 20);
}

TEST(StackTest, ComplexTypeOperations) {
    fixed_vector_memory_resource pool(2048);
    
    struct Point {
        int x, y;
        Point(int x = 0, int y = 0) : x(x), y(y) {}
    };
    
    Stack<Point> stack(&pool);
    
    stack.push(Point(1, 2));
    stack.push(Point(3, 4));
    
    EXPECT_EQ(stack.top().x, 3);
    EXPECT_EQ(stack.top().y, 4);
    
    stack.pop();
    EXPECT_EQ(stack.top().x, 1);
    EXPECT_EQ(stack.top().y, 2);
}

TEST(StackTest, IteratorTest) {
    fixed_vector_memory_resource pool(1024);
    Stack<int> stack(&pool);
    
    for (int i = 1; i <= 5; i++) stack.push(i);
    
    int expected = 5;
    for (auto it = stack.begin(); it != stack.end(); ++it) {
        EXPECT_EQ(*it, expected);
        expected--;
    }
}

TEST(StackTest, MoveTest) {
    fixed_vector_memory_resource pool(1024);
    Stack<int> stack1(&pool);
    
    stack1.push(100);
    stack1.push(200);
    stack1.push(300);
    
    EXPECT_EQ(stack1.size(), 3);
    
    Stack<int> stack2(std::move(stack1));
    
    EXPECT_EQ(stack1.size(), 0);
    EXPECT_EQ(stack2.size(), 3);
    EXPECT_EQ(stack2.top(), 300);
}

TEST(StackTest, ClearTest) {
    fixed_vector_memory_resource pool(1024);
    Stack<int> stack(&pool);
    
    for (int i = 0; i < 10; i++) stack.push(i);
    
    EXPECT_EQ(stack.size(), 10);
    stack.clear();
    EXPECT_TRUE(stack.empty());
    EXPECT_EQ(stack.size(), 0);
}

TEST(StackTest, RangeBasedForTest) {
    fixed_vector_memory_resource pool(1024);
    Stack<int> stack(&pool);
    
    for (int i = 1; i <= 3; i++) stack.push(i * 10);
    
    std::vector<int> expected = {30, 20, 10};
    std::vector<int> actual;
    
    for (const auto& item : stack) actual.push_back(item);
    
    EXPECT_EQ(actual, expected);
}

TEST(MemoryResourceTest, MemoryReuse) {
    fixed_vector_memory_resource pool(512);
    
    {
        Stack<int> stack1(&pool);
        stack1.push(1);
        stack1.push(2);
        stack1.push(3);
    }
    
    {
        Stack<int> stack2(&pool);
        stack2.push(10);
        stack2.push(20);
        EXPECT_EQ(stack2.top(), 20);
    }
}

TEST(MemoryResourceTest, AllocationException) {
    fixed_vector_memory_resource pool(64);
    
    Stack<int> stack(&pool);
    
    stack.push(1);
    
    try {
        for (int i = 0; i < 100; i++) stack.push(i);
    } catch (const std::bad_alloc&) {
        SUCCEED();
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
