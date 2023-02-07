// In this exercise we will explore pointers and references relative to each other. Play and learn! We do this in context of a function that returns the maximum of two doubles. Here is the function with call-by-value parameters: 
// 
// double max(double a, double b)
// { return a < b ? b : a; }
// For each of the cases below, program the max function, call it, make make sure you get the expected result from the call. In each of the cases, what would be a meaningful return type. Think, when would you want to use such an interface (maybe in a function that does more than the simple max, maybe in a function where double is replaced by some other, larger type).
// 
// Please, be aware that the cases become increasingly strange...
// 
// Make a version with C-style call by reference parameters - pointers passed by value.
// Make a version with call by C++ references.
// Make a version with call by C++ const references instead of call-by-value parameters.
// Demonstrate that pointers to references do not exist (does not compile).
// Make a version with references to pointers. Does that make sense to you?
// Next, make a version with const references to pointers.
// Can you imagine other interesting variations?
#include <iostream>

// C-style call by reference, pointers passed by value
double max_ptr(double *a, double *b) {
    return *a < *b ? *b : *a;
}

// Call by C++ references
double max_ref(double &a, double &b) {
    return a < b ? b : a;
}

// Call by C++ const reference
double max_const_ref(const double &a, const double &b) {
    return a < b ? b : a;
}

// pointers to references --- doesn't compile
// double &* max_ptr_to_ref(double &*a, double &*b) {
//     return *a < *b ? *b : *a;
// }

// Reference to pointers
double max_ref_to_ptr(double *&a, double *&b) {
    return *a < *b ? *b : *a;
}

// const references to pointers
double max(const double * const &a, const double * const &b) {
    return *a < *b ? *b : *a;
}

int main() {
    double a = 3.14, b = 2.72;
    double *pa = &a, *pb = &b;

    double result_ptr = max_ptr(pa, pb);
    std::cout << "Max of " << a << " and " << b << " is: " << result_ptr << std::endl;
    // Meaningful return type: double. We'd want to return the max of two `double` values.

    double result_ref = max_ref(a, b);
    std::cout << "Max of " << a << " and " << b << " is: " << result_ref << std::endl;
    // Meaningful return type: double. Still returning max of two `double` values.

    double result_const_ref = max_const_ref(a, b);
    std::cout << "Max of " << a << " and " << b << " is: " << result_const_ref << std::endl;
    // Still double.

    // max_ptr_to_ref is invalid.

    double result_ref_to_ptr = max_ref_to_ptr(pa, pb);
    std::cout << "Max of " << a << " and " << b << " is: " << result_ref_to_ptr << std::endl;
    // Still double.

    double result_const_ref_to_ptr = max(pa, pb);
    std::cout << "Max of " << a << " and " << b << " is: " << result_const_ref_to_ptr << std::endl;
    // Still double. Useful when you want to pass a pointer to double but don't want to allow modifications to the original value.

    return 0;
}