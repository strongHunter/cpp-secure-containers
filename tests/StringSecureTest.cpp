#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "SecureContainers.h"

using testing::_;

template <typename T>
class SanitizingAllocatorChild : public SanitizingAllocator<T> {
public:
    using SanitizingAllocator<T>::SanitizingAllocator;
};


TEST(StringSecureTest, DefaultAllocatorShouldBe_SanitizingAllocator)
{
    string_secure str;
    EXPECT_TRUE((std::is_same<typename decltype(str)::allocator_type, SanitizingAllocator<char>>::value));
}

TEST(StringSecureTest, CanDefineVectorWithDerivedSanitizingAllocator)
{
    basic_string_secure<char, SanitizingAllocatorChild<char>> str;
    EXPECT_TRUE((std::is_base_of<SanitizingAllocator<char>, decltype(str)::allocator_type>::value));
}

TEST(StringSecureTest, ChildAllocator_NotEqual_SanitizingAllocator)
{
    basic_string_secure<char, SanitizingAllocatorChild<char>> str;
    EXPECT_FALSE((std::is_same<typename decltype(str)::allocator_type, SanitizingAllocator<char>>::value));
}
