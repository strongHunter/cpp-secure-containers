#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <cpp_sc/basic_string_secure.h>

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
class SanitizingAllocatorChild : public sanitizing_allocator<T> {
public:
    using sanitizing_allocator<T>::sanitizing_allocator;

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
    EXPECT_TRUE((std::is_same<typename decltype(str)::allocator_type, sanitizing_allocator<char>>::value));
}

TEST(StringSecureTest, CanDefineStringWithDerivedSanitizingAllocator)
{
    basic_string_secure<char, SanitizingAllocatorChild<char>> str;
    EXPECT_TRUE((std::is_base_of<sanitizing_allocator<char>, decltype(str)::allocator_type>::value));
}

TEST(StringSecureTest, ChildAllocator_NotEqual_SanitizingAllocator)
{
    basic_string_secure<char, SanitizingAllocatorChild<char>> str;
    EXPECT_FALSE((std::is_same<typename decltype(str)::allocator_type, sanitizing_allocator<char>>::value));
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


class StringSecureConstructorTest : public testing::Test {
protected:
    void SetUp()
    {
        str_ = string_secure("1234567890abcdef");
    }

    string_secure str_;
};

TEST_F(StringSecureConstructorTest, FromStringShouldMakeStringEmpty)
{
    std::string s15 = _15SymbolsString;
    std::string s16 = _16SymbolsString;

    str_ = string_secure::fromString(std::move(s15));
    EXPECT_TRUE(s15.empty());

    str_ = string_secure::fromString(std::move(s16));
    EXPECT_TRUE(s16.empty());
}

TEST_F(StringSecureConstructorTest, FromStringDontEraseMemoryForShortString)
{
    std::string s15 = _15SymbolsString;
    char* ptr15 = s15.data();

    str_ = string_secure::fromString(std::move(s15));
    EXPECT_TRUE(0 == memcmp(ptr15 + 1, _15SymbolsString + 1, strlen(_15SymbolsString) - 1));
}

TEST_F(StringSecureConstructorTest, FromStringShouldCopyMemoryForLongString)
{
    std::string s16 = _16SymbolsString;
    char* ptr16 = s16.data();

    str_ = string_secure::fromString(std::move(s16));
    EXPECT_NE(str_.data(), ptr16);
}

TEST(StringSecureTest, FromStringShouldMoveMemoryForEqualAllocator)
{
    std::basic_string<char, std::char_traits<char>, sanitizing_allocator<char>> s16 = _16SymbolsString;
    char* ptr16 = s16.data();

    string_secure str = string_secure::fromString(std::move(s16));
    EXPECT_EQ(str.data(), ptr16);
}

TEST_F(StringSecureConstructorTest, MoveConstructorShouldMakeMovedStringEmpty)
{
    char* ptr1 = str_.data();
    char* ptr2;

    string_secure moved(std::move(str_));
    ptr2 = moved.data();

    EXPECT_TRUE(str_.empty());
    EXPECT_EQ(ptr1, ptr2);
}

TEST_F(StringSecureConstructorTest, MoveConstructorWithAllocatorShouldMakeMovedStringEmpty)
{
    sanitizing_allocator<char> allocator;
    char* ptr1 = str_.data();
    char* ptr2;

    string_secure moved(std::move(str_), allocator);
    ptr2 = moved.data();

    EXPECT_TRUE(str_.empty());
    EXPECT_EQ(ptr1, ptr2);
}

TEST_F(StringSecureConstructorTest, MoveAssignmentOperatorShouldMakeMovedStringEmpty)
{
    char* ptr1 = str_.data();
    char* ptr2;

    string_secure moved = std::move(str_);
    ptr2 = moved.data();

    EXPECT_TRUE(str_.empty());
    EXPECT_EQ(ptr1, ptr2);
}

TEST(StringSecureTest, MoveAssignmentOperatorIntoExistingObjectShouldMakeMovedStringEmpty)
{
    string_secure str1;
    string_secure str2(_16SymbolsString);

    char* ptr1;
    char* ptr2 = str2.data();

    str1 = std::move(str2);
    ptr1 = str1.data();

    EXPECT_EQ(ptr1, ptr2);
}

TEST_F(StringSecureConstructorTest, MethodCopyShouldCopiesDataCorrectly)
{
    char* ptr1 = str_.data();
    char* ptr2;

    string_secure copied = string_secure::copy(str_);
    ptr2 = copied.data();

    EXPECT_NE(ptr1, ptr2);
    EXPECT_EQ(str_, copied);
}

TEST_F(StringSecureConstructorTest, MethodCopyWithAllocatorShouldCopiesDataCorrectly)
{
    sanitizing_allocator<char> allocator;
    char* ptr1 = str_.data();
    char* ptr2;

    string_secure copied = string_secure::copy(str_, allocator);
    ptr2 = copied.data();

    EXPECT_NE(ptr1, ptr2);
    EXPECT_EQ(str_, copied);
}

TEST_F(StringSecureConstructorTest, MethodCopyByIteratorsShouldCopiesDataCorrectly)
{
    char* ptr1 = str_.data();
    char* ptr2;

    string_secure copied = string_secure::copy(str_.cbegin(), str_.cend());
    ptr2 = copied.data();

    EXPECT_NE(ptr1, ptr2);
    EXPECT_EQ(str_, copied);
}

TEST_F(StringSecureConstructorTest, MethodCopyFromPosShouldCopiesDataCorrectly)
{
    str_ = "0123456789";
    string_secure copied = string_secure::copy(str_, 2);

    EXPECT_EQ(copied, "23456789");
}

TEST_F(StringSecureConstructorTest, MethodCopySomeSymbolsFromPosShouldCopiesDataCorrectly)
{
    str_ = "0123456789";
    string_secure copied = string_secure::copy(str_, 2, 5);

    EXPECT_EQ(copied, "23456");
}
