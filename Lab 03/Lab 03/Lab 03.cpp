#include <iostream>
#include <string>
#include <ctime>
#include "Profiler.h"

using namespace std;

void PrintArray(int A[], int Size, string Message = "")
{
    for (int i = 0; i < Size; i++)
    {
        cout << A[i] << " ";
    }
    cout << "- " << Message << "\n";
}

int Partition(int A[], int p, int r)
{
    int x = A[r];
    int i = p - 1;
    for (int j = p; j < r; j++)
    {
        if (A[j] <= x)
        {
            i++;
            swap(A[i], A[j]);
        }
    }
    swap(A[i + 1], A[r]);
    return i + 1;
}

int RandomizedPartition(int A[], int p, int r)
{
    srand(time(0));
    int i = rand() % (r - p + 1) + p;
    swap(A[i], A[r]);
    return Partition(A, p, r);
}

void QuickSort(int A[], int p, int r)
{
    if (p < r)
    {
        //int q = Partition(A, p, r);
        int q = RandomizedPartition(A, p, r);
        PrintArray(A + p, r - p + 1, "Partitioned for p = " + to_string(p) + ", r = " + to_string(r) + ", q = " + to_string(q));
        QuickSort(A, p, q - 1);
        QuickSort(A, q + 1, r);
    }
}

void Test()
{
    static int arr1[] = { 11, 10, 9, 8, 5, 4, 6, 7, 3, 2 },
        arr2[] = { 2, 9, 1, 0, 4, 6, 3, 8, 7, 5 },
        arr3[] = { 2, 9, 1, 0, 4, 6, 3, 8, 7, 5 },
        arr4[] = { 2, 9, 1, 0, 4, 6, 3, 8, 7, 5 },
        arr5[] = { 2, 5, 1, 0, 4, 6, 3, 8, 7, 9 };
    const int size = 10;

    Profiler profiler("Test");
    Operation op = profiler.createOperation("test", size);

    cout << "\t\t\QuickSort\n";
    PrintArray(arr1, size, "Initial array");
    QuickSort(arr1, 0, size - 1);
    PrintArray(arr1, size, "Array after QuickSort");
}

int main()
{
    Test();
    return 0;
}