/**
 * @author Katona Aron
 * @group 30423
 *
 * Problem specification:
 *		 Implement correctly and efficiently Quicksort and QuickSelect (RandomizedSelect). Analyze the complexity of Quicksort and Heapsort comparatively.
 *
 * Efficiency:
 *  Denote n as the size of the array.
 *
 *  1. Heapsort:
 *      Heapsort is implemented using the Bottom-Up heap building procedure, which has an O(n) complexity for any input, and it is only called once. The MaxHeapify procedure is 
 *      worst case O(log n), and it is called n - 1 times. Thus the worst case complexity of the Heapsort is O(n) + (n - 1) * O(log n) == O(n) + O(n * log n) == O(n * log n).
 *      Heapsort uses O(1) auxiliary space and its worst case complexity O(n * log n) == Omega(n * log n), thus heapsort is an optimal algorithm.
 *  2. Quicksort:
 *      Quicksort is a divide and conquer algorithm, that "divides" the problem using a partitioning algorithm, "conquers" with two recursive calls, and does not "combine". The
 *      partitioning algorithm divides the problem into two sub-problems by choosing a pivot and placing the elements that are smaller or equal than the pivot on its left side,
 *      and the other elements on its right side. It is implemented in two ways: by choosing the last element as pivot or by choosing a random pivot. In both
 *
 *      2.1 Choosing the last element as pivot:
 *          The worst case occurs when the array is partitioned into two partitions having n-1 and 0 sizes. This happens if the chosen pivot is the largest or the smallest element
 *          in the array. In this case the running time is T(n) = T(n - 1) + T(0) + O(n) = T(n) + O(n) = O(n^2).
 *          The best case is when the array is split such that each partition has no more that n/2 elements. This way the running time is T(n) = 2T(n/2) + O(n). By applying the 
 *          master theorem we get that the average case complexity is O(n * log n).
 *          In the average case, the running time is close to the best case, thus it is O(n * log n) on the average case.
 *      2.2 Choosing a random pivot:
 *          By choosing a random pivot the worst case running time is also O(n^2), but it has a so small probability, that the worst case expected running time is O(n log n), thus 
 *          the worst case is avoided. Form the charts we can deduce that the average case performance is the same as in the previous implementation.
 *
 *      Quicksort uses O(log n) additional space by having the recursive calls. The first implementation has O(n^2) worst case complexity so it is not optimal. The second implementation
 *      has O(n * log n) expected worst case running time with a very high probability, so it can be considered as an optimal algorithm. Another implementation of the Quicksort
 *      algorithm is to use the median as the pivot. This way is the worst case is eliminated. Finding the median is O(n) thus this method is O(n * log n) in the worst case. Thus if 
 *      logarithmic additional space is allowed, quicksort can be implemented to be an optimal sorting algorithm either by choosing a random pivot, or having the median as the pivot.
 *
        The quicksort can be improved additionally by applying a direct sorting method if the input data size is small. I compared the two quicksort implementations with insertion sort
        and found that in the average for n < 18 the running time of insertion sort is smaller than the running time of both implementations.

 * Stability:
 *  Neither the Heapsort, nor these implementations of the Quicksort are stable. Examples: Heapsort: A = {4, 2, 2, 1, 3, 0}, QuickSort: A = {5, 5, 2}
 *
 * Comparison of Heapsort and Quicksort:
 *  Heapsort and Quicksort are both comparison based in-place sorting algorithms, having O(n * log n) running time in the average case. But the multiplicative constant of the heapsort,
    which is ignored by the O notation, is greater than the multiplicative constant of the Quicksort. Thus in the average Quicksort is faster. But advantage of heapsort is that it 
    guarantees an O(n * log n) worst case running time, and it uses O(1) additional space. 
 */

#include <iostream>
#include <string>
#include <ctime>

#include "Profiler.h"
#include <cassert>

using namespace std;

enum SORTED_ORDER { RANDOM = 0, ASC, DESC };

