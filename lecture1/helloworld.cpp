#include <iostream>
using namespace std;

class Vector
{
public:
    Vector(int s) : elem{new double[s]}, sz{s} {} // construct Vector
    double &operator[](int i) { return elem[i]; } // element access: subscripting
    int size() { return sz; }
    int one();

private:
    double *
        elem; // pointer to elements
    int sz;   // number of elements
};

int Vector::one()
{
    return 1;
}

main()
{
    cout << "Hello, World!" << endl;

    char v[6]{'a', 'b', 'c', 'd', 'e', 'f'};
    char *p = &v[3];
    char x = *p;

    Vector vec(6);
    cout << vec.one() << endl;

    return 0;
}