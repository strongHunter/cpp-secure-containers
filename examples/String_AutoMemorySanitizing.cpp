#include "commonUtils.h"
#include <include/basic_string_secure.h>

int main()
{
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
