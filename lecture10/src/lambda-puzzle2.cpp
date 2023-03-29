/**
 * Purpose: debug lambda expressions.
 * @author Marius Mikucionis <marius@cs.aau.dk>
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <functional>  // function
#include <algorithm>   // all_of

using filters_t = std::vector<std::function<bool(int)>>;

int divisor;

void add_divisor_filter(filters_t& container, int divisor)
{
    container.emplace_back([=](int value) { return value % divisor == 0; });
}

TEST_CASE("Lambda puzzle2")
{
    auto filters = filters_t{};
    divisor = 3;
    add_divisor_filter(filters, divisor);
    divisor = 5;
    add_divisor_filter(filters, divisor);

    auto os = std::ostringstream{};
    for (auto i = 0; i < 100; ++i) {
        if (std::all_of(std::begin(filters), std::end(filters), [&](auto& f) { return f(i); })) {
            os << i << " ";
        }
    }
    CHECK(os.str() == "0 15 30 45 60 75 90 ");
}
