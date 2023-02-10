#include <vector>
#include <iostream>
#include <algorithm>

template <typename T>
void forEach(const std::vector<T> &values, void (*func)(T))
{
    for (auto x : values)
        func(x);
}

int main()
{
    std::vector<int> values{1, 5, 4, 2, 3};
    auto it = std::find_if(values.begin(), values.end(), [](int value)
                           { return value > 3; });

    std::cout << *it << std::endl;

    auto printAll = [](int value)
    { std::cout << "Value: " << value << std::endl; };

    forEach<int>(values, printAll);
}