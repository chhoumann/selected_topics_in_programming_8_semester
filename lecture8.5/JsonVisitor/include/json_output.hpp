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

        if (ignore_chars.find(value) != ignore_chars.end())
            os << value;
        else
            os << '"' << value << '"';
        return *this;
    }

    template <Container T>
    json_ostream& operator<<(const T& value)
    {
        os << '[';
        for (auto it = value.begin(); it != value.end(); ++it) {
            if (it != value.begin())
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

    std::string write_key(const std::string& key)
    {
        if (!isFirstPairInObject)
            j.os << ',';

        isFirstPairInObject = false;

        return key;
    }

    template <typename Data>
    void visit(const std::string& key, const Data& value)
    {
        /** TODO: use static visitor pattern to write class fields into output stream */
        if (!key.empty()) {
            if (!isFirstPairInObject)
                j.os << ',';
            j.os << '"' << key << "\":";
            isFirstPairInObject = false;
        }

        bool was_first = isFirstPairInObject; // for nested objects

        isFirstPairInObject = true;

        j.os << '{';
        value.accept(*this);
        j.os << '}';

        isFirstPairInObject = true;
    }

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
    writer.visit("", value);
    return j;
}

template <typename T>
json_ostream& operator<<(json_ostream&& j, const T& value)
{
    json_writer_t writer{j};
    writer.visit("", value);
    return j;
}

#endif  // STATIC_VISITOR_JSON_OUTPUT_HPP
