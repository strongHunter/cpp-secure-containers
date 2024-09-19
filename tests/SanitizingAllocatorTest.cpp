#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <cpp_sc/sanitizing_allocator.h>

using testing::_;

template <typename T>
static void fillData(T* ptr, size_t count)
{
    for (size_t i = 0; i < count; ++i)
        ptr[i] = T(i);
}

MATCHER_P(EachIsZero, count, "All bytes are 0")
{
    return std::all_of(arg, arg + count,
                       [](auto x) { return x == 0; });
}


template <typename T>
class CleanseTest : public testing::Test {
protected:
    static bool cleanseCalled_;

    void SetUp() override
    {
        cleanseCalled_ = false;
    }

    static void cleanseCalled(void*, size_t) noexcept
    {
        cleanseCalled_ = true;
    }
};
template <typename T> bool CleanseTest<T>::cleanseCalled_;
TYPED_TEST_SUITE_P(CleanseTest);

TYPED_TEST_P(CleanseTest, DeallocateShouldCallCleanse)
{
    sanitizing_allocator_base<TypeParam, std::allocator, &CleanseTest<TypeParam>::cleanseCalled> allocator;

    TypeParam* p = allocator.allocate(5);
    allocator.deallocate(p, 5);

    EXPECT_TRUE(CleanseTest<TypeParam>::cleanseCalled_);
}


template <typename T>
class SanitizingAllocatorTest : public testing::Test {
protected:
    void SetUp() override
    {
        allocator_ = std::make_unique<sanitizing_allocator<T>>();
    }

    void TearDown() override
    {}

    std::unique_ptr<sanitizing_allocator<T>> allocator_;
};
TYPED_TEST_SUITE_P(SanitizingAllocatorTest);

TYPED_TEST_P(SanitizingAllocatorTest, CleanseShouldSetDataToNulls)
{
    const size_t count = 5;
    TypeParam* ptr = this->allocator_->allocate(count);
    fillData<TypeParam>(ptr, count);

    this->allocator_->sanitize(ptr, count);
    ASSERT_THAT(ptr, EachIsZero(count));

    this->allocator_->deallocate(ptr, count);
}

REGISTER_TYPED_TEST_SUITE_P(CleanseTest, DeallocateShouldCallCleanse);
REGISTER_TYPED_TEST_SUITE_P(SanitizingAllocatorTest, CleanseShouldSetDataToNulls);

using TestingTypes = ::testing::Types<uint8_t, uint16_t, uint32_t, uint64_t, float, double>;
INSTANTIATE_TYPED_TEST_SUITE_P(NumericInstantiation, CleanseTest, TestingTypes);
INSTANTIATE_TYPED_TEST_SUITE_P(NumericInstantiation, SanitizingAllocatorTest, TestingTypes);
