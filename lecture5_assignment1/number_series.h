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
        int _minimum{};
        int _maximum{};

        int get_min() const;

        int get_max() const;

        number_series(const int x);
        explicit number_series(const std::vector<int> &&ns);

        ~number_series();

        static number_series MakeRandom(int upper, int lower, size_t length);

        number_series operator+(const number_series &other);

        number_series &operator+=(const number_series &other);

        bool operator<(const number_series &other) const;

    private:
        std::vector<int> series{};

        void update_min_max();
        };
}

#endif