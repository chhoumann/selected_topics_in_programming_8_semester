// Create a union type containing float, unsigned int and char[4]. Set the integer value and print out the character array one number at a time. Does your computer use big-endian (most significant byte first) or little-endian (least significant byte first) layout?
// Use the same union type to reinterpret floating point number as unsigned int: set the floating point value to 1, 2, 4, 8 and observe how the integer value changes. Use std::bitset to print the content of the unsigned integer bit-by-bit. Which bits are used for mantisa and which for exponent of the floating point number?
#include <iostream>
#include <bitset>
#include <cstring>

union FlatIntCharArray
{
    float f;
    unsigned int i;
    char c[4];
};

int main() {
    FlatIntCharArray data;

    data.i = 0x12345678;

    std::cout << "Integar value: " << data.i << std::endl;
    std::cout << "Character array: ";

    // Print out the character array one number at a time
    for (auto i = 0; i < 4; i++) {
        std::cout << (int)data.c[i] << " ";
    }

    std::cout << std::endl;

    // Check endianness
    if (data.c[0] == 0x78) {
        std::cout << "Little-endian" << std::endl;
    } else {
        std::cout << "Big-endian" << std::endl;
    }

    // Set floating point values and observe the integer value
    data.f = 1;
    std::cout << "Floating point value: " << data.f << " Integer value: " << data.i << std::endl;
    
    data.f = 2;
    std::cout << "Floating point value: " << data.f << " Integer value: " << data.i << std::endl;
    
    data.f = 4;
    std::cout << "Floating point value: " << data.f << " Integer value: " << data.i << std::endl;
    
    data.f = 8;
    std::cout << "Floating point value: " << data.f << " Integer value: " << data.i << std::endl;
    
    // Print the integer value bit-by-bit
    std::bitset<32> bits(data.i);
    std::cout << "Bits: " << bits << std::endl;

    return 0;
}