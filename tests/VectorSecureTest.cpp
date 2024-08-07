#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <cpp_sc/vector_secure.h>

using Type = uint32_t;

template <typename T>
class SanitizingAllocatorChild : public sanitizing_allocator<T> {
public:
    using sanitizing_allocator<T>::sanitizing_allocator;

    template<typename U>
    struct rebind {
        typedef SanitizingAllocatorChild<U> other;
    };
};


TEST(VectorSecureTests, DefaultAllocatorShouldBe_SanitizingAllocator)
{
    vector_secure<Type> vec;
    EXPECT_TRUE((std::is_same<typename decltype(vec)::allocator_type, sanitizing_allocator<Type>>::value));
}

TEST(VectorSecureTests, CanDefineVectorWithDerivedSanitizingAllocator)
{
    vector_secure<Type, SanitizingAllocatorChild<Type>> vec;
    EXPECT_TRUE((std::is_base_of<sanitizing_allocator<Type>, decltype(vec)::allocator_type>::value));
}

TEST(VectorSecureTests, ChildAllocator_NotEqual_SanitizingAllocator)
{
    vector_secure<Type, SanitizingAllocatorChild<Type>> vec;
    EXPECT_FALSE((std::is_same<typename decltype(vec)::allocator_type, sanitizing_allocator<Type>>::value));
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

template <typename T>
testing::AssertionResult VectorsEqual(const vector_secure<T>& l, const vector_secure<T>& r)
{
    return std::equal(l.cbegin(), l.cend(), r.cbegin(), r.cend()) ?
        testing::AssertionSuccess()
        : testing::AssertionFailure() << "Vectors not equal";
}


TEST_F(VectorSecureConstructorsTest, InitializerListConstructorShouldBeOk)
{
    EXPECT_TRUE(std::equal(vec_.cbegin(), vec_.cend(),
                           INIT_DATA.begin(), INIT_DATA.end()));
}

TEST_F(VectorSecureConstructorsTest, MoveConstructorShouldMakeMovedVectorEmpty)
{
    uint8_t* ptr1 = vec_.data();
    uint8_t* ptr2;

    vector_secure<uint8_t> moved(std::move(vec_));
    ptr2 = moved.data();

    EXPECT_TRUE(vec_.empty());
    EXPECT_EQ(ptr1, ptr2);
}

TEST_F(VectorSecureConstructorsTest, MoveConstructorWithAllocatorShouldMakeMovedVectorEmpty)
{
    sanitizing_allocator<uint8_t> allocator;
    uint8_t* ptr1 = vec_.data();
    uint8_t* ptr2;

    vector_secure<uint8_t> moved(std::move(vec_), allocator);
    ptr2 = moved.data();

    EXPECT_TRUE(vec_.empty());
    EXPECT_EQ(ptr1, ptr2);
}

TEST_F(VectorSecureConstructorsTest, MoveAssignmentOperatorShouldMakeMovedVectorEmpty)
{
    uint8_t* ptr1 = vec_.data();
    uint8_t* ptr2;

    vector_secure<uint8_t> moved = std::move(vec_);
    ptr2 = moved.data();

    EXPECT_TRUE(vec_.empty());
    EXPECT_EQ(ptr1, ptr2);
}

TEST_F(VectorSecureConstructorsTest, MethodCopyShouldCopiesDataCorrectly)
{
    uint8_t* ptr1 = vec_.data();
    uint8_t* ptr2;

    vector_secure<uint8_t> copied = vector_secure<uint8_t>::copy(vec_);
    ptr2 = copied.data();

    EXPECT_NE(ptr1, ptr2);
    EXPECT_TRUE(VectorsEqual(vec_, copied));
}

TEST_F(VectorSecureConstructorsTest, MethodCopyWithAllocatorShouldCopiesDataCorrectly)
{
    sanitizing_allocator<uint8_t> allocator;
    uint8_t* ptr1 = vec_.data();
    uint8_t* ptr2;

    vector_secure<uint8_t> copied = vector_secure<uint8_t>::copy(vec_, allocator);
    ptr2 = copied.data();

    EXPECT_NE(ptr1, ptr2);
    EXPECT_TRUE(VectorsEqual(vec_, copied));
}

TEST_F(VectorSecureConstructorsTest, MethodCopyByIteratorsShouldCopiesDataCorrectly)
{
    uint8_t* ptr1 = vec_.data();
    uint8_t* ptr2;

    vector_secure<uint8_t> copied = vector_secure<uint8_t>::copy(vec_.cbegin(), vec_.cend());
    ptr2 = copied.data();

    EXPECT_NE(ptr1, ptr2);
    EXPECT_TRUE(VectorsEqual(vec_, copied));
}