//Configuration of the evaluation
const int MIN_SIZE = 100;
const int MAX_SIZE = 10000;
const int STEPS = 100;
const int NR_OF_MEASUREMENTS_AVERAGE_CASE = 5;
const int RANGE_MIN = 10;
const int RANGE_MAX = 50000;

#define DEMO

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

void MaxHeapify(int A[], int HeapSize, int i, Operation Op)
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

        MaxHeapify(A, HeapSize, largest, Op);
    }
}

void BuildMaxHeapBottomUp(int A[], int ArraySize, Operation Op)
{
    int heapSize = ArraySize;
    for (int i = (heapSize - 1) / 2; i >= 0; i--)
    {
        MaxHeapify(A, heapSize, i, Op);
    }
}

void HeapSort(int A[], int ArraySize, Operation Op)
{
    BuildMaxHeapBottomUp(A, ArraySize, Op);
#ifdef DEMO
    PrintHeap(A, ArraySize, "BuildMaxHeap was called");
#endif // DEMO    

    for (int i = ArraySize - 1; i >= 1; i--)
    {
        swap(A[0], A[i]);
        Op.count(3);
#ifdef DEMO
        PrintHeap(A, ArraySize, to_string(A[i]) + " was swapped with " + to_string(A[0]));
#endif // DEMO      

        MaxHeapify(A, i, 0, Op);
#ifdef DEMO
        PrintHeap(A, ArraySize, "MaxHeapify was called for A[0]");
#endif // DEMO    
    }
}

void InsertionSort(int A[], int Size, Operation Ass, Operation Comp)
{
    int key, j;
    for (int i = 1; i < Size; i++)
    {
        key = A[i];
        Ass.count();
        j = i - 1;
        while (j >= 0 && A[j] > key)
        {
            Comp.count();
            A[j + 1] = A[j];
            Ass.count();
            j--;
        }
        if (j >= 0)
            Comp.count();

        A[j + 1] = key;
        Ass.count();
    }
}

typedef int (*PartitionFunction)(int A[], int p, int r, Operation Op);

inline int Size(int start, int end)
{
    return end - start + 1;
}


int Partition(int A[], int p, int r, Operation Op)
{
    int x = A[r];
    Op.count();
    int i = p - 1;
    for (int j = p; j < r; j++)
    {
        Op.count();
        if (A[j] <= x)
        {
            i++;
            swap(A[i], A[j]);
            Op.count(3);
        }
    }
    swap(A[i + 1], A[r]);
    Op.count(3);
    return i + 1;
}

int RandomizedPartition(int A[], int p, int r, Operation Op)
{
    srand(time(0));
    int i = rand() % (r - p + 1) + p;
    swap(A[i], A[r]);
    Op.count(3);
    return Partition(A, p, r, Op);
}

int RandomizedSelect(int A[], int p, int r, int i, Operation Op)
{
    if (p == r)
        return A[p];
    int q = RandomizedPartition(A, p, r, Op);

#ifdef DEMO
    PrintArray(A + p, r - p + 1, "RandomizedSelect: Partitioned for p = " + to_string(p) + ", r = " + to_string(r) + ", q = " + to_string(q) + ", i = " + to_string(i));
#endif // DEMO

    int k = q - p + 1;
    if (i == k)
        return A[q];
    if (i < k)
        return RandomizedSelect(A, p, q - 1, i, Op);
    return RandomizedSelect(A, q + 1, r, i - k, Op);
}

int BestCasePartition(int A[], int p, int r, Operation Op)
{
    Profiler profiler("dummy");
    Operation dummy = profiler.createOperation("dummy", 0);

    int size = Size(p, r);
    int q = (size - 1) / 2 + p;
    RandomizedSelect(A, p, r, q - p + 1, dummy);
    swap(A[q], A[r]);
    return Partition(A, p, r, Op);
}


