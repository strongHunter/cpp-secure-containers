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


template <typename T, IsSanitizingAllocator Allocator = SanitizingAllocator<T>>
class vector_secure : public std::vector<T, Allocator>
{
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


template <typename CharT, IsSanitizingAllocator Allocator = SanitizingAllocator<CharT>>
class basic_string_secure : public std::basic_string<CharT, std::char_traits<CharT>, Allocator>
{
public:
    using std::basic_string<CharT, std::char_traits<CharT>, Allocator>::basic_string;
    using size_type = typename std::basic_string<CharT, std::char_traits<CharT>, Allocator>::size_type;

    template <typename BasicAllocator>
    static basic_string_secure fromString(std::basic_string<CharT, std::char_traits<CharT>, BasicAllocator>&& other) noexcept
    {
        return basic_string_secure(std::move(other));
    }


    // move constructors
    constexpr basic_string_secure(basic_string_secure&& other) noexcept
        : std::basic_string<CharT, std::char_traits<CharT>, Allocator>()
    {
        swap(*this, other);
    }

    constexpr basic_string_secure(basic_string_secure&& other, const Allocator& alloc)
        : std::basic_string<CharT, std::char_traits<CharT>, Allocator>(alloc)
    {
        swap(*this, other);
    }

    constexpr basic_string_secure& operator=(basic_string_secure other) noexcept
    {
        swap(*this, other);
        return *this;
    }

    [[nodiscard]] static basic_string_secure copy(const basic_string_secure& other, size_type pos,
                                                  const Allocator& alloc = Allocator())
    {
        return basic_string_secure(other, pos, alloc);
    }

    [[nodiscard]] static basic_string_secure copy(const basic_string_secure& other,
                                                  size_type pos, size_type count,
                                                  const Allocator& alloc = Allocator())
    {
        return basic_string_secure(other, pos, count, alloc);
    }

    template<class InputIt>
    [[nodiscard]] static basic_string_secure copy(InputIt first, InputIt last, const Allocator& alloc = Allocator())
    {
        return basic_string_secure(first, last, alloc);
    }

    [[nodiscard]] static basic_string_secure copy(const basic_string_secure& other)
    {
        return basic_string_secure(other);
    }

    [[nodiscard]] static basic_string_secure copy(const basic_string_secure& other, const Allocator& alloc)
    {
        return basic_string_secure(other, alloc);
    }

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
            Allocator::sanitize(this->data(), n);
        }
    }

    constexpr basic_string_secure substr(size_type pos = 0, size_type count = basic_string_secure::npos) const
    {
        return std::basic_string<CharT, std::char_traits<CharT>, Allocator>::substr(pos, count);
    }

    template<typename LhsT, typename RhsT>
    friend basic_string_secure operator+(LhsT&& lhs, RhsT&& rhs)
    {
        basic_string_secure result(std::forward<LhsT>(lhs));
        result += std::forward<RhsT>(rhs);
        return result;
    }

    friend void swap(basic_string_secure& lhs, basic_string_secure& rhs) noexcept
    {
        std::swap(static_cast<std::basic_string<CharT, std::char_traits<CharT>, Allocator>&>(lhs),
                  static_cast<std::basic_string<CharT, std::char_traits<CharT>, Allocator>&>(rhs));
    }

protected:
    constexpr basic_string_secure(CharT ch)
        : std::basic_string<CharT, std::char_traits<CharT>, Allocator>(1, ch)
    {}

    template <typename BasicAllocator>
    constexpr basic_string_secure(std::basic_string<CharT, std::char_traits<CharT>, BasicAllocator> other) noexcept
            : std::basic_string<CharT, std::char_traits<CharT>, Allocator>(std::move(other))
    {}

    constexpr basic_string_secure(const basic_string_secure& other, size_type pos,
                                  const Allocator& alloc = Allocator())
          : std::basic_string<CharT, std::char_traits<CharT>, Allocator>(other, pos, alloc)
    {}

    constexpr basic_string_secure(const basic_string_secure& other,
                                  size_type pos, size_type count,
                                  const Allocator& alloc = Allocator())
          : std::basic_string<CharT, std::char_traits<CharT>, Allocator>(other, pos, count, alloc)
    {}

    template<class InputIt>
    constexpr basic_string_secure(InputIt first, InputIt last, const Allocator& alloc = Allocator())
        : std::basic_string<CharT, std::char_traits<CharT>, Allocator>(first, last, alloc)
    {}

    constexpr basic_string_secure(const basic_string_secure& other)
        : std::basic_string<CharT, std::char_traits<CharT>, Allocator>(other)
    {}

    constexpr basic_string_secure(const basic_string_secure& other, const Allocator& alloc)
        : std::basic_string<CharT, std::char_traits<CharT>, Allocator>(other, alloc)
    {}
};

using string_secure = basic_string_secure<char>;
using wstring_secure = basic_string_secure<wchar_t>;
#ifdef __cpp_lib_char8_t
using u8string_secure = basic_string_secure<char8_t>;
#endif // __cpp_lib_char8_t
using u16string_secure = basic_string_secure<char16_t>;
using u32string_secure = basic_string_secure<char32_t>;

#endif //SECURE_CONTAINERS_H
