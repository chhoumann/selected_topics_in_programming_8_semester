#include <gtest/gtest.h>
#include "../src/types.cpp"

TEST(MyTestSuite, TestReactionOstreamOverload) {
    // Arrange
    System s = System();

    auto A = s("A", 1);
    auto B = s("B", 1);
    auto C = s("C", 1);

    auto r = s(A + B >>= C, 0.001);

    std::ostringstream ss;

    // Act
    ss << r;

    // Assert
    EXPECT_EQ(ss.str(), "A + B → C (rate: 0.001)");
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
