#include <vector>
#include <iostream>
#include <typeinfo>

void print(std::vector<int> v)
{
    for (int i = 0; i < v.size(); i++)
    {
        std::cout << v[i] << " ";
    }

    std::cout << std::endl;
}

// std::ostream &operator<<(std::ostream &os, const std::vector<int> &v)
// {
//     for (int i = 0; i < v.size(); i++)
//     {
//         os << v[i] << " ";
//     }

//     return os;
// }

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &v)
{
    for (int i = 0; i < v.size(); i++)
    {
        os << v[i] << " ";
    }

    return os;
}

template <typename T>
class TD;

template<typename T>
void type_displayer(const T&) {
    std::cout << typeid(T).name() << std::endl;
}

int main()
{
    std::vector<int> vec = {1, 2, 3, 4, 5};
    print(vec);
    std::cout << vec << std::endl;

    // TD<decltype(vec)> td;
    // type_displayer(vec);
}