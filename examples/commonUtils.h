#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#include <iostream>
#include <iomanip>
#include <sstream>

inline void hexPrint(const void* ptr, size_t size)
{
    const uint8_t* data = static_cast<const uint8_t*>(ptr);

    for (size_t i = 0; i < size; ++i) {
        std::cout << std::setfill('0') << std::setw(2) << std::uppercase
                  << std::hex << static_cast<int>(data[i]) << " ";
        if ((i + 1) % 16 == 0)
            std::cout << std::endl;
    }
}

inline std::string printAddress(const void* ptr)
{
    std::stringstream ss;
    if (ptr == nullptr)
        ss << "NULL";
    else
        ss << std::hex << std::showbase << std::internal << std::setfill('0') << ptr;

    return ss.str();
}

#endif // COMMON_UTILS_H
