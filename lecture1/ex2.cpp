#include <iostream>
#include <vector>
#include <utility>
#include <cstring>
#include <chrono>
using namespace std;
using namespace std::chrono;

void swap_ends(int arr[], int size)
{
    for (int i = size / 2 - 1, j = size - 1; i >= 0; --i, --j)
        swap(arr[i], arr[j]);
}

void swap_ends(vector<int> &arr)
{ // overloading the same fn-name
    for (int i = arr.size() / 2 - 1, j = arr.size() - 1; i >= 0; --i, --j)
        swap(arr[i], arr[j]);
}

constexpr int memcpy_arr_size = 10'000'00;

void swap_ends_static(int arr[], int size)
{
    static int buffer[memcpy_arr_size];

    memcpy(buffer, arr, size * sizeof(int));
    memcpy(arr, buffer + size / 2, size / 2 * sizeof(int));
    memcpy(arr + size / 2, buffer, size / 2 * sizeof(int));
}

void swap_ends_dynamic(int arr[], int size)
{
    int *temp = new int[size];

    // copy (size * int_size) bytes to temp from arr
    memcpy(temp, arr, size * sizeof(int));

    // copy second half of temp to first half of arr
    memcpy(arr, temp + size / 2, size / 2 * sizeof(int));

    // copy first half of temp to second half of arr
    memcpy(arr + size / 2, temp, size / 2 * sizeof(int));

    delete[] temp;
}

int c_arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

int main()
{
    constexpr int arr_size = 10'000'00;

    int c_arr[arr_size];
    vector<int> vec(arr_size);

    // Time C array swapping
    auto t0 = high_resolution_clock::now();
    swap_ends(c_arr, arr_size);
    auto t1 = high_resolution_clock::now();
    cout << duration<double, std::milli>(t1 - t0).count() << "ms" << endl;

    // Time vector swapping
    auto t2 = high_resolution_clock::now();
    swap_ends(vec);
    auto t3 = high_resolution_clock::now();
    cout << duration<double, std::milli>(t3 - t2).count() << "ms" << endl;

    // Time dynamic swapping
    auto t4 = high_resolution_clock::now();
    swap_ends_dynamic(c_arr, arr_size);
    auto t5 = high_resolution_clock::now();
    cout << duration<double, std::milli>(t5 - t4).count() << "ms" << endl;

    // Time static swapping
    auto t6 = high_resolution_clock::now();
    swap_ends_static(c_arr, arr_size);
    auto t7 = high_resolution_clock::now();
    cout << duration<double, std::milli>(t7 - t6).count() << "ms" << endl;
}