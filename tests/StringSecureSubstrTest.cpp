#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "SecureContainers.h"

using testing::_;


class StringSecureSubstrTest : public testing::Test {
protected:
    void SetUp()
    {
        str_ = string_secure("Hello, world!");
    }

    string_secure str_;
};

TEST_F(StringSecureSubstrTest, SubstrShouldNotChangeOriginalObject)
{
    str_.substr();
    EXPECT_EQ(str_, "Hello, world!");
}

TEST_F(StringSecureSubstrTest, SubstringShouldBe_string_secure)
{
    auto substring = str_.substr();
    EXPECT_TRUE((std::is_same_v<decltype(substring), string_secure>));
}

TEST_F(StringSecureSubstrTest, SubstringShouldCreateNewObject)
{
    void* ptr = str_.data();
    auto substring = str_.substr();

    EXPECT_NE(ptr, substring.data());
}

TEST_F(StringSecureSubstrTest, SubstrCorrectness_1)
{
    auto substring = str_.substr(7);
    EXPECT_EQ(substring, "world!");
}

TEST_F(StringSecureSubstrTest, SubstrCorrectness_2)
{
    auto substring = str_.substr(0, str_.length() - 1);
    EXPECT_EQ(substring, "Hello, world");
}

TEST_F(StringSecureSubstrTest, SubstrCorrectness_3)
{
    auto substring = str_.substr(1, str_.length() - 2);
    EXPECT_EQ(substring, "ello, world");
}
