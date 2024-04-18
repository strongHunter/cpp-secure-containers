#include <iostream>
#include "SecureContainers.h"

// Example from https://en.cppreference.com/w/cpp/string/basic_string
int main()
{
    using namespace std::literals;

    // Creating a string from const char*
    string_secure str1 = "hello";

    // Creating a string using string literal
    string_secure str2 = string_secure::fromString("world"s);

    // Concatenating strings
    string_secure str3 = str1 + " " + str2;

    // Print out the result
    std::cout << str3 << '\n';

    string_secure::size_type pos = str3.find(" ");
    str1 = str3.substr(pos + 1); // the part after the space
    str2 = str3.substr(0, pos);  // the part till the space

    std::cout << str1 << ' ' << str2 << '\n';

    // Accessing an element using subscript operator[]
    std::cout << str1[0] << '\n';
    str1[0] = 'W';
    std::cout << str1 << '\n';
}
