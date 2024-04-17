#ifndef PLATFORM_H
#define PLATFORM_H

#if (defined(_WIN32) || defined(_WIN64))
#define __WINDOWS_API__
#include <windows.h>
#elif defined(linux)
#define __LINUX_API__
#include <cstring>
#elif defined(__APPLE__)
#define __MAC_OS_API__
#include <cstring>
#endif

#include <cstdint>
inline void burn(void* ptr, size_t size) noexcept
{
#if defined(__WINDOWS_API__)
    SecureZeroMemory(ptr, size);

#elif (defined(__LINUX_API__) || defined(__MAC_OS_API__))
    memset(ptr, 0, size);
    asm volatile("" : : "r"(ptr) : "memory");

#else
    volatile uint8_t* p = static_cast<volatile uint8_t*>(ptr);
    size_t len = size;

    while (len--) *p++ = 0;
#endif
}

#endif // PLATFORM_H
