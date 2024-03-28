#include "SecureContainers.h"

int main()
{
    {
        vector_secure<uint8_t> buf8(10);
        vector_secure<uint16_t> buf16(10);
        vector_secure<uint32_t> buf32(10);
        vector_secure<uint64_t> buf64(10);

        string_secure str = "0123456789abcdef";
        wstring_secure wstr = L"0123456789abcdef";
        #ifdef __cpp_lib_char8_t
        u8string_secure str8 = u8"0123456789abcdef";
        #endif
        u16string_secure str16 = u"0123456789abcdef";
        u32string_secure str32 = U"0123456789abcdef";
    }

    return 0;
}
