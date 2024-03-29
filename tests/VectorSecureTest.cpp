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
