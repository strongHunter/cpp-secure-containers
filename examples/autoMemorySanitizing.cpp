#include <iostream>
#include <iomanip>

#include "SecureContainers.h"

void hexPrint(const void* ptr, size_t size)
{
    const uint8_t* data = static_cast<const uint8_t*>(ptr);

    for (size_t i = 0; i < size; ++i) {
        std::cout << std::setfill('0') << std::setw(2) << std::uppercase
                  << std::hex << static_cast<int>(data[i]) << " ";
        if ((i + 1) % 16 == 0)
            std::cout << std::endl;
    }
}

int main()
{
    const uint8_t* ptr8;
    const uint16_t* ptr16;
    const uint32_t* ptr32;
    const uint64_t* ptr64;

    size_t size8, size16, size32, size64;

    {
        vector_secure<uint8_t> buf8 = { 0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef,
                                        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17
        };
        vector_secure<uint16_t> buf16 = { 0x1234, 0x5678, 0x90ab, 0xcdef,
                                          0x1011, 0x1213, 0x1415, 0x1617
        };
        vector_secure<uint32_t> buf32 = { 0x12345678, 0x90abcdef,
                                          0x10111213, 0x14151617
        };
        vector_secure<uint64_t> buf64 = { 0x1234567890abcdef, 0x1011121314151617 };

        ptr8 = buf8.data();
        ptr16 = buf16.data();
        ptr32 = buf32.data();
        ptr64 = buf64.data();

        size8 = buf8.size() * sizeof(uint8_t);
        size16 = buf16.size() * sizeof(uint16_t);
        size32 = buf32.size() * sizeof(uint32_t);
        size64 = buf64.size() * sizeof(uint64_t);

        std::cout << "uint8_t vector:" << std::endl;
        hexPrint(ptr8, size8);
        std::cout << "uint16_t vector:" << std::endl;
        hexPrint(ptr16, size16);
        std::cout << "uint32_t vector:" << std::endl;
        hexPrint(ptr32, size32);
        std::cout << "uint64_t vector:" << std::endl;
        hexPrint(ptr64, size64);
    }
    std::cout << std::endl << "Destructors called" << std::endl;

    std::cout << "uint8_t vector:" << std::endl;
    hexPrint(ptr8, size8);
    std::cout << "uint16_t vector:" << std::endl;
    hexPrint(ptr16, size16);
    std::cout << "uint32_t vector:" << std::endl;
    hexPrint(ptr32, size32);
    std::cout << "uint64_t vector:" << std::endl;
    hexPrint(ptr64, size64);

    std::cout << std::endl << std::endl;


    const char* cPtr;
    const wchar_t* wPtr;
    #ifdef __cpp_lib_char8_t
    const char8_t* c8Ptr;
    #endif
    const char16_t* c16Ptr;
    const char32_t* c32Ptr;

    size_t cSize, wSize, c8Size, c16Size, c32Size;

    {
        string_secure str = "0123456789abcdef";
        wstring_secure wstr = L"0123456789abcdef";
        #ifdef __cpp_lib_char8_t
        u8string_secure str8 = u8"0123456789abcdef";
        #endif
        u16string_secure str16 = u"0123456789abcdef";
        u32string_secure str32 = U"0123456789abcdef";

        cPtr = str.c_str();
        wPtr = wstr.c_str();
        #ifdef __cpp_lib_char8_t
        c8Ptr = str8.c_str();
        #endif
        c16Ptr = str16.c_str();
        c32Ptr = str32.c_str();

        cSize = str.length() * sizeof(char);
        wSize = wstr.length() * sizeof(wchar_t);
        #ifdef __cpp_lib_char8_t
        c8Size = str8.length() * sizeof(char8_t);
        #endif
        c16Size = str16.length() * sizeof(char16_t);
        c32Size = str32.length() * sizeof(char32_t);

        std::cout << "char string:" << std::endl;
        hexPrint(cPtr, cSize);
        std::cout << "wchar_t string:" << std::endl;
        hexPrint(wPtr, wSize);
        #ifdef __cpp_lib_char8_t
        std::cout << "char8_t string:" << std::endl;
        hexPrint(c8Ptr, c8Size);
        #endif
        std::cout << "char16_t string:" << std::endl;
        hexPrint(c16Ptr, c16Size);
        std::cout << "char32_t string:" << std::endl;
        hexPrint(c32Ptr, c32Size);
    }
    std::cout << std::endl << "Destructors called" << std::endl;

    std::cout << "char string:" << std::endl;
    hexPrint(cPtr, cSize);
    std::cout << "wchar_t string:" << std::endl;
    hexPrint(wPtr, wSize);
    #ifdef __cpp_lib_char8_t
    std::cout << "char8_t string:" << std::endl;
    hexPrint(c8Ptr, c8Size);
    #endif
    std::cout << "char16_t string:" << std::endl;
    hexPrint(c16Ptr, c16Size);
    std::cout << "char32_t string:" << std::endl;
    hexPrint(c32Ptr, c32Size);

    return 0;
}
