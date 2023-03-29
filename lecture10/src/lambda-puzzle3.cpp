/**
 * Purpose: exercise lambda expressions with STL algorithms.
 * @author Marius Mikucionis <marius@cs.aau.dk>
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <iostream>
#include <vector>
#include <random>

static auto gen = std::mt19937{std::random_device{}()};
static auto dist = std::uniform_int_distribution<int>{0, 100};

void fill_with_data(std::vector<int>& data, size_t count)
{
    std::generate_n(std::back_inserter(data), count, []() { return dist(gen); });
}

template <typename T>
void print_data(std::ostream& os, const std::vector<T>& data)
{
    std::copy(data.begin(), data.end(), std::ostream_iterator<T>(os, " "));
    os << '\n';
}

int compute_sum(const std::vector<int>& data) { return std::accumulate(data.begin(), data.end(), 0); }

std::string concatenate(const std::vector<int>& data)
{
    return std::accumulate(data.begin(), data.end(), std::string{},
                           [](const std::string& a, int b) { return a + std::to_string(b); });
}

size_t odd_count(const std::vector<int>& data)
{
    return std::count_if(data.begin(), data.end(), [](int d) { return d % 2 == 1; });
}

std::vector<std::string> stringify(const std::vector<int>& data)
{
    std::vector<std::string> res;
    std::transform(data.begin(), data.end(), std::back_inserter(res), [](int d) { return std::to_string(d); });
    return res;
}

std::vector<std::string> compute_sorted(const std::vector<std::string>& data)
{
    auto res = data;
    std::sort(res.begin(), res.end(), std::greater<std::string>());
    return res;
}

std::vector<int> squares(const std::vector<int>& data)
{
    std::vector<int> res;
    std::transform(data.begin(), data.end(), std::back_inserter(res), [](int d) { return d * d; });
    return res;
}

TEST_CASE("Lambda puzzle3")
{
    auto data = std::vector<int>{};
    fill_with_data(data, 20);

    print_data(std::cout, data);

    SUBCASE("Test compute_sum") {
        std::vector<int> testData{1, 2, 3, 4, 5};
        CHECK(compute_sum(testData) == 15);
    }

    SUBCASE("Test concatenate") {
        std::vector<int> testData{1, 2, 3};
        CHECK(concatenate(testData) == "123");
    }

    SUBCASE("Test odd_count") {
        std::vector<int> testData{1, 2, 3, 4, 5};
        CHECK(odd_count(testData) == 3);
    }

    SUBCASE("Test stringify") {
        std::vector<int> testData{1, 2, 3};
        std::vector<std::string> expected{"1", "2", "3"};
        CHECK(stringify(testData) == expected);
    }

    SUBCASE("Test compute_sorted") {
        std::vector<std::string> testData{"1", "3", "2", "10"};
        std::vector<std::string> expected{"3", "2", "10", "1"};
        CHECK(compute_sorted(testData) == expected);
    }

    SUBCASE("Test squares") {
        std::vector<int> testData{1, 2, 3, 4, 5};
        std::vector<int> expected{1, 4, 9, 16, 25};
        CHECK(squares(testData) == expected);
        print_data(std::cout, squares(testData));
    }
}
