#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "SecureContainers.h"

static const char* _15SymbolsString = "0123456789abcde";
static const char* _16SymbolsString = "0123456789abcdef";

using testing::_;

template <typename T>
class SanitizeMock {
public:
    MOCK_METHOD(void, calledFromSanitize, (T*, size_t), ());
};

SanitizeMock<char>* mock = nullptr;


template <typename T>
class SanitizingAllocatorChild : public SanitizingAllocator<T> {
public:
    using SanitizingAllocator<T>::SanitizingAllocator;

    template<typename U>
    struct rebind {
        typedef SanitizingAllocatorChild<U> other;
    };

    static void sanitize(T* p, size_t n)
    {
        if (mock)
            mock->calledFromSanitize(p, n);
    }
};


TEST(StringSecureTest, DefaultAllocatorShouldBe_SanitizingAllocator)
{
    string_secure str;
    EXPECT_TRUE((std::is_same<typename decltype(str)::allocator_type, SanitizingAllocator<char>>::value));
}

TEST(StringSecureTest, CanDefineStringWithDerivedSanitizingAllocator)
{
    basic_string_secure<char, SanitizingAllocatorChild<char>> str;
    EXPECT_TRUE((std::is_base_of<SanitizingAllocator<char>, decltype(str)::allocator_type>::value));
}

TEST(StringSecureTest, ChildAllocator_NotEqual_SanitizingAllocator)
{
    basic_string_secure<char, SanitizingAllocatorChild<char>> str;
    EXPECT_FALSE((std::is_same<typename decltype(str)::allocator_type, SanitizingAllocator<char>>::value));
}


class StaticSanitizeTest : public testing::Test {
protected:
    void SetUp()
    {
        mock = &mock_;
    }

    void TearDown()
    {
        mock = nullptr;
    }

    SanitizeMock<char> mock_;
};

TEST_F(StaticSanitizeTest, ShouldCall_Sanitize_ForEmptyString)
{
    basic_string_secure<char, SanitizingAllocatorChild<char>> str;

    EXPECT_CALL(*mock, calledFromSanitize(_, _)).Times(1);
}

TEST_F(StaticSanitizeTest, ShouldCall_Sanitize_For15SymbolsString)
{
    basic_string_secure<char, SanitizingAllocatorChild<char>> str = _15SymbolsString;

    EXPECT_CALL(*mock, calledFromSanitize(_, _)).Times(1);
}

TEST_F(StaticSanitizeTest, ShouldNotCall_Sanitize_For16SymbolsString)
{
    basic_string_secure<char, SanitizingAllocatorChild<char>> str = _16SymbolsString;

    EXPECT_CALL(*mock, calledFromSanitize(_, _)).Times(0);
}