void QuickSort(int A[], int p, int r, Operation Op, PartitionFunction Partition)
{
    const int MinSizeToApplyDirectSort = 18;
    int size = Size(p, r);

#ifndef DEMO
    if (size <= MinSizeToApplyDirectSort)
        InsertionSort(A + p, size, Op, Op);
    else
    {
#endif // !DEMO
        int q = Partition(A, p, r, Op);
#ifdef DEMO
        PrintArray(A + p, size, "Partitioned for p = " + to_string(p) + ", r = " + to_string(r) + ", q = " + to_string(q));
#endif // !DEMO
        if (p < q - 1)
            QuickSort(A, p, q - 1, Op, Partition);
        if (q + 1 < r)
            QuickSort(A, q + 1, r, Op, Partition);
#ifndef DEMO
    }
#endif // !DEMO
}

void Demo()
{
    static int arr1[] = { 2, 9, 1, 0, 4, 6, 3, 8, 7, 5 },
        arr2[] = { 2, 9, 1, 0, 4, 6, 3, 8, 7, 5 }, //{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
        arr3[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }, //{ 2, 9, 1, 0, 4, 6, 3, 8, 7, 5 },
        arr4[] = { 2, 9, 1, 0, 4, 6, 3, 8, 7, 5 },
        arr5[] = { 2, 5, 1, 0, 4, 6, 3, 8, 7, 9 },
        arr6[] = { 2, 5, 1, 0, 4, 6, 3, 8, 7, 9, 10 };
    int size = 10;

    Profiler profiler("Demo");
    Operation op = profiler.createOperation("dummy", 0);

    cout << "\t\tHeapSort\n";
    PrintHeap(arr1, size, "Initial tree");
    HeapSort(arr1, size, op);
    PrintArray(arr1, size, "Array after HeapSort");

    cout << "\t\tQuickSort\n";
    PrintArray(arr2, size, "Initial array");
    QuickSort(arr2, 0, size - 1, op, Partition);
    PrintArray(arr2, size, "Array after QuickSort");

    cout << "\t\tRandomized QuickSort\n";
    PrintArray(arr3, size, "Initial array");
    QuickSort(arr3, 0, size - 1, op, RandomizedPartition);
    PrintArray(arr3, size, "Array after Randomized QuickSort");

    cout << "\t\tBest Case QuickSort\n";
    PrintArray(arr4, size, "Initial array");
    QuickSort(arr4, 0, size - 1, op, BestCasePartition);
    PrintArray(arr4, size, "Array after Best Case QuickSort");

    cout << "\t\tRandomizedSelect\n";
    PrintArray(arr5, size, "Initial array");
    int q = (size - 1) / 2;
    int res = RandomizedSelect(arr5, 0, size - 1, q + 1, op);
    PrintArray(arr5, size, "Array after RandomizedSelect");
    cout << "Result of Randomized select: " << res << ", q = " << q << "\n";

    size = sizeof(arr6) / sizeof(arr6[0]);
    cout << "\t\tRandomizedSelect\n";
    PrintArray(arr6, size, "Initial array");
    q = (size - 1) / 2;
    res = RandomizedSelect(arr6, 0, size - 1, q + 1, op);
    PrintArray(arr6, size, "Array after RandomizedSelect");
    cout << "Result of Randomized select: " << res << ", q = " << q << "\n";
}

