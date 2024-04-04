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
    vector_secure<uint8_t> moved1(std::move(vec1));
    vector_secure<uint8_t> moved2(std::move(vec2), allocator);
    vector_secure<uint8_t> moved3 = std::move(vec3);

    std::cout << "vector 1 size; address:" << std::endl;
    std::cout << vec1.size() << "; " << printAddress(vec1.data()) << std::endl;
    std::cout << "moved 1:" << std::endl;
    hexPrint(ptr1, size1);
    std::cout << std::endl;

    std::cout << "vector 2 size; address:" << std::endl;
    std::cout << vec2.size() << "; " << printAddress(vec2.data()) << std::endl;
    std::cout << "moved 2:" << std::endl;
    hexPrint(ptr2, size2);
    std::cout << std::endl;

    std::cout << "vector 3 size; address:" << std::endl;
    std::cout << vec3.size() << "; " << printAddress(vec3.data()) << std::endl;
    std::cout << "moved 3:" << std::endl;
    hexPrint(ptr3, size3);
    std::cout << std::endl;

    return 0;
}
