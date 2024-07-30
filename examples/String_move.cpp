#include "commonUtils.h"
#include <cpp_sc/basic_string_secure.h>

int main()
{
    string_secure str1 = "0123456789abcdef";
    string_secure str2 = "0123456789abcdef";
    string_secure str3 = "0123456789abcdef";


    std::cout << "string 1:" << std::endl;
    hexPrint(str1.c_str(), str1.size() * sizeof(char));
    std::cout << "string 2:" << std::endl;
    hexPrint(str2.c_str(), str2.size() * sizeof(char));
    std::cout << "string 3:" << std::endl;
    hexPrint(str3.c_str(), str3.size() * sizeof(char));

    std::cout << std::endl << "Moving..." << std::endl;
    sanitizing_allocator<char> allocator;
    string_secure moved1(std::move(str1));
    string_secure moved2(std::move(str2), allocator);
    string_secure moved3 = std::move(str3);

    std::cout << "string 1 size:" << std::endl;
    std::cout << str1.size() << std::endl;
    std::cout << "string 1 data" << std::endl;
    hexPrint(&str1, sizeof(str1));
    std::cout << std::endl << "moved 1:" << std::endl;
    hexPrint(moved1.c_str(), moved1.size() * sizeof(char));
    std::cout << std::endl;

    std::cout << "string 2 size:" << std::endl;
    std::cout << str2.size() << std::endl;
    std::cout << "string 2 data" << std::endl;
    hexPrint(&str2, sizeof(str2));
    std::cout << std::endl << "moved 2:" << std::endl;
    hexPrint(moved2.c_str(), moved2.size() * sizeof(char));
    std::cout << std::endl;

    std::cout << "string 3 size:" << std::endl;
    std::cout << str3.size() << std::endl;
    std::cout << "string 3 data" << std::endl;
    hexPrint(&str3, sizeof(str3));
    std::cout << std::endl << "moved 3:" << std::endl;
    hexPrint(moved3.c_str(), moved3.size() * sizeof(char));
    std::cout << std::endl;

    return 0;
}
