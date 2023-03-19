#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <type_traits>
#include <list>
#include <deque>
#include <set>

// Overload the output operator for std::vector<T>
template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
    os << '{';
    for (size_t i = 0; i < vec.size(); ++i) {
        if constexpr (std::is_same_v<T, const char*>)
            os << std::quoted(vec[i]);
        else
            os << vec[i];
        if (i < vec.size() - 1) os << ',';
    }
    os << '}';
    return os;
}

/*
This template function overloads the output operator for std::vector<T> where T is any printable type.
It iterates through the vector elements and prints them inside curly braces in a comma-separated format.
If the element type T is const char*, it uses std::quoted to add quotes around the element.
*/

// Generalized output operator for any container (excluding std::string)
template <typename C, typename T = typename C::value_type, typename std::enable_if<!std::is_same<C, std::string>::value, int>::type = 0>
std::ostream& operator<<(std::ostream& os, const C& container) {
    os << '{';
    auto it = container.begin();
    for (; it != container.end(); ++it) {
        if constexpr (std::is_same_v<T, const char*>)
            os << std::quoted(*it);
        else
            os << *it;
        if (std::next(it) != container.end()) os << ',';
    }
    os << '}';
    return os;
}

/*
This template function overloads the output operator for any container C with element type T, excluding std::string.
It uses SFINAE with std::enable_if to exclude std::string from this template.
The function iterates through the container using iterators and prints the elements inside curly braces in a comma-separated format.
If the element type T is const char*, it uses std::quoted to add quotes around the element.
*/

int main() {
    // Test cases for std::vector
    std::cout << std::vector{1, 2, 3} << std::endl;
    std::cout << std::vector{2.7, 3.14} << std::endl;
    std::cout << std::vector{std::vector{"hello", "template"}, std::vector{"world"}} << std::endl;

    // Test cases for generalized output operator
    std::list<int> intList{1, 2, 3};
    std::cout << intList << std::endl;

    std::deque<std::string> strDeque{"hello", "world"};
    std::cout << strDeque << std::endl;

    std::set<double> doubleSet{3.14, 2.718};
    std::cout << doubleSet << std::endl;

    return 0;
}
