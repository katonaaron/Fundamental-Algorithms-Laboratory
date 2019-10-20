/**
 * @author Katona Aron
 * @group 30423
 *
 * Problem specification:
 *		 Implement correctly and efficiently two methods for building a heap, namely the bottom­-up and the top-­down strategies. Additionally, implement heapsort.
 *
 * Correctness:
 *  For both heap building methods: 
 *      pre-condition: An array A[0...n-1] represents a binary tree, for any parent with index i, their child are on index 2*i+1 and 2*i+2.
 *      post-conditions: The elements in A are reordered such that every node satisfies the heap property.
 *
 *	1. Bottom-up heap building:
 *      loop invariant: Each node with the indices i+1, i+2,...,n is a root of a heap
 *      initialization: Half of the nodes of a binary tree are leaves, and each leaf is a heap with only one node. Thus, by the start of the first loop the nodes with the indices
 *                      n/2, n/2+1, ... n-1 are leaves, and in the same time they are roots of heaps with a single node.
 *      maintenance: In each the iteration, by the loop invariant the children of node i (2*i+1, 2*i+2) are roots of heaps. The Heapify procedure is called, which
 *                   has as input a node having two children that are roots of heaps, and reorders the elements such that the node at the index i will become a root of a heap.
 *                   By decrementing i, the loop invariant holds.
 *      termination: The algorithm terminates when i == -1. By the loop invariant all the nodes with indices 0,...n-1 are roots of heaps. Thus every node satisfies the heap property.
 *  2. Top-down heap building:
 *      loop invariant: The sub-array A[0...i-1] is a heap
 *      initialization: Initially i==0, so this sub-array is empty, thus there does not exist any heap property violation, so the loop invariant holds.
 *      maintenance: In each iteration the size of the heap (A[0...i-1]) is increased. The element at the index i is added to the heap such that it is swapped with its parent, 
 *                  then with it's initial parent's parent, and so on, until the heap property is restored,  this element might become the root of the heap. Thus, when this element
 *                  is inserted, every node satisfies the heap property in the sub-array A[0...i], and by incrementing i, the loop invariant holds.
 *      termination: The algorithm terminates when i==n. By the loop invariant A[0...n-1] is a heap, so the heap property is satisfied in all the parent-child relations in the array.
 *
 * Efficiency:
 *  1. Heapify:
 *      The running time at one level is O(1) (swap i with the largest of Left(i) and Right(i)). The height of the tree is h = log2(n), thus there are h+1 levels, so the complexity
 *      of heapify is O(h) == O(log n).
 *  2. Bottom-up heap building:
 *      The Heapify procedure is called only for the non-leaf elements, so for 1/2*n elements 0 operations are done.
 *      For the parents of the leaves, a constant amount of operation is done. So the required time to heapify all of them is 1/(2^2) * n * 1.
 *      For their parents there are 2 steps to heapify them, so the required time is 1/(2^3)*n*2.
 *      By summing the required time of all levels, we get that, the total complexity is: t(n) == Sum(h: 0->log n)(n/(2^(h+1))*h) == n/2 Sum(h: 0->log n)(1/(2^h)*h).
 *      By applying the series: Sum(k*x^k) == x/((1-x)^2) we get t(n) == n/2 * 2 == O(n). Thus the heap is built with a linear complexity.
 *  3. HeapInsert:
 *      This procedure is similar to Heapify, the difference is that, while Heapify is top-down, HeapInsert is bottom-up, it compares the inserted leaf to its parent at each level.
 *      Therefore the complexity of HeapInsert is O(h) == O(log n).
 *  4. Top-down heap building:
 *      The HeapInsert method is called for each element of the initial array, and for each element there could be h swaps, thus the total complexity of the top-down heap building is
 *      O(n) * O(h) == O(n) * O(log n) == O(n * log n).
 *
 * Stability:
 *  These heap building methods are not stable. Examples: Bottom-Up: A = {4, 2, 2, 1, 3, 0}, Top-Down: A = {5, 4, 2, 3, 1, 3}
 *
 * Comparison of the max-heap building methods:
 *  Best case: 
 *      In the best case, when the array is sorted in descending order, both Heapify and HeapInsert has the complexity O(1), because no recursion and no loop is executed. Therefore 
 *      both heap building methods are O(n).
 *  Worst case:
 *      In the worst case, the array is sorted in ascending order. For each element HeapInsert traverses the nodes from the leaf to the root. Thus the difference in running time 
 *      between the O(n) bottom-up method and the O(n*log n) top-down method is the greatest in this case. The latter grows faster, thus the former is more efficient.
 *  Average case:
 *      As in any other case, in the average case the bottom-up method is also O(n). From the charts we can deduce, that although the complexity of the top-down function is greater,
 *      in the average case the difference in the running time is much smaller than in the worst case.
 *  Conclusion:
 *      In each case (best, average, worst) the bottom-up heap building method outperforms the top-down one in the case of the running time: O(n). This makes the bottom-up method a 
 *          great choice to be used in sorting, in the heapsort. But the dimension of the heap is fixed, we cannot add any new elements into it.
 *      In the other hand, the size of the heap is variable in the top-down method. One can any time insert new elements to the heap in O(log n), get the maximum element in O(1),
 *          and extract it in O(log n). At insertion the size of the heap increases, and at extraction the size of the heap decreases. These properties make the heap an efficient 
 *          priority queue implementation.
 *      In conclusion, the bottom-up heap build method should be used in heapsort, the top-down should be used as a priority queue.
 *              
 */

#include <iostream>
#include <vector>
#include "Profiler.h"

using namespace std;

enum SORTED_ORDER { RANDOM = 0, ASC, DESC };

//Configuration of the evaluation
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