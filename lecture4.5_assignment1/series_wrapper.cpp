#include "series_wrapper.h"

namespace series
{
    series_wrapper &series_wrapper::operator=(const series_wrapper &other)
    {
        if (this != &other)
            _series = std::make_unique<number_series>(*other._series);

        return *this;
    }

    // Could have been = default;
    series_wrapper &series_wrapper::operator=(series_wrapper &&other)
    {
        if (this != &other)
            _series = std::move(other._series);

        return *this;
    }

    series_wrapper series_wrapper::make_random(int lower, int upper, size_t length)
    {
        series_wrapper sw;
        sw._series->make_random(upper, lower, length);

        return sw;
    }

    // Trivial methods are easier to maintain inline within a class.
    int series_wrapper::get_min() const
    {
        return _series->get_min();
    }

    int series_wrapper::get_max() const
    {
        return _series->get_max();
    }

    // Could have reused += operator:
    // auto *res = this;
    // res += other;
    // return res;
    series_wrapper series_wrapper::operator+(const series_wrapper &other)
    {
        series_wrapper sw{};
        sw._series = std::make_unique<number_series>(*_series + *other._series);

        return sw;
    }

    series_wrapper &series_wrapper::operator+=(const series_wrapper &other)
    {
        *_series += *other._series;

        return *this;
    }

    bool series_wrapper::operator<(const series_wrapper &other) const
    {
        return *_series < *other._series;
    }
}