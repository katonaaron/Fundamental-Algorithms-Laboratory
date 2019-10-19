#include <iostream>
#include <vector>
#include "Profiler.h"

using namespace std;

enum SORTED_ORDER { RANDOM = 0, ASC, DESC };

//Configuration of the evalutaion
const int MIN_SIZE = 100;
const int MAX_SIZE = 10000;
const int STEPS = 100;
const int NR_OF_MEASUREMENTS_AVERAGE_CASE = 5;
const int RANGE_MIN = 10;
const int RANGE_MAX = 50000;

void PrintArray(int A[], int Size, string Message = "")
{
    for (int i = 0; i < Size; i++)
    {
        cout << A[i] << " ";
    }
    cout << "- " << Message << "\n";
}

void PrintHeap(int A[], int HeapSize, string Message = "")
{
    int height = (int)log2((double)HeapSize) + 1;
    bool first = true;

    cout << Message << "\n";

    for (int level = 1; level <= height; level++)
    {
        cout << string((1 << (height - level)) - 1, ' ');
        first = true;
        for (int i = (1 << (level - 1)) - 1; i < HeapSize && i < (1 << level) - 1; i++)
        {
            if (!first)
            {
                cout << string((1 << (height - level + 1)) - 1, ' ');
            }
            first = false;
            cout << A[i];
        }
        cout << "\n";
    }
    cout << "\n";
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

void MaxHeapify(int A[], int HeapSize, int i, Operation Op, bool Debug = false)
{
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
    Op.count(2); //2x Comp

    if (largest != i)
    {
        swap(A[i], A[largest]);
        Op.count(3);

        if (Debug)
            PrintHeap(A, HeapSize, to_string(A[largest]) + " swapped with " + to_string(A[i]));

        MaxHeapify(A, HeapSize, largest, Op, Debug);
    }
}

void BuildMaxHeapBottomUp(int A[], int ArraySize, Operation Op, bool Debug = false)
{
    int heapSize = ArraySize;
    int val;
    for (int i = (heapSize - 1) / 2; i >= 0; i--)
    {
        if (Debug)
            val = A[i];
        MaxHeapify(A, heapSize, i, Op);
        if (Debug)
            PrintHeap(A, heapSize, "MaxHeapify was called for " + to_string(val));
    }
}

void HeapSort(int A[], int ArraySize, Operation Op, bool Debug = false)
{
    BuildMaxHeapBottomUp(A, ArraySize, Op);
    if (Debug)
        PrintHeap(A, ArraySize, "BuildMaxHeap was called");
    for (int i = ArraySize - 1; i >= 1; i--)
    {
        swap(A[0], A[i]);
        Op.count(3);
        if (Debug)
            PrintHeap(A, ArraySize, to_string(A[i]) + " was swapped with " + to_string(A[0]));
        MaxHeapify(A, i, 0, Op);
        if (Debug)
            PrintHeap(A, ArraySize, "MaxHeapify was called for A[0]");
    }
}

void HeapInsert(int A[], int& HeapSize, int Key, Operation Op, bool Debug = false)
{
    A[HeapSize++] = Key;
    Op.count();
    if (Debug)
        PrintHeap(A, HeapSize, to_string(Key) + " was inserted to the end");
    int i = HeapSize - 1;
    while (i > 0 && A[Parent(i)] < A[i])
    {
        swap(A[Parent(i)], A[i]);
        if (Debug)
            PrintHeap(A, HeapSize, to_string(A[Parent(i)]) + " was swapped with " + to_string(A[i]));
        i = Parent(i);
        Op.count(4); //comp + swap
    }
    if (i > 0)
        Op.count(); //comp
}

void BuildMaxHeapTopDown(int A[], int ArraySize, Operation Op, bool Debug = false)
{
    int heapSize = 0;
    int keyToPrint;
    for (int i = 0; i < ArraySize; i++)
    {
        if (Debug)
            keyToPrint = A[i];
        HeapInsert(A, heapSize, A[i], Op);
        if (Debug)
            PrintHeap(A, heapSize, to_string(keyToPrint) + " was inserted");
    }
}

void Test()
{
    static int arr1[] = { 1, 10, 9, 8, 5, 4, 6, 7, 3, 2 },
        arr2[] = { 2, 9, 1, 0, 4, 6, 3, 8, 7, 5 },
        arr3[] = { 2, 9, 1, 0, 4, 6, 3, 8, 7, 5 },
        arr4[] = { 2, 9, 1, 0, 4, 6, 3, 8, 7, 5 },
        arr5[] = { 2, 5, 1, 0, 4, 6, 3, 8, 7, 9 };
    const int size = 10;
    Profiler profiler("Test");
    Operation op = profiler.createOperation("test", size);

    cout << "\t\t\tHeapify\n";
    PrintHeap(arr1, size, "Initial heap");
    MaxHeapify(arr1, size, 0, op, true);

    cout << "\t\t\tBuildMaxHeapBottomUp\n";
    PrintHeap(arr2, size, "Initial tree");
    BuildMaxHeapBottomUp(arr2, size, op, true);

    cout << "\t\t\tHeapSort using BuildMaxHeapBottomUp\n";
    PrintHeap(arr3, size, "Initial tree");
    HeapSort(arr3, size, op, true);

    cout << "\t\t\tHeapInsert\n";
    BuildMaxHeapBottomUp(arr4, size - 1, op);
    PrintHeap(arr4, size - 1, "Initial heap");
    int heapSize = size - 1;
    int key = 10;
    HeapInsert(arr4, heapSize, key, op, true);

    cout << "\t\t\tBuildMaxHeapTopDown\n";
    PrintHeap(arr5, size, "Initial tree");
    BuildMaxHeapTopDown(arr5, size, op, true);
}

void EvaluateHeapBuildingMethods()
{
    static int data[MAX_SIZE], dataCopy[MAX_SIZE];

    //Best Case
    Profiler profiler("Build-Heap");
    for (unsigned int size = MIN_SIZE; size <= MAX_SIZE; size += STEPS)
    {
        cout << "Evaluating best case for size: " << size << "\n";
        Operation totalBottomUp = profiler.createOperation("total_BuildMaxHeapBottomUp_best", size);
        Operation totalTopDown = profiler.createOperation("total_BuildMaxHeapTopDown_best", size);

        FillRandomArray(data, size, RANGE_MIN, RANGE_MAX, false, DESC);
        copy(begin(data), end(data), begin(dataCopy));

        cout << "\tBuildMaxHeapBottomUp\n";
        BuildMaxHeapBottomUp(data, size, totalBottomUp);

        cout << "\tBuildMaxHeapTopDown\n";
        BuildMaxHeapTopDown(dataCopy, size, totalTopDown);
    }
    profiler.createGroup("total_best", "total_BuildMaxHeapBottomUp_best", "total_BuildMaxHeapTopDown_best");

     //Worst Case
    for (unsigned int size = MIN_SIZE; size <= MAX_SIZE; size += STEPS)
    {
        cout << "Evaluating worst case for size: " << size << "\n";
        Operation totalBottomUp = profiler.createOperation("total_BuildMaxHeapBottomUp_worst", size);
        Operation totalTopDown = profiler.createOperation("total_BuildMaxHeapTopDown_worst", size);

        FillRandomArray(data, size, RANGE_MIN, RANGE_MAX, false, ASC);
        copy(begin(data), end(data), begin(dataCopy));

        cout << "\tBuildMaxHeapBottomUp\n";
        BuildMaxHeapBottomUp(data, size, totalBottomUp);

        cout << "\tBuildMaxHeapTopDown\n";
        BuildMaxHeapTopDown(dataCopy, size, totalTopDown);
    }
    profiler.createGroup("total_worst", "total_BuildMaxHeapBottomUp_worst", "total_BuildMaxHeapTopDown_worst");

    //Average Case
    for (int m = 1; m <= NR_OF_MEASUREMENTS_AVERAGE_CASE; m++)
        for (unsigned int size = MIN_SIZE; size <= MAX_SIZE; size += STEPS)
        {
            cout << "Evaluating average case for size: " << size << " - measurement: " << m << "\n";
            Operation totalBottomUp = profiler.createOperation("total_BuildMaxHeapBottomUp_average", size);
            Operation totalTopDown = profiler.createOperation("total_BuildMaxHeapTopDown_average", size);

            FillRandomArray(data, size, RANGE_MIN, RANGE_MAX, false, RANDOM);
            copy(begin(data), end(data), begin(dataCopy));

            cout << "\tBuildMaxHeapBottomUp\n";
            BuildMaxHeapBottomUp(data, size, totalBottomUp);

            cout << "\tBuildMaxHeapTopDown\n";
            BuildMaxHeapTopDown(dataCopy, size, totalTopDown);
        }
    profiler.divideValues("total_BuildMaxHeapBottomUp_average", NR_OF_MEASUREMENTS_AVERAGE_CASE);
    profiler.divideValues("total_BuildMaxHeapTopDown_average", NR_OF_MEASUREMENTS_AVERAGE_CASE);
    profiler.createGroup("total_average", "total_BuildMaxHeapBottomUp_average", "total_BuildMaxHeapTopDown_average");

    profiler.showReport();
}


int main()
{
    Test();
    //EvaluateHeapBuildingMethods();
    return 0;
}