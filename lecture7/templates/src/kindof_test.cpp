/**
 * Exercise for lecture on templates and type deduction.
 * The purpose is to get acquainted with template specialization.
 * Author: Marius Mikucionis <marius@cs.aau.dk>
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

#include <vector>
#include <string>

#include <type_traits> // Needed for std::is_same

template <typename T> // Forward declare kind_t now, so we can specialize it for the different
struct kind_t; // types we want to support. The implementation is given below.

// Specialization for pointers
// This specialization handles pointer types. If the pointed-to type is a char, we return "C string"; otherwise, we return "pointer".
template <typename T>
struct kind_t<T*> {
    static std::string to_string() {
        if (std::is_same<char, T>::value) {
            return "C string";
        }
        return "pointer";
    }
};

// Specialization for const char* - C string
// This specialization handles C strings explicitly (const char pointers). We return "C string" for this case.
template <>
struct kind_t<const char*> {
    static std::string to_string() {
        return "C string";
    }
};

// Specialization for std::string - C++ string
// This specialization handles std::string types. We return "C++ string" for this case.
template <>
struct kind_t<std::string> {
    static std::string to_string() {
        return "C++ string";
    }
};

// Primary template implementation
// This is the primary kind_t template implementation, which handles all other non-specialized types. We return "type" for these cases.
template <typename T>
struct kind_t {
    static std::string to_string() {
        return "type";
    }
};

// kind_of function template
// This function template takes a value of type T and returns a string representation of its kind, as defined by the kind_t template class.
template <typename T>
std::string kind_of(T value) {
    return kind_t<decltype(value)>::to_string();
}


/**
 * Type Displayer template declaration without an implementation:
 * when invoked, it forces the compiler to issue an error and reveal the type argument.
 * Useful in compile-time type-debugging.
 */
template <typename T>
struct TD;

using namespace std;
using namespace std::string_literals; // ""s operator converting string literals into C++ string

TEST_CASE("Type kinds")
{
    auto v1 = 5;
    auto v2 = 3.141;
    auto v3 = "hello C";
    auto v4 = "hello C++"s;
    auto v5 = string("C++");
    auto v6 = new int(7);
    auto v7 = new char[11];
    auto v8 = vector<int>();

	CHECK(kind_of(v1)=="type");
	CHECK(kind_of(v2)=="type");
	CHECK(kind_of(v3)=="C string");
	CHECK(kind_of(v4)=="C++ string");
	CHECK(kind_of(v5)=="C++ string");
	CHECK(kind_of(v6)=="pointer");
	CHECK(kind_of(v7)=="C string");
	CHECK(kind_of(v8)=="type");
	
	delete v6;
	delete [] v7;
}

// The solution uses template specialization to define different behavior for different types. The kind_of function template then calls the appropriate specialization to get the string representation of the input's kind.



