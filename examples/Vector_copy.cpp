#include "commonUtils.h"
#include "SecureContainers.h"

int main()
{
    const uint8_t* ptr1;
    const uint8_t* ptr2;
    const uint8_t* ptr3;
    size_t size1, size2, size3;

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

    ptr1 = vec1.data();
    ptr2 = vec2.data();
    ptr3 = vec3.data();

    size1 = vec1.size() * sizeof(uint8_t);
    size2 = vec2.size() * sizeof(uint8_t);
    size3 = vec3.size() * sizeof(uint8_t);

    std::cout << "vector 1:" << std::endl;
    hexPrint(ptr1, size1);
    std::cout << "vector 2:" << std::endl;
    hexPrint(ptr2, size2);
    std::cout << "vector 3:" << std::endl;
    hexPrint(ptr3, size3);

    std::cout << std::endl << "Moving..." << std::endl;
    SanitizingAllocator<uint8_t> allocator;
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
