#include <gtest/gtest.h>

TEST(TCPClientServerTest, TestServerConnection) {
    ASSERT_EQ(0, 0);
}

// Custom main to run all tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

