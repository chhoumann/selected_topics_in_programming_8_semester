#ifndef NUMBERSERIES_H
#define NUMBERSERIES_H

#include <vector>
#include <iostream>
#include <algorithm>

namespace series
{
    class number_series
    {
    public:
        int get_min() const;

        int get_max() const;

        static number_series make_random(int upper, int lower, size_t length);

        number_series operator+(const number_series &other);

        number_series &operator+=(const number_series &other);

        bool operator<(const number_series &other) const;

    private:
        int averages[12]; // number_series sort just got slower
        std::vector<int> series;

        void update_min_max();

        int _minimum{};
        int _maximum{};
    };
}

#endif