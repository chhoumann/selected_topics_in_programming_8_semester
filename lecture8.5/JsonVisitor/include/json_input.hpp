#ifndef STATIC_VISITOR_JSON_INPUT_HPP
#define STATIC_VISITOR_JSON_INPUT_HPP

#include <iostream>
#include "meta.hpp"

/** TODO: implement json_istream adapter with json input operations
 * The goal is to exercise meta-programming and not have complete JSON (Unicode support is beyond the scope).
 * Parsing should follow the type structure rather than the content of the input stream.
 * Visitor parsing may depend on the order of fields, which is OK for this exercise.
 */

struct json_istream
{
    std::istream& is;

    template <Boolean T>
    json_istream& operator>>(T& value)
    {
        std::string s;
        is >> s;
        // Should have used boolalpha
        if (s == "true")
            value = true;
        else if (s == "false")
            value = false;
        else
            is.setstate(std::ios_base::failbit);
        return *this;
    }

    template <Number T>
    json_istream& operator>>(T& value)
    {
        is >> value;
        return *this;
    }

    template <String T>
    json_istream& operator>>(T& value)
    {
        char c;
        is >> c;
        if (c != '"') // OK, but consider `is.peek()` and then `is >> std::quoted(value);` instead of `std:getline`
            is.setstate(std::ios_base::failbit);
        else {
            std::getline(is, value, '"');
            if (is.fail())
                is.setstate(std::ios_base::failbit);
        }
        return *this;
    }

    template <Container T>
    json_istream& operator>>(T& value)
    {
        char c;
        is >> c;
        if (c != '[')
            is.setstate(std::ios_base::failbit);
        else {
            while (is >> c) {
                if (c == ']')
                    break;
                else if (c == ',')
                    continue;
                else
                    is.unget(); // should have used `is.peek()`
                typename T::value_type v; // this doesn't work with C arrays. Instead `std::iter_value_t<T>` could be used
                *this >> v;
                value.push_back(v); // should check if input is successful before pushing, i.e. if (is) value.push_back(v);
            }
        }
        return *this;
    }
};

/** Visitor pattern support for reading JSON */
struct json_reader_t
{
    json_istream& j;

    json_reader_t(json_istream& j) : j{j} {}

    template <typename Data>
    void visit(const std::string& name, Data& value)
    {
        value.accept(*this); // Need to read the field name first, check that the name is correct, the colon character is in place and only then handle the value. Also need to check that the commas are in correct places.
    }

    template <Boolean T>
    void visit(const std::string& name, T& value)
    {
        j >> value;
    }

    template <Number T>
    void visit(const std::string& name, T& value)
    {
        j >> value;
    }

    template <String T>
    void visit(const std::string& name, T& value)
    {
        j >> value;
    }

    template <Container T>
    void visit(const std::string& name, T& value)
    {
        j >> value;
    }
};

template <typename T>
json_istream& operator>>(json_istream& j, T& value) // This is a non-member input operator overload, but you already have member input operator overloads, why duplicate? Did it compile for you?
{
    /** TODO: implement input of arbitrary types so that tests in json_input_test pass.
     * Focus on one test at a time: begin from the first and then continue to next.
     * In order to support various types, develop and test meta-predicates (see meta_test.cpp)
     * Tip: either use if-constexpr or overloading with SFINAE/concepts
     */
    json_reader_t reader{j};
    reader.visit("", value); // Hackish-design. `json_reader_t` is needed only in case data accepts a visitor, therefore it needs to be constructed only in that case, while other cases can be handled with `if constexpr (is_bool_v<T>) { ... }`
    return j;
}

/** Helper for rvalue reference */
template <typename T>
json_istream& operator>>(json_istream&& j, T& value)
{
    return j >> value;
}

#endif  // STATIC_VISITOR_JSON_INPUT_HPP
