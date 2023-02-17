#include "number_series.h"
#include <iterator>
#include <iostream>
#include <random>
#include <algorithm>
#include <functional>
#include <immintrin.h>

namespace series
{
    number_series number_series::make_random(int lower, int upper, size_t length)
    {
        number_series ns{};
        ns.series.reserve(length);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(lower, upper);

        std::generate(ns.series.begin(), ns.series.end(), [&gen, &dist]()
                      { return dist(gen); });

        return ns;
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

        number_series ns{};
        ns.series.reserve(series.size());

        for (size_t i = 0; i < series.size(); ++i)
        {
            ns.series.push_back(series[i] + other.series[i]);
        }

        return ns;
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
        _minimum = 0;
        _maximum = 0;

        for (const auto &x : series)
        {
            if (x < _minimum)
                _minimum = x;

            if (x < _maximum)
                _maximum = x;
        }
    }
}