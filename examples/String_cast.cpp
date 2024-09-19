#include <cassert>
#include <cpp_sc/basic_string_secure.h>

// External API, which accepts std::string in parameters
void foo(std::string& str) { /* do something */ }
void cfoo(const std::string& str) { /* do something */ }

int main()
{
    {
        string_secure secure_str = "Some data";

        std::string& std_str_ref = std_string_cast(secure_str);
        const std::string& const_std_str_ref = std_string_cast(secure_str);

        // Calling external API
        foo(std_str_ref);
        cfoo(const_std_str_ref);

        assert(secure_str.data() == std_str_ref.data());
        assert(secure_str.data() == const_std_str_ref.data());

        // ATTENTION
        // It's bad practice
        // string will be copied and data from `std_str_copy` will not be erased
        // cast to reference type only
        std::string std_str_copy = std_string_cast(secure_str);
        assert(secure_str.data() != std_str_copy.data());
    }

    return 0;
}
