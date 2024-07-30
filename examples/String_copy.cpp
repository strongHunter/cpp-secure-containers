#include "commonUtils.h"
#include <cpp_sc/basic_string_secure.h>

int main()
{
    string_secure str1 = "0123456789abcdef";
    string_secure str2 = "0123456789abcdef";
    string_secure str3 = "0123456789abcdef";
    string_secure str4 = "0123456789abcdef";
    string_secure str5 = "0123456789abcdef";


    std::cout << "string 1:" << std::endl;
    hexPrint(str1.c_str(), str1.size() * sizeof(char));
    std::cout << "string 2:" << std::endl;
    hexPrint(str2.c_str(), str2.size() * sizeof(char));
    std::cout << "string 3:" << std::endl;
    hexPrint(str3.c_str(), str3.size() * sizeof(char));
    std::cout << "string 4:" << std::endl;
    hexPrint(str4.c_str(), str4.size() * sizeof(char));
    std::cout << "string 5:" << std::endl;
    hexPrint(str5.c_str(), str5.size() * sizeof(char));

    std::cout << std::endl << "Moving..." << std::endl;
    sanitizing_allocator<char> allocator;
    string_secure copy1(string_secure::copy(str1));
    string_secure copy2(string_secure::copy(str2, allocator));
    string_secure copy3(string_secure::copy(str3.cbegin(), str3.cend()));
    string_secure copy4(string_secure::copy(str4, 5));
    string_secure copy5(string_secure::copy(str5, 5, 7));

    std::cout << "string 1; copy 1 address:" << std::endl;
    std::cout << printAddress(str1.c_str()) << "; " << printAddress(copy1.c_str()) << std::endl;
    std::cout << "copy 1:" << std::endl;
    hexPrint(copy1.c_str(), copy1.size() * sizeof(char));
    std::cout << std::endl;

    std::cout << "string 2; copy 2 address:" << std::endl;
    std::cout << printAddress(str2.c_str()) << "; " << printAddress(copy2.c_str()) << std::endl;
    std::cout << "copy 2:" << std::endl;
    hexPrint(copy2.c_str(), copy2.size() * sizeof(char));
    std::cout << std::endl;

    std::cout << "string 3; copy 3 address:" << std::endl;
    std::cout << printAddress(str3.c_str()) << "; " << printAddress(copy3.c_str()) << std::endl;
    std::cout << "copy 3:" << std::endl;
    hexPrint(copy3.c_str(), copy3.size() * sizeof(char));
    std::cout << std::endl;

    std::cout << "string 4; copy 4 address:" << std::endl;
    std::cout << printAddress(str4.c_str()) << "; " << printAddress(copy4.c_str()) << std::endl;
    std::cout << "copy 4:" << std::endl;
    hexPrint(copy4.c_str(), copy4.size() * sizeof(char));
    std::cout << std::endl;

    std::cout << "string 5; copy 5 address:" << std::endl;
    std::cout << printAddress(str5.c_str()) << "; " << printAddress(copy5.c_str()) << std::endl;
    std::cout << "copy 5:" << std::endl;
    hexPrint(copy5.c_str(), copy5.size() * sizeof(char));
    std::cout << std::endl;

    return 0;
}
