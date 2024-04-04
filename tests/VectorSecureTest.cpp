#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "SecureContainers.h"

using Type = uint32_t;

template <typename T>
class SanitizingAllocatorChild : public SanitizingAllocator<T> {
public:
    using SanitizingAllocator<T>::SanitizingAllocator;
};


TEST(VectorSecureTests, DefaultAllocatorShouldBe_SanitizingAllocator)
{
    vector_secure<Type> vec;
    EXPECT_TRUE((std::is_same<typename decltype(vec)::allocator_type, SanitizingAllocator<Type>>::value));
}

TEST(VectorSecureTests, CanDefineVectorWithDerivedSanitizingAllocator)
{
    vector_secure<Type, SanitizingAllocatorChild<Type>> vec;
    EXPECT_TRUE((std::is_base_of<SanitizingAllocator<Type>, decltype(vec)::allocator_type>::value));
}

TEST(VectorSecureTests, ChildAllocator_NotEqual_SanitizingAllocator)
{
    vector_secure<Type, SanitizingAllocatorChild<Type>> vec;
    EXPECT_FALSE((std::is_same<typename decltype(vec)::allocator_type, SanitizingAllocator<Type>>::value));
}


class VectorSecureConstructorsTest : public testing::Test {
protected:
    void SetUp()
    {
        vec_ = vector_secure<uint8_t>(INIT_DATA);
    }

    static constexpr std::initializer_list<uint8_t> INIT_DATA = {
            0x12, 0x34, 0x56, 0x78,
            0x90, 0xab, 0xcd, 0xef
    };

    vector_secure<uint8_t> vec_;
};


TEST_F(VectorSecureConstructorsTest, InitializerListConstructorShouldBeOk)
{
    EXPECT_TRUE(std::equal(vec_.cbegin(), vec_.cend(),
                           INIT_DATA.begin(), INIT_DATA.end()));
}
