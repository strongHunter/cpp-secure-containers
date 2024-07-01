#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "basic_string_secure.h"

using testing::_;


TEST(StringSecureConcatenationTest, StringSecure_Plus_ConstChar)
{
    string_secure s1 = "123";
    string_secure s = s1 + "456";

    EXPECT_EQ(s, "123456");
}

TEST(StringSecureConcatenationTest, StringSecure_Plus_Char)
{
    string_secure s1 = "123";
    string_secure s = s1 + '4';

    EXPECT_EQ(s, "1234");
}

TEST(StringSecureConcatenationTest, StringSecure_Plus_lvalue_StringSecure)
{
    string_secure s1 = "123";
    string_secure s2 = "456";
    string_secure s = s1 + s2;

    EXPECT_EQ(s, "123456");
}

TEST(StringSecureConcatenationTest, StringSecure_Plus_rvalue_StringSecure)
{
    string_secure s1 = "123";
    string_secure s = s1 + string_secure("456");

    EXPECT_EQ(s, "123456");
}

TEST(StringSecureConcatenationTest, rvalue_StringSecure_Plus_lvalue_StringSecure)
{
    string_secure s2 = "456";
    string_secure s = string_secure("123") + s2;

    EXPECT_EQ(s, "123456");
}

TEST(StringSecureConcatenationTest, rvalue_StringSecure_Plus_rvalue_StringSecure)
{
    string_secure s = string_secure("123") + string_secure("456");

    EXPECT_EQ(s, "123456");
}

TEST(StringSecureConcatenationTest, rvalue_StringSecure_Plus_ConstChar)
{
    string_secure s = string_secure("123") + "456";

    EXPECT_EQ(s, "123456");
}

TEST(StringSecureConcatenationTest, rvalue_StringSecure_Plus_Char)
{
    string_secure s = string_secure("123") + '4';

    EXPECT_EQ(s, "1234");
}

TEST(StringSecureConcatenationTest, ConstChar_Plus_lvalue_StringSecure)
{
    string_secure s2 = "456";
    string_secure s = "123" + s2;

    EXPECT_EQ(s, "123456");
}

TEST(StringSecureConcatenationTest, ConstChar_Plus_rvalue_StringSecure)
{
    string_secure s = "123" + string_secure("456");

    EXPECT_EQ(s, "123456");
}

TEST(StringSecureConcatenationTest, Char_Plus_lvalue_StringSecure)
{
    string_secure s2 = "234";
    string_secure s = '1' + s2;

    EXPECT_EQ(s, "1234");
}

TEST(StringSecureConcatenationTest, Char_Plus_rvalue_StringSecure)
{
    string_secure s = '1' + string_secure("234");

    EXPECT_EQ(s, "1234");
}
