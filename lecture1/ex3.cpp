#include <iostream>
#include <algorithm>
#include <vector>
#include <array>

// Write a short function that gets an integer array as input and returns a copy of it, but sorted (use std::sort in your implementation).
// Consider the array as a C array as well as a vector<int>.
// Consider as many as possible different interfaces for the function -- different ways of getting the array out to the calling code. 


// Not possbile with C arrays. Can't return the copied array, as the lifetime runs out and it breaks.
int* c_arr_sort_arr_copy(int *arr)
{
    int n = sizeof(arr) / sizeof(arr[0]);
    int temp[n];

    for (auto i = 0; i < n; i++)
        temp[i] = arr[i];

    std::sort(temp, temp + n);

    return temp;
}

std::vector<int> cpp_vector_sort_arr_copy(int *arr)
{
    int n = sizeof(arr) / sizeof(arr[0]);
    std::vector<int> temp;

    temp.resize(n);

    for (auto i = 0; i < n; i++) {
        temp[i] = arr[i];
        std::cout << temp[i] << ":" << arr[i] << std::endl;
    }

    std::sort(temp.begin(), temp.end());

    return temp;
}

int main()
{
    int random_arr[5];
    std::cout << "Enter some numbers:" << std::endl;

    for (auto i = 0; i < 5; i++)
    {
        int temp = 0;
        std::cin >> temp;
        random_arr[i] = temp;
    }

    std::cout << "Unsorted:\n";
    for (int x : random_arr)
        std::cout << x;

    std::cout << std::endl;

    std::vector<int> sorted = cpp_vector_sort_arr_copy(random_arr);

    std::cout << "Sorted:\n";
    for (auto i = 0; i < 5; i++)
        std::cout << sorted[i];

    std::cout << std::endl;

}