#ifndef STATIC_VISITOR_JSON_OUTPUT_HPP
#define STATIC_VISITOR_JSON_OUTPUT_HPP

#include <iostream>
#include "meta.hpp"
#include <unordered_set>

/** TODO: implement json_ostream adapter with json output operations
 * The goal is to exercise meta-programming and not have complete JSON (Unicode support is beyond the scope).
 */
struct json_ostream
{
    std::ostream& os;

    template <Boolean T>
    json_ostream& operator<<(const T& value)
    {
        os << (value ? "true" : "false");
        return *this;
    }

    template <Number T>
    json_ostream& operator<<(const T& value)
    {
        os << value;
        return *this;
    }

    template <String T>
    json_ostream& operator<<(const T& value)
    {
        static const std::unordered_set<std::string> ignore_chars = {":", ",",    "[",     "]",   "{",
                                                                     "}", "true", "false", "null"};

        if (ignore_chars.find(value) != ignore_chars.end()) // Inneficient (buffering) and convoluted design (what if the data contains one of those words?)
            os << value;
        else
            os << '"' << value << '"'; // OK, but consider `os << std::quoted(value);`, the I/O manipulators are very powerfull (std::quoted will escape the quotes in value).
        return *this;
    }

    template <Container T>
    json_ostream& operator<<(const T& value)
    {
        os << '[';
        for (auto it = value.begin(); it != value.end(); ++it) { // Use non-member `std::begin` and `std::end` which are more generic and works on C arrays.
            if (it != value.begin()) // OK, but it would be better to move the `if` outside the loop.
                os << ',';
            *this << *it;
        }
        os << ']';
        return *this;
    }
};

/** Visitor pattern support for writing JSON */
struct json_writer_t
{
    json_ostream& j;
    bool isFirstPairInObject = true;

    json_writer_t(json_ostream& j): j{j} {}

    // Creates an independent copy, inneficient! Either print the quoted key (better), or return `const std::string&`
    std::string write_key(const std::string& key)
    {
        if (!isFirstPairInObject)
            j.os << ',';

        isFirstPairInObject = false;

        return key;
    }

    template <typename Data> // requires accepts_v<const Data&, json_writer_t>
    void visit(const std::string& key, const Data& value)
    {
        /** TODO: use static visitor pattern to write class fields into output stream */
        if (!key.empty()) { // hack
            if (!isFirstPairInObject)
                j.os << ',';
            j.os << '"' << key << "\":"; // OK, but consider: j.os << std::quoted(key) << ':';
            isFirstPairInObject = false;
        }

        bool was_first = isFirstPairInObject; // for nested objects

        isFirstPairInObject = true;

        j.os << '{';
        value.accept(*this);
        j.os << '}';

        isFirstPairInObject = true; // should be was_first!!!
    }

    // Correct set of constrained overloads, but a simpler (more succinct) code would use
    // if constexpr (Boolean<T>) { ... }
    // else if constexpr (Number<T>) {...}
    template <Boolean T>
    void visit(const std::string& key, const T& value)
    {
        j << write_key(key) << ":" << value;
    }

    template <Number T>
    void visit(const std::string& key, const T& value)
    {
        j << write_key(key) << ":" << value;
    }

    template <String T>
    void visit(const std::string& key, const T& value)
    {
        j << write_key(key) << ":" << value;
    }

    template <Container T>
    void visit(const std::string& key, const T& value)
    {
        j << write_key(key) << ":" << value;
    }
};

template <typename T>
json_ostream& operator<<(json_ostream& j, const T& value)
{
    /** TODO: implement output of arbitrary types so that tests in json_test pass
     * Focus on one test in json_test at a time: begin from the first and then continue to next.
     * In order to support various types, develop and test meta-predicates (see meta_test.cpp)
     * Tip: either use if-constexpr or overloading with SFINAE/concepts
     */
    json_writer_t writer{j};
    writer.visit("", value); // convoluted design
    return j;
}

template <typename T>
json_ostream& operator<<(json_ostream&& j, const T& value)
{
    return j << value;
}

#endif  // STATIC_VISITOR_JSON_OUTPUT_HPP
