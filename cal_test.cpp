#include "gmock/gmock.h"
#include "Cal.cpp"

TEST(group, Tc1)
{
    Cal cal;
    int result = cal.getSum(10, 20);
    EXPECT_EQ(30, result);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
