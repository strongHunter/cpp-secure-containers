#ifndef SANITIZING_ALLOCATOR_H
#define SANITIZING_ALLOCATOR_H

#include <cstdint>
#include <memory>

#include "platform.h"

template <typename T, template <typename> typename BasicAllocator>
struct sanitizing_allocator_base : public BasicAllocator<T> {
    using BasicAllocator<T>::BasicAllocator;

    template<typename U>
    struct rebind {
        typedef sanitizing_allocator_base<U, BasicAllocator> other;
    };

    static void sanitize(T* p, size_t n)
    {
        burn(p, n * sizeof(T));
    }

    virtual void cleanse(T* p, size_t n)
    {
        sanitize(p, n);
    }

    void deallocate(T* p, size_t n)
    {
        cleanse(p, n);
        BasicAllocator<T>::deallocate(p, n);
    }
};


template <typename T>
using sanitizing_allocator = sanitizing_allocator_base<T, std::allocator>;

template<typename T>
concept SanitizingAllocatorDerived = std::is_base_of<sanitizing_allocator<typename T::value_type>, T>::value;

#endif // SANITIZING_ALLOCATOR_H
