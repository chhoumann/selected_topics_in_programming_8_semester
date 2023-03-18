#ifndef TEMPLATES_FIBONACCI_HPP
#define TEMPLATES_FIBONACCI_HPP

#include <cstddef> // size_t

template <typename T = std::size_t>
constexpr T fib(std::size_t i)
{
	if (i < 2u)
		return T{1};
	else
		return fib<T>(i-1) + fib<T>(i-2);
}

template <std::size_t N>
struct Fib
{
    static constexpr std::size_t value = Fib<N - 1>::value + Fib<N - 2>::value;
};

template <>
struct Fib<0>
{
    static constexpr std::size_t value = 1;
};

template <>
struct Fib<1>
{
    static constexpr std::size_t value = 1;
};

#endif	// TEMPLATES_FIBONACCI_HPP