void EvaluateSortingMethods()
{
    static int data[MAX_SIZE], dataCopy[MAX_SIZE], sorted[MAX_SIZE];

    //Best Case
    Profiler profiler("Sorting-Algorithms");
    for (unsigned int size = MIN_SIZE; size <= MAX_SIZE; size += STEPS)
    {
        cout << "Evaluating best case for size: " << size << "\n";
        Operation total = profiler.createOperation("total_QuickSort_best", size);

        FillRandomArray(data, size, RANGE_MIN, RANGE_MAX, false, RANDOM);
        copy(begin(data), end(data), begin(sorted));
        sort(sorted, sorted + size);

        cout << "\tQuickSort\n";
        QuickSort(data, 0, size - 1, total, BestCasePartition);
        assert(memcmp(data, sorted, size * sizeof(int)) == 0);
    }

    //Worst Case
    for (unsigned int size = MIN_SIZE; size <= MAX_SIZE; size += STEPS)
    {
        cout << "Evaluating worst case for size: " << size << "\n";
        Operation totalQS = profiler.createOperation("total_QuickSort_worst", size);

        FillRandomArray(data, size, RANGE_MIN, RANGE_MAX, false, DESC);
        copy(begin(data), end(data), begin(sorted));
        sort(sorted, sorted + size);

        cout << "\tQuickSort\n";
        QuickSort(data, 0, size - 1, totalQS, Partition);
        assert(memcmp(data, sorted, size * sizeof(int)) == 0);
    }

    //Average Case
    for (int m = 1; m <= NR_OF_MEASUREMENTS_AVERAGE_CASE; m++)
        for (unsigned int size = MIN_SIZE; size <= MAX_SIZE; size += STEPS)
        {
            cout << "Evaluating average case for size: " << size << " - measurement: " << m << "\n";
            Operation totalQS = profiler.createOperation("total_QuickSort_average", size);
            Operation totalRQS = profiler.createOperation("total_RandomizedQuickSort_average", size);
            Operation totalHeapSort = profiler.createOperation("total_HeapSort_average", size);

            FillRandomArray(data, size, RANGE_MIN, RANGE_MAX, false, RANDOM);
            copy(begin(data), end(data), begin(dataCopy));
            copy(begin(data), end(data), begin(sorted));
            sort(sorted, sorted + size);

            cout << "\tQuickSort\n";
            QuickSort(data, 0, size - 1, totalQS, Partition);
            assert(memcmp(data, sorted, size * sizeof(int)) == 0);

            cout << "\tRandomizedQuickSort\n";
            copy(begin(dataCopy), end(dataCopy), begin(data));
            QuickSort(data, 0, size - 1, totalRQS, RandomizedPartition);
            assert(memcmp(data, sorted, size * sizeof(int)) == 0);

            cout << "\tHeapSort\n";
            copy(begin(dataCopy), end(dataCopy), begin(data));
            HeapSort(data, size, totalHeapSort);
            assert(memcmp(data, sorted, size * sizeof(int)) == 0);
        }
    profiler.divideValues("total_QuickSort_average", NR_OF_MEASUREMENTS_AVERAGE_CASE);
    profiler.divideValues("total_RandomizedQuickSort_average", NR_OF_MEASUREMENTS_AVERAGE_CASE);
    profiler.divideValues("total_HeapSort_average", NR_OF_MEASUREMENTS_AVERAGE_CASE);
    profiler.createGroup("total_average", "total_QuickSort_average", "total_HeapSort_average", "total_RandomizedQuickSort_average");
    profiler.createGroup("total_QuickSort", "total_QuickSort_average", "total_QuickSort_best", "total_RandomizedQuickSort_average");

    profiler.showReport();
}

