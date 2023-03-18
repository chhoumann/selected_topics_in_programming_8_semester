#ifndef TEMPLATES_POWER_HPP
#define TEMPLATES_POWER_HPP

#include <cstddef>

template <typename T>
T power(T number, std::size_t p)
{
	if (p == 0)
		return 1;
	if (p == 1)
		return number;
	T res = power(number, p/2);
	res *= res;
	if (p%2 == 1)
		res *= number;
	return res;
}

template <int base, int exponent>
struct Power {
    static constexpr int value = base * Power<base, exponent-1>::value;
};

template <int base>
struct Power<base, 0> {
    static constexpr int value = 1;
};

#endif	// TEMPLATES_POWER_HPP
