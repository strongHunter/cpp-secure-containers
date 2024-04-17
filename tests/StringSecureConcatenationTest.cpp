#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "SecureContainers.h"

using testing::_;

TEST(StringSecureConcatenationTest, StringSecure_Plus_ConstChar)
{
    string_secure s1 = "123";
    string_secure s = s1 + "456";

    EXPECT_EQ(s, "123456");
}
