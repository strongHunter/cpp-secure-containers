#ifndef BASIC_STRING_SECURE_H
#define BASIC_STRING_SECURE_H

#include <cstring>
#include <string>

#include "sanitizing_allocator.h"

template <typename CharT, SanitizingAllocatorDerived Allocator = sanitizing_allocator<CharT>>
class basic_string_secure : public std::basic_string<CharT, std::char_traits<CharT>, Allocator> {
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


template <typename SecureStringType>
[[nodiscard]]
inline decltype(auto) std_string_cast(SecureStringType&& str) noexcept
{
    using CharT = typename std::remove_reference_t<SecureStringType>::value_type;
    using StdStringType = std::basic_string<CharT>;

    static_assert(sizeof(SecureStringType) == sizeof(StdStringType),
                  "Size of basic_string_secure<CharT> is not equal to size of std::basic_string<CharT>");

    if constexpr (std::is_const_v<std::remove_reference_t<SecureStringType>> &&
                  std::is_lvalue_reference_v<SecureStringType>) {
        return reinterpret_cast<const StdStringType&>(str);
    } else if constexpr (std::is_lvalue_reference_v<SecureStringType>) {
        return reinterpret_cast<StdStringType&>(str);
    } else {
        // static_assert(false) not working yet
        struct dependent_false : std::false_type {};

        static_assert(dependent_false::value,
                      "Rvalue cannot be a type for cast");
    }
}

template <typename StdStringType>
[[nodiscard]]
inline decltype(auto) secure_string_cast(StdStringType&& str) noexcept
{
    using CharT = typename std::remove_reference_t<StdStringType>::value_type;
    using SecureStringType = basic_string_secure<CharT>;

    static_assert(std::is_rvalue_reference_v<StdStringType&&>,
                  "Only rvalue references are allowed for conversion to basic_string_secure");

    static_assert(sizeof(SecureStringType) == sizeof(StdStringType),
                  "Size of basic_string_secure<CharT> is not equal to size of std::basic_string<CharT>");

    return reinterpret_cast<SecureStringType&&>(str);
}

#endif // BASIC_STRING_SECURE_H
