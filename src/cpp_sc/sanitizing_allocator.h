#ifndef SANITIZING_ALLOCATOR_H
#define SANITIZING_ALLOCATOR_H

#include <cstdint>
#include <memory>

#include "platform.h"

template <typename T, template <typename> typename BasicAllocator,
        void(*CleanseFunc)(void*, size_t) = &burn>
struct sanitizing_allocator_base : public BasicAllocator<T> {
    using BasicAllocator<T>::BasicAllocator;

    template<typename U>
    struct rebind {
        typedef sanitizing_allocator_base<U, BasicAllocator, CleanseFunc> other;
    };

    static void sanitize(T* p, size_t n)
    {
        CleanseFunc(p, n * sizeof(T));
    }

    void deallocate(T* p, size_t n)
    {
        CleanseFunc(p, n);
        BasicAllocator<T>::deallocate(p, n);
    }
};


template <typename T>
using sanitizing_allocator = sanitizing_allocator_base<T, std::allocator>;

static_assert(sizeof(sanitizing_allocator<void>) == sizeof(std::allocator<void>),
              "Size of sanitizing_allocator is not equal to size of std::allocator");


template <typename Derived, template <typename, template <typename> typename> typename Base>
struct is_derived_from {
    template <typename T, template <typename> typename Alloc>
    static std::true_type __test(Base<T, Alloc>*);

    static std::false_type __test(...);

    using type = decltype(__test(std::declval<Derived*>()));
    static constexpr bool value = type::value;
};

template <typename Allocator>
concept SanitizingAllocatorDerived = is_derived_from<
        Allocator,
        sanitizing_allocator_base
>::value;

#endif // SANITIZING_ALLOCATOR_H
