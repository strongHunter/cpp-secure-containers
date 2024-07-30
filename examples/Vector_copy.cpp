#include "commonUtils.h"
#include <include/vector_secure.h>

int main()
{
    vector_secure<uint8_t> vec1 = {
            0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef,
            0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17
    };
    vector_secure<uint8_t> vec2 = {
            0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef,
            0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17
    };
    vector_secure<uint8_t> vec3 = {
            0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef,
            0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17
    };


    std::cout << "vector 1:" << std::endl;
    hexPrint(vec1.data(), vec1.size() * sizeof(uint8_t));
    std::cout << "vector 2:" << std::endl;
    hexPrint(vec2.data(), vec2.size() * sizeof(uint8_t));
    std::cout << "vector 3:" << std::endl;
    hexPrint(vec3.data(), vec3.size() * sizeof(uint8_t));

    std::cout << std::endl << "Moving..." << std::endl;
    sanitizing_allocator<uint8_t> allocator;
    vector_secure<uint8_t> copy1(vector_secure<uint8_t>::copy(vec1));
    vector_secure<uint8_t> copy2(vector_secure<uint8_t>::copy(vec2, allocator));
    vector_secure<uint8_t> copy3(vector_secure<uint8_t>::copy(vec3.cbegin(), vec3.cend()));

    std::cout << "vector 1; copy 1 address:" << std::endl;
    std::cout << printAddress(vec1.data()) << "; " << printAddress(copy1.data()) << std::endl;
    std::cout << "copy 1:" << std::endl;
    hexPrint(copy1.data(), copy1.size() * sizeof(uint8_t));
    std::cout << std::endl;

    std::cout << "vector 2; copy 2 address:" << std::endl;
    std::cout << printAddress(vec2.data()) << "; " << printAddress(copy2.data()) << std::endl;
    std::cout << "copy 2:" << std::endl;
    hexPrint(copy2.data(), copy2.size() * sizeof(uint8_t));
    std::cout << std::endl;

    std::cout << "vector 3; copy 3 address:" << std::endl;
    std::cout << printAddress(vec3.data()) << "; " << printAddress(copy3.data()) << std::endl;
    std::cout << "copy 3:" << std::endl;
    hexPrint(copy3.data(), copy3.size() * sizeof(uint8_t));
    std::cout << std::endl;

    return 0;
}
