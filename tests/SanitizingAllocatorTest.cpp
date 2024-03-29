#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "SecureContainers.h"

using testing::_;

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
        allocator_ = std::make_unique<MockSanitizingAllocator<T>>();
    }

    void TearDown() override
    {}

    std::unique_ptr<MockSanitizingAllocator<T>> allocator_;
};

TYPED_TEST_SUITE_P(SanitizingAllocatorTest);

TYPED_TEST_P(SanitizingAllocatorTest, DeallocateShouldCallCleanse)
{
    EXPECT_CALL(*this->allocator_, cleanse(_, _)).Times(1);

    TypeParam* ptr = this->allocator_->allocate(5);
    this->allocator_->deallocate(ptr, 5);
}

REGISTER_TYPED_TEST_SUITE_P(SanitizingAllocatorTest, DeallocateShouldCallCleanse);

using TestingTypes = ::testing::Types<uint8_t, uint16_t, uint32_t, uint64_t, float, double>;
INSTANTIATE_TYPED_TEST_SUITE_P(NumericInstantiation, SanitizingAllocatorTest, TestingTypes);
