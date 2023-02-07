// Experiment with build-in fundamental types of C++: bool, char (and possibly other versions of it), all integer types, all floating-point types. Print their sizes in bytes on your machine, using sizeof (note that sizeof returns in multiples of the size of char). For numeric types (including char) establish the range of possible values using numeric_limits from <limits> (see Section 5.6.5).
#include <iostream>
#include <limits>

using namespace std;

int main()
{
    std::cout << "bool size: " << sizeof(bool) << " bytes" << std::endl;
    std::cout << "char size: " << sizeof(char) << " bytes" << std::endl;
    std::cout << "signed char size: " << sizeof(signed char) << " bytes" << std::endl;
    std::cout << "unsigned char size: " << sizeof(unsigned char) << " bytes" << std::endl;
    std::cout << "wchar_t size: " << sizeof(wchar_t) << " bytes" << std::endl;
    std::cout << "short int size: " << sizeof(short int) << " bytes" << std::endl;
    std::cout << "unsigned short int size: " << sizeof(unsigned short int) << " bytes" << std::endl;
    std::cout << "int size: " << sizeof(int) << " bytes" << std::endl;
    std::cout << "unsigned int size: " << sizeof(unsigned int) << " bytes" << std::endl;
    std::cout << "long int size: " << sizeof(long int) << " bytes" << std::endl;
    std::cout << "unsigned long int size: " << sizeof(unsigned long int) << " bytes" << std::endl;
    std::cout << "long long int size: " << sizeof(long long int) << " bytes" << std::endl;
    std::cout << "unsigned long long int size: " << sizeof(unsigned long long int) << " bytes" << std::endl;
    std::cout << "float size: " << sizeof(float) << " bytes" << std::endl;
    std::cout << "double size: " << sizeof(double) << " bytes" << std::endl;
    std::cout << "long double size: " << sizeof(long double) << " bytes" << std::endl;

    std::cout << "char range: [" << std::numeric_limits<char>::min() << ", " << std::numeric_limits<char>::max() << "]" << std::endl;
    std::cout << "signed char range: [" << std::numeric_limits<signed char>::min() << ", " << std::numeric_limits<signed char>::max() << "]" << std::endl;
    std::cout << "unsigned char range: [" << std::numeric_limits<unsigned char>::min() << ", " << std::numeric_limits<unsigned char>::max() << "]" << std::endl;
    std::cout << "short int range: [" << std::numeric_limits<short int>::min() << ", " << std::numeric_limits<short int>::max() << "]" << std::endl;
    std::cout << "unsigned short int range: [" << std::numeric_limits<unsigned short int>::min() << ", " << std::numeric_limits<unsigned short int>::max() << "]" << std::endl;
    std::cout << "int range: [" << std::numeric_limits<int>::min() << ", " << std::numeric_limits<int>::max() << "]" << std::endl;
    std::cout << "unsigned int range: [" << std::numeric_limits<unsigned int>::min() << ", " << std::numeric_limits<unsigned int>::max() << "]" << std::endl;
    std::cout << "long int range: [" << std::numeric_limits<long int>::min() << ", " << std::numeric_limits<long int>::max() << "]" << std::endl;
    std::cout << "unsigned long int range: [" << std::numeric_limits<unsigned long int>::min() << ", " << std::numeric_limits<unsigned long int>::max() << "]" << std::endl;
    std::cout << "long long int range: [" << std::numeric_limits<long long int>::min() << ", " << std::numeric_limits<long long int>::max() << "]" << std::endl;
    std::cout << "unsigned long long int range: [" << std::numeric_limits<unsigned long long int>::min() << ", " << std::numeric_limits<unsigned long long int>::max() << "]" << std::endl;
    std::cout << "float range: [" << std::numeric_limits<float>::min() << ", " << std::numeric_limits<float>::max() << "]" << std::endl;
    std::cout << "double range: [" << std::numeric_limits<double>::min() << ", " << std::numeric_limits<double>::max() << "]" << std::endl;
    std::cout << "long double range: [" << std::numeric_limits<long double>::min() << ", " << std::numeric_limits<long double>::max() << "]" << std::endl;

    return 0;
}