void FindDelta()
{
    const int min_size = 1;
    const int max_size = 30;
    const int steps = 1;
    const int nr_of_measurements_average_case = 5;
    const int range_min = 10;
    const int range_max = 50000;

    static int data[max_size], dataCopy[max_size], sorted[max_size];

    //ASC
    Profiler profiler("Finding Delta");
    for (unsigned int size = min_size; size <= max_size; size += steps)
    {
        cout << "Evaluating for sorted input of size: " << size << "\n";
        Operation totalQS = profiler.createOperation("total_QuickSort_ASC", size);
        Operation totalRQS = profiler.createOperation("total_RandomizedQuickSort_ASC", size);
        Operation totalIS = profiler.createOperation("total_InsertionSort_ASC", size);


        FillRandomArray(data, size, range_min, range_max, false, ASC);
        copy(begin(data), end(data), begin(dataCopy));
        copy(begin(data), end(data), begin(sorted));
        sort(sorted, sorted + size);

        cout << "\tQuickSort\n";
        QuickSort(data, 0, size - 1, totalQS, Partition);
        assert(memcmp(data, sorted, size * sizeof(int)) == 0);

        cout << "\tRandomizedQuickSort\n";
        copy(begin(dataCopy), end(dataCopy), begin(data));
        QuickSort(data, 0, size - 1, totalRQS, RandomizedPartition);
        assert(memcmp(data, sorted, size * sizeof(int)) == 0);

        cout << "\tInsertionSort\n";
        copy(begin(dataCopy), end(dataCopy), begin(data));
        InsertionSort(data, size, totalIS, totalIS);
        assert(memcmp(data, sorted, size * sizeof(int)) == 0);
    }
    profiler.createGroup("total_ASC", "total_QuickSort_ASC", "total_RandomizedQuickSort_ASC", "total_InsertionSort_ASC");

    //DESC
    for (unsigned int size = min_size; size <= max_size; size += steps)
    {
        cout << "Evaluating for inversely sorted input of size: " << size << "\n";
        Operation totalQS = profiler.createOperation("total_QuickSort_DESC", size);
        Operation totalRQS = profiler.createOperation("total_RandomizedQuickSort_DESC", size);
        Operation totalIS = profiler.createOperation("total_InsertionSort_DESC", size);


        FillRandomArray(data, size, range_min, range_max, false, DESC);
        copy(begin(data), end(data), begin(dataCopy));
        copy(begin(data), end(data), begin(sorted));
        sort(sorted, sorted + size);

        cout << "\tQuickSort\n";
        QuickSort(data, 0, size - 1, totalQS, Partition);
        assert(memcmp(data, sorted, size * sizeof(int)) == 0);

        cout << "\tRandomizedQuickSort\n";
        copy(begin(dataCopy), end(dataCopy), begin(data));
        QuickSort(data, 0, size - 1, totalRQS, RandomizedPartition);
        assert(memcmp(data, sorted, size * sizeof(int)) == 0);

        cout << "\tInsertionSort\n";
        copy(begin(dataCopy), end(dataCopy), begin(data));
        InsertionSort(data, size, totalIS, totalIS);
        assert(memcmp(data, sorted, size * sizeof(int)) == 0);
    }
    profiler.createGroup("total_DESC", "total_QuickSort_DESC", "total_RandomizedQuickSort_DESC", "total_InsertionSort_DESC");

    //Average Case
    for (int m = 1; m <= nr_of_measurements_average_case; m++)
        for (unsigned int size = min_size; size <= max_size; size += steps)
        {
            cout << "Evaluating average case for size: " << size << " - measurement: " << m << "\n";
            Operation totalQS = profiler.createOperation("total_QuickSort_average", size);
            Operation totalRQS = profiler.createOperation("total_RandomizedQuickSort_average", size);
            Operation totalIS = profiler.createOperation("total_InsertionSort_average", size);


            FillRandomArray(data, size, range_min, range_max, false, RANDOM);
            copy(begin(data), end(data), begin(dataCopy));
            copy(begin(data), end(data), begin(sorted));
            sort(sorted, sorted + size);

            cout << "\tQuickSort\n";
            QuickSort(data, 0, size - 1, totalQS, Partition);
            assert(memcmp(data, sorted, size * sizeof(int)) == 0);

            cout << "\tRandomizedQuickSort\n";
            copy(begin(dataCopy), end(dataCopy), begin(data));
            QuickSort(data, 0, size - 1, totalRQS, RandomizedPartition);
            assert(memcmp(data, sorted, size * sizeof(int)) == 0);

            cout << "\tInsertionSort\n";
            copy(begin(dataCopy), end(dataCopy), begin(data));
            InsertionSort(data, size, totalIS, totalIS);
            assert(memcmp(data, sorted, size * sizeof(int)) == 0);
        }
    profiler.divideValues("total_QuickSort_average", nr_of_measurements_average_case);
    profiler.divideValues("total_RandomizedQuickSort_average", nr_of_measurements_average_case);
    profiler.divideValues("total_InsertionSort_average", nr_of_measurements_average_case);
    profiler.createGroup("total_average", "total_QuickSort_average", "total_RandomizedQuickSort_average", "total_InsertionSort_average");

    profiler.showReport();
        }

int main()
{
#ifdef DEMO
    Demo();
    //FindDelta();
#else
    EvaluateSortingMethods();
#endif // DEMO
    return 0;
}