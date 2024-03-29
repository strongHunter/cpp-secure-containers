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

    virtual void cleanse(T* p, size_t n)
    {
        burn(p, n * sizeof(T));
    }

    void deallocate(T* p, size_t n)
    {
        cleanse(p, n);
        std::allocator<T>::deallocate(p, n);
    }
};


template <typename T, typename A = SanitizingAllocator<T>>
class vector_secure : public std::vector<T, SanitizingAllocator<T>>
{
public:
    using std::vector<T, SanitizingAllocator<T>>::vector;
};


template <typename T, typename A = SanitizingAllocator<T>>
class basic_string_secure : public std::basic_string<T, std::char_traits<T>, SanitizingAllocator<T>>
{
public:
    using std::basic_string<T, std::char_traits<T>, SanitizingAllocator<T>>::basic_string;

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
            this->get_allocator().cleanse(this->data(), n);
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
