#ifndef SERIESWRAPPER_H
#define SERIESWRAPPER_H

#include <memory>
#include "number_series.h"

namespace series
{
    class series_wrapper
    {
    public:
        series_wrapper() : _series(std::make_unique<number_series>()){};
        series_wrapper(const series_wrapper &other) : _series(std::make_unique<number_series>(*other._series)){};
        series_wrapper(const series_wrapper &&other) : _series(std::make_unique<number_series>(std::move(other._series))){};

        series_wrapper &operator=(const series_wrapper &other);
        series_wrapper &operator=(series_wrapper &&other);

        static series_wrapper make_random(int lower, int upper, size_t length);

        int get_min() const;
        int get_max() const;

        series_wrapper operator+(const series_wrapper &other);
        series_wrapper &operator+=(const series_wrapper &other);

        bool operator<(const series_wrapper &other) const;

    private:
        std::unique_ptr<number_series> _series;
    };
}

#endif