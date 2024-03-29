#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "SecureContainers.h"

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
class MockSanitizingAllocator : public SanitizingAllocator<T> {
public:
    MOCK_METHOD(void, cleanse,(T* p, size_t n), (override));
};

template <typename T>
class SanitizingAllocatorTest : public testing::Test {
protected:
    void SetUp() override
    {
        allocatorMock_ = std::make_unique<MockSanitizingAllocator<T>>();
        allocatorOrig_ = std::make_unique<SanitizingAllocator<T>>();
    }

    void TearDown() override
    {}

    std::unique_ptr<MockSanitizingAllocator<T>> allocatorMock_;
    std::unique_ptr<SanitizingAllocator<T>> allocatorOrig_;
};

TYPED_TEST_SUITE_P(SanitizingAllocatorTest);

TYPED_TEST_P(SanitizingAllocatorTest, DeallocateShouldCallCleanse)
{
    EXPECT_CALL(*this->allocatorMock_, cleanse(_, _)).Times(1);

    TypeParam* ptr = this->allocatorMock_->allocate(5);
    this->allocatorMock_->deallocate(ptr, 5);
}

TYPED_TEST_P(SanitizingAllocatorTest, CleanseShouldSetDataToNulls)
{
    const size_t count = 5;
    TypeParam* ptr = this->allocatorOrig_->allocate(count);
    fillData<TypeParam>(ptr, count);

    this->allocatorOrig_->cleanse(ptr, count);
    ASSERT_THAT(ptr, EachIsZero(count));

    this->allocatorOrig_->deallocate(ptr, count);
}

REGISTER_TYPED_TEST_SUITE_P(SanitizingAllocatorTest,
                            DeallocateShouldCallCleanse, CleanseShouldSetDataToNulls);

using TestingTypes = ::testing::Types<uint8_t, uint16_t, uint32_t, uint64_t, float, double>;
INSTANTIATE_TYPED_TEST_SUITE_P(NumericInstantiation, SanitizingAllocatorTest, TestingTypes);
