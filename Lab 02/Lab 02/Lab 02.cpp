#include <iostream>
#include <vector>
#include "Profiler.h"

using namespace std;

const int MIN_SIZE = 100;
const int MAX_SIZE = 10000;
const int STEPS = 100;
const int NR_OF_MEASUREMENTS_AVERAGE_CASE = 5;
const int RANGE_MIN = 1;// 10;
const int RANGE_MAX = 10;// 50000;

void PrintArray(int A[], int Size, string Message = "")
{
    for (unsigned i = 0; i < Size; i++)
    {
        cout << A[i] << " ";
    }
    cout << "- " << Message << "\n";
}

inline int Parent(int index)
{
    return (index - 1) / 2;
}

inline int Left(int index)
{
    return index * 2 + 1;
}

inline int Right(int index)
{
    return index * 2 + 2;
}

void MaxHeapify(int A[], int HeapSize, int i)
{
    cout << "MaxHeapify for HeapSize = " << HeapSize << ", i = " << i << "\n";
    int largest = i;
    int left = Left(i);
    int right = Right(i);

    if (left < HeapSize && A[left] > A[largest])
    {
        largest = left;
    }
    if (right < HeapSize && A[right] > A[largest])
    {
        largest = right;
    }

    if (largest != i)
    {
        swap(A[i], A[largest]);

        PrintArray(A, HeapSize, to_string(A[largest]) + " swapped with " + to_string(A[i]));
        MaxHeapify(A, HeapSize, largest);
    }
    else
    {
        PrintArray(A, HeapSize, "no swap");
    }
}

void BuildMaxHeap(int A[], int ArraySize)
{
    cout << "BuildMaxHeap for ArraySize = " << ArraySize << "\n";
    int heapSize = ArraySize;
    for (int i = (heapSize - 1) / 2; i >= 0; i--)
    {
        MaxHeapify(A, heapSize, i);
    }
}

void HeapSort(int A[], int ArraySize)
{
    BuildMaxHeap(A, ArraySize);
    cout << "S:";
    PrintArray(A, ArraySize);
    cout << "\n";
    for (int i = ArraySize - 1; i >= 1; i--)
    {
        swap(A[0], A[i]);
        MaxHeapify(A, i, 0);
        cout << "S:";
        PrintArray(A, ArraySize);
        cout << "\n";
    }
}

int HeapMaximum(int A[], int ArraySize)
{
    return A[0];
}

void Test()
{
    static int a[MAX_SIZE];
    const int size = 10;

    FillRandomArray(a, size, RANGE_MIN, RANGE_MAX, true, 0);
    cout << "I:";
    PrintArray(a, size);
    //BuildMaxHeap(a, size);
    HeapSort(a, size);
}

int main()
{

    Test();
    return 0;
}