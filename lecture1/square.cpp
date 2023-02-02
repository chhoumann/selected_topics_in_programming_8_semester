#include <iostream>
#include <vector>

using namespace std;

double square(double x) {
    return x * x;
}

void print_square(double x) {
    cout << "The square of " << x << " is " << square(x) << endl;
}

int main() {
    double b = 10.0;
    print_square(b);

    return 0;
}
