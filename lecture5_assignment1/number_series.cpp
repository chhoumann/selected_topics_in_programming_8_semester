#include "number_series.h"
#include <iterator>
#include <iostream>
#include <random>
#include <algorithm>
#include <functional>
#include <immintrin.h>

namespace series
{
    number_series::~number_series()
    {
        series.clear();
    }

    number_series::number_series() = default;

    number_series::number_series(const std::vector<int> &&ns) : series{std::move(ns)}
    {
        update_min_max();
    }

    number_series number_series::make_random(int lower, int upper, size_t length)
    {
        std::vector<int> series(length);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(lower, upper);

        std::generate(series.begin(), series.end(), [&gen, &dist]()
                      { return dist(gen); });

        return number_series{std::move(series)};
    }

    int number_series::get_min() const
    {
        return _minimum;
    }

    int number_series::get_max() const
    {
        return _maximum;
    }

    number_series number_series::operator+(const number_series &other)
    {
        if (series.size() != other.series.size())
            throw std::invalid_argument("Vectors must have the same size");

        std::vector<int> res(series.size());

        for (size_t i = 0; i < series.size(); ++i)
        {
            res[i] = series[i] + other.series[i];
        }

        return number_series{std::move(res)};
    }

    number_series &number_series::operator+=(const number_series &other)
    {
        if (series.size() != other.series.size())
            throw std::invalid_argument("Vectors must have the same size");

        for (size_t i = 0; i < series.size(); ++i)
            series[i] += other.series[i];

        update_min_max();

        return *this;
    }

    bool number_series::operator<(const number_series &other) const
    {
        return (get_max() - get_min()) < (other.get_max() - other.get_min());
    }

    void number_series::update_min_max()
    {
        auto res = std::minmax_element(series.begin(), series.end());

        _minimum = *res.first;
        _maximum = *res.second;
    }
}