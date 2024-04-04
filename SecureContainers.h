#ifndef SECURE_CONTAINERS_H
#define SECURE_CONTAINERS_H

#include <cstdint>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

#include "platform.h"

template <typename T>
struct SanitizingAllocator : public std::allocator<T> {
public:
    using std::allocator<T>::allocator;

    template<typename U>
    struct rebind {
        typedef SanitizingAllocator<U> other;
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
        std::allocator<T>::deallocate(p, n);
    }
};

template<typename T>
concept IsSanitizingAllocator = std::is_base_of<SanitizingAllocator<typename T::value_type>, T>::value;


template <typename T, IsSanitizingAllocator Alloc = SanitizingAllocator<T>>
class vector_secure : public std::vector<T, Alloc>
{
public:
    using std::vector<T, Alloc>::vector;

    constexpr vector_secure(vector_secure&& other) noexcept
        : std::vector<T, Alloc>(std::move(other))
    {}

    constexpr vector_secure(vector_secure&& other, const Alloc& alloc)
        : std::vector<T, Alloc>(std::move(other), alloc)
    {}

    constexpr vector_secure& operator=(vector_secure&& other) noexcept
    {
        std::vector<T, Alloc>::operator=(std::move(other));
        return *this;
    }
};


template <typename T, IsSanitizingAllocator Alloc = SanitizingAllocator<T>>
class basic_string_secure : public std::basic_string<T, std::char_traits<T>, Alloc>
{
public:
    using std::basic_string<T, std::char_traits<T>, Alloc>::basic_string;

    /**
     * \fn  ~basic_string_secure()
     * \brief Custom destructor that clear memory for short strings
     *
     * Because of "C++ small string optimization" std::basic_string does not allocate
     * memory for strings smaller than 16 bytes, so SecureAllocator is never invoked.
     * In case of a short string we call the cleanse method directly from this
     * destructor.
     *
     * \see https://tc-imba.github.io/posts/cpp-sso
     */
    ~basic_string_secure()
    {
        auto n = this->size();
        if (n < 16) {
            Alloc::sanitize(this->data(), n);
        }
    }
};

using string_secure = basic_string_secure<char>;
using wstring_secure = basic_string_secure<wchar_t>;
#ifdef __cpp_lib_char8_t
using u8string_secure = basic_string_secure<char8_t>;
#endif // __cpp_lib_char8_t
using u16string_secure = basic_string_secure<char16_t>;
using u32string_secure = basic_string_secure<char32_t>;

#endif //SECURE_CONTAINERS_H
