#ifndef VECTOR_SECURE_H
#define VECTOR_SECURE_H

#include <vector>

#include "sanitizing_allocator.h"

template <typename T, SanitizingAllocatorDerived Allocator = sanitizing_allocator<T>>
class vector_secure : public std::vector<T, Allocator> {
public:
    using std::vector<T, Allocator>::vector;

    constexpr vector_secure(vector_secure&& other) noexcept
            : std::vector<T, Allocator>(std::move(other))
    {}

    constexpr vector_secure(vector_secure&& other, const Allocator& alloc)
            : std::vector<T, Allocator>(std::move(other), alloc)
    {}

    constexpr vector_secure& operator=(vector_secure&& other) noexcept
    {
        std::vector<T, Allocator>::operator=(std::move(other));
        return *this;
    }

    template<class InputIt>
    [[nodiscard]] static vector_secure copy(InputIt first, InputIt last, const Allocator& alloc = Allocator())
    {
        return vector_secure(first, last, alloc);
    }

    [[nodiscard]] static vector_secure copy(const vector_secure& other)
    {
        return vector_secure(other);
    }

    [[nodiscard]] static vector_secure copy(const vector_secure& other, const Allocator& alloc)
    {
        return vector_secure(other, alloc);
    }

protected:
    template<class InputIt>
    constexpr vector_secure(InputIt first, InputIt last, const Allocator& alloc = Allocator())
            : std::vector<T, Allocator>(first, last, alloc)
    {}

    constexpr vector_secure(const vector_secure& other)
            : std::vector<T, Allocator>(other)
    {}

    constexpr vector_secure(const vector_secure& other, const Allocator& alloc)
            : std::vector<T, Allocator>(other, alloc)
    {}
};

#endif // VECTOR_SECURE_H
