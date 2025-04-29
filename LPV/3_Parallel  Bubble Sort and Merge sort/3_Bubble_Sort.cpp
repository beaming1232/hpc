#include <iostream>
#include <cstdlib>
#include <omp.h>
using namespace std;

void bubble(int *, int);
void swap(int &, int &);

void bubble(int *a, int n)
{
    for (int i = 0; i < n; i++)
    {
        int first = i % 2;

#pragma omp parallel for shared(a, first)
        for (int j = first; j < n - 1; j += 2)
        {
            if (a[j] > a[j + 1])
            {
                swap(a[j], a[j + 1]);
            }
        }
    }
}

void swap(int &a, int &b)
{
    int temp;
    temp = a;
    a = b;
    b = temp;
}

int main()
{
    // cout << "\n\nName: Shriharsh Deshmukh\nRoll No.62 \t Div.A\n\n";
    int *a, n;
    cout << "\nEnter total number of elements: ";
    cin >> n;
    a = new int[n];
    cout << "\nEnter elements: ";
    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
    }

    bubble(a, n);

    cout << "\nSorted array is:\n";
    for (int i = 0; i < n; i++)
    {
        cout << a[i] << " ";
    }
    cout << endl;

    delete[] a;
    return 0;
}

// Run Commands (Ubuntu Terminal):
// g++ -fopenmp  3_Bubble_Sort.cpp -o bubble_sort
//  ./bubble_sort

// Run Commands (VS Code):
// g++ -fopenmp -o bubble_sort 3_Bubble_Sort.cpp
// .\bubble_sort