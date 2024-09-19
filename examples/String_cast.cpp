#include <cassert>
#include <cpp_sc/basic_string_secure.h>

// External API, which accepts std::string in parameters
void foo(std::string& str) { /* do something */ }
void cfoo(const std::string& str) { /* do something */ }

std::string getSmallString()
{
    std::string result = "Some data";
    return result;
}

std::string getString()
{
    std::string result = "Some long text > 16 bytes";
    return result;
}

int main()
{
    // foo(string); examples
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

    // string = bar(); examples
    {
        std::string string = getString();
        const char* data = string.data();

        string_secure secure_str = secure_string_cast(std::move(string));
        assert(secure_str.data() == data);

        // ATTENTION
        // Small String Optimization (SSO):
        // If the string is small enough, its data is stored directly in the string object (on the stack)
        // instead of allocating memory on the heap.
        //
        // In such cases, moving the string will result in a copy of the internal buffer rather than
        // a transfer of ownership, because the data is part of the original object's storage.
        // Therefore, `small_string` data will not be erased
        std::string small_string = getSmallString();
        const char* small_data = small_string.data();

        string_secure small_secure_string = secure_string_cast(std::move(small_string));
        assert(small_secure_string.data() != small_data);
    }

    return 0;
}
