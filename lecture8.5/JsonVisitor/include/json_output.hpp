#ifndef STATIC_VISITOR_JSON_OUTPUT_HPP
#define STATIC_VISITOR_JSON_OUTPUT_HPP

#include <iostream>

/** TODO: implement json_ostream adapter with json output operations
 * The goal is to exercise meta-programming and not have complete JSON (Unicode support is beyond the scope).
 */
struct json_ostream
{
    std::ostream& os;
};

/** Visitor pattern support for writing JSON */
struct json_writer_t
{
    template <typename Data>
    void visit(const std::string& name, const Data& value)
    {
        /** TODO: use static visitor pattern to write class fields into output stream */
    }
};

template <typename T>
json_ostream& operator<<(json_ostream& j, const T&)
{
    /** TODO: implement output of arbitrary types so that tests in json_test pass
     * Focus on one test in json_test at a time: begin from the first and then continue to next.
     * In order to support various types, develop and test meta-predicates (see meta_test.cpp)
     * Tip: either use if-constexpr or overloading with SFINAE/concepts
     */
    return j;
}

template <typename T>
json_ostream& operator<<(json_ostream&& j, const T& value)
{
    return j << value;
}

#endif  // STATIC_VISITOR_JSON_OUTPUT_HPP
