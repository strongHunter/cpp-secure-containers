#include "commonUtils.h"
#include "vector_secure.h"

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
    return 0;
}
