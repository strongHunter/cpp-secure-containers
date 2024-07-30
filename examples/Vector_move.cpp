#include "commonUtils.h"
#include <cpp_sc/vector_secure.h>

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
    vector_secure<uint8_t> moved1(std::move(vec1));
    vector_secure<uint8_t> moved2(std::move(vec2), allocator);
    vector_secure<uint8_t> moved3 = std::move(vec3);

    std::cout << "vector 1 size; address:" << std::endl;
    std::cout << vec1.size() << "; " << printAddress(vec1.data()) << std::endl;
    std::cout << "moved 1:" << std::endl;
    hexPrint(moved1.data(), moved1.size() * sizeof(uint8_t));
    std::cout << std::endl;

    std::cout << "vector 2 size; address:" << std::endl;
    std::cout << vec2.size() << "; " << printAddress(vec2.data()) << std::endl;
    std::cout << "moved 2:" << std::endl;
    hexPrint(moved2.data(), moved2.size() * sizeof(uint8_t));
    std::cout << std::endl;

    std::cout << "vector 3 size; address:" << std::endl;
    std::cout << vec3.size() << "; " << printAddress(vec3.data()) << std::endl;
    std::cout << "moved 3:" << std::endl;
    hexPrint(moved3.data(), moved3.size() * sizeof(uint8_t));
    std::cout << std::endl;

    return 0;
}
