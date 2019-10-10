/**
 * @author Katona Aron
 * @group 30423
 *
 * Problem specification:
 *		Implement correctly and efficiently 3 direct sorting methods (Bubble Sort,
 *		Insertion Sort - using either linear or binary insertion and Selection Sort)
 *
 * Correctness:
 *		In each algorithm the array A[0...n-1] is logically split into two sets: the Destination(D) set, in which the elements are in a sorted order,
 *      and the Source(S) set which contains elements in an arbitrary order. Initially, D is empty and S contains all the elements of the set.
 *      The algorithms in every iteration take one element from S and insert it into D, such that D preserves its sorted property. In the end S
 *      is empty and D contains every element in sorted order. 
 *
 *	1. Insertion sort:
 *      In this algorithm, we start as D having 1 element, the first element of the array, based on the fact that a single element is already sorted.
 *
 *      pre-condition: |D| = 1 is sorted at the beginning
 *      post-condition: |D| = n , n is the size of the array, D contains every element of the initial array in a sorted order, thus the array is sorted.
 *      loop invariant: A[0..i] = D is sorted
 *          initialization: D =  A[0] is sorted, thus the invariant holds before the first iteration
 *          maintenance: In each iteration, the correct position of A[i] is searched in A[0..i-1], by shifting the elements in A[0..i-1] to the right until the correct
 *                       position is found. Thus an element is taken from S and inserted into D. The result is the original elements in A[0...i], but in sorted order.
 *          termination: The algorithm terminates when i == n, meaning that all the elements from S was inserted into D, and A[0...n-1] contains the original
 *                       elements but in sorted order.
 *  2. Selection sort
 *      pre-condition: |D| = 0 is sorted at the beginning
 *      post-condition: |D| = n , n is the size of the array, D contains every element of the initial array in a sorted order, thus the array is sorted.
 *      loop invariant: A[0..i] = D is sorted
 *          initialization: D is empty, therefore it is sorted, thus the invariant holds before the first iteration
 *          maintenance: In each iteration, the minimum element from S is taken and inserted at the end of D, at A[i]. Because all the elements in A[0...i-1]=D are smaller than
 *                       or equal with each element in A[i...n-1]=S, by inserting the smallest element from S at A[i], A[0...i] remains sorted.
 *          termination: The algorithm terminates when i == n-1, the elements in A[0...n-2] = D are sorted and S = A[n-1]. Because all the elements in D are smaller than or 
 *                       equal with each element in S, A[n-1] can be considered as the last element in D, thus A[0...n-1] contains the original elements but in sorted order.
 *  3. Bubble sort
 *      pre-condition: |D| = 0 is sorted at the beginning
 *      post-condition: |D| = n , n is the size of the array, D contains every element of the initial array in a sorted order, thus the array is sorted.
 *      loop invariant: A[0..i] = D is sorted
 *          initialization: D is empty, therefore it is sorted, thus the invariant holds before the first iteration
 *          maintenance: In each iteration, the minimum element from S is taken and inserted at the end of D, at A[i]. This is accomplished by swapping all the adjacent elements 
 *                       A[j], A[j-1], for which A[j] < A[j -1], j starting each time from the end of the array (n-1), downto the first element of S, A[i]. After the last swap,  
 *                       A[0..i] will be in a sorted order, thus an element from S is inserted into D.
 *          termination: The algorithm terminates when i == n or when no swaps were executed, both means that A[0...n-1] contains the original elements in a sorted order.
 * Efficiency:
 *  1. Insertion sort:  
 *      Best case: The array is already sorted: Each element of the array is compared to its neighbour, thus it has O(n) comparisions, each time the key of A[i] is saved into
 *                 an auxiliary memory and restored, thus the implementation has O(n) assignments, resulting in an O(n) total complexity on the best case.
 *      Worst case: The array is inversely sorted: Each element is compared to every element before it, and each time the previous elements are shifted right, resulting in 
 *                  O(n^2) comparisions, assignments and total complexity on worst case.
 *      Average case: The array is not sorted: There are so many comparisions and assignments in each iteration, that it results in O(n^2) comparisions, assignments and total 
 *                    complexity on average case.
 *  2. Selection sort:
 *      The number of comparisions is independent of the monotonity of the input array, in each case it is O(n^2), because in each iteration we search trough all the elements in S. 
 *      By avoiding swapping an element with itself, the number of assignments in the best case is 0, thus O(1). But in the worst and average case there are O(n) assignment, in each
 *      iteration one swap will be executed. The total complexity is O(n^2) in each case.
 *  3. Bubble sort:
 *      Best case: The array is already sorted: Each element of the array is compared to its neighbour, then because of the absence of any swap the algorithm terminates. 
 *                 Thus it has O(n) comparisions, 0 = O(1) assignments and O(n) total complexity on best case.
 *      Worst case: The array is inversely sorted: Each two element in the array will be compared and swapped, resulting in O(n^2) comparisions, assignments and total 
 *                  complexity on worst case.
 *      Average case: The array is not sorted: There are so many comparisions and assignments in each iteration, that it results in O(n^2) comparisions, assignments and total
 *                    complexity on average case.
 * Stability:
 *  1. Insertion sort:
 *      The implementation is stable because in the comparision the "<" operator was used instead of "<=", meaning that if two elements with the same key are compared, they will not
 *      be swapped, they will be placed next to each other, preserving the relative order of equal elements from the input in the output.
 *  2. Selection sort:
 *      This implementation is not stable because swapping non adjacent elements could change the relative order of some equal elements in S.
 *  3. Bubble sort:
 *      The implementation is stable because in the comparision the "<" operator was used instead of "<=", meaning that if two elements with the same key are compared, they will not
 *      be swapped, the element with the smallest index will be compared to the other elements from that point forward, preserving the relative order of equal elements from the 
 *      input in the output.
 *
 * Comparision:
 *      1. All of the three sorting methods are comparision based, in place sorting algorithms with O(1) auxiliary space complexity. None of them are optimal, because the worst case
 *      complexity is O(n^2) != Omega(nlogn). 
 *      2. In the best case Insertion s. and Bubble s. have better comparision complexity then Selection s., O(n) instead of O(n^2). In the worst case the comparisions are O(n^2).
 *      Although in the average case the comparisions are O(n^2), from the chart we can deduce that Insertion s. does fewer comparisions than the others, in this implementation it 
 *      has half as much.
 *      3. In the best case Insertion s. has O(n), the others have 0 = O(1) assignments. But in the average and worst case, Selection s. has O(n), the others have O(n^2) assignments. 
 *      It can be deduced from the charts that Bubble sort does much more assignments than Insertion sort in the worst and average case.
 *      4. The total complexity of Selection s. in the best case is O(N^2) while the others are O(n). In the other cases all the algorithms are O(n^2), but from the chart we can deduce 
 *      that Bubblesort uses more operations than the others.
 */

#include <iostream>
#include <vector>
#include <climits>
#include <cassert>
#include "Profiler.h"

using namespace std;

#define MIN_SIZE 100
#define MAX_SIZE 10000
#define STEPS 100
#define NR_OF_MEASUREMENTS_AVERAGE_CASE 5
#define RANGE_MIN 10
#define RANGE_MAX 50000

#define DEBUG

typedef void (*SortingAlgortithm)(int A[], size_t Size, Operation Ass, Operation Comp);

void PrintArray(int A[], size_t Size, int i = -1, int j = -1)
{
    for (unsigned i = 0; i < Size; i++)
    {
        cout << A[i] << " ";
    }
    if (i >= 0)
    {
        cout << "\t(i = " << i;
        if (j >= 0)
            cout << ", j = " << j << ")";
        else
            cout << ")";
    }
    cout << "\n";
}

void TestSortingAlgorithm(SortingAlgortithm Sort)
{
    const int size = 5;
    const int range_min = 1;
    const int range_max = 10;
    const enum { RANDOM = 0, ASC, DESC } order = RANDOM;
    const bool unique = false;

    static int data[size], dataCopy[size];

    Profiler profiler("test");
    Operation ass = profiler.createOperation("assing_test", size);
    Operation comp = profiler.createOperation("comp_test", size);

    FillRandomArray(data, size, range_min, range_max, unique, order);
    copy(begin(data), end(data), begin(dataCopy));

    cout << "I: ";
    PrintArray(data, size);

    Sort(data, size, ass, comp);

    sort(dataCopy, dataCopy + size);
    cout << "R: ";
    PrintArray(dataCopy, size);
    assert(memcmp(data, dataCopy, size * sizeof(int)) == 0);

    //profiler.showReport();
}

void InsertionSort(int A[], size_t Size, Operation Ass, Operation Comp)
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
#ifdef DEBUG
            PrintArray(A, Size, i, j + 1);
#endif // DEBUG
        }
        if (j >= 0)
            Comp.count();

        A[j + 1] = key;
        Ass.count();

#ifdef DEBUG
        PrintArray(A, Size, i);
#endif // DEBUG
    }
}

void SelectionSort(int A[], size_t Size, Operation Ass, Operation Comp)
{
    int min_index, j;
    for (int i = 0; i < Size - 1; i++)
    {
        min_index = i;
        for (j = i + 1; j < Size; j++)
        {
            Comp.count();
            if (A[j] < A[min_index])
            {
                min_index = j;
            }
        }
        if (min_index != i)
        {
            swap(A[i], A[min_index]);
            Ass.count(3);
        }

#ifdef DEBUG
        PrintArray(A, Size, i);
#endif // DEBUG
    }
}

void BubbleSort(int A[], size_t Size, Operation Ass, Operation Comp)
{
    bool swapped = true;
    for (int i = 0; i < Size - 1; i++)
    {
        swapped = false;
        for (int j = Size - 1; j > i; j--)
        {
            Comp.count();
            if (A[j] < A[j - 1])
            {
                swap(A[j], A[j - 1]);
                Ass.count(3);
                swapped = true;
#ifdef DEBUG
                PrintArray(A, Size, i, j);
#endif // DEBUG
            }
        }
#ifdef DEBUG
        PrintArray(A, Size, i);
#endif // DEBUG

        if (!swapped)
            return;
    }
}

void EvaluateSortingAlgorithms()
{
    static int data[MAX_SIZE], dataCopy[MAX_SIZE];

    //Best Case
    Profiler profiler("Direct-Sorting-Methods-Best-Case");
    for (unsigned int size = MIN_SIZE; size <= MAX_SIZE; size += STEPS)
    {
        cout << "Evaluating best case for size: " << size << "\n";
        Operation assIns = profiler.createOperation("assing_Insertion", size);
        Operation assSel = profiler.createOperation("assing_Selection", size);
        Operation assBub = profiler.createOperation("assing_Bubble", size);
        Operation compIns = profiler.createOperation("comp_Insertion", size);
        Operation compSel = profiler.createOperation("comp_Selection", size);
        Operation compBub = profiler.createOperation("comp_Bubble", size);

        FillRandomArray(data, size, RANGE_MIN, RANGE_MAX, false, 1);
        copy(begin(data), end(data), begin(dataCopy));

        cout << "\tInsertion\n";
        InsertionSort(data, size, assIns, compIns);

        copy(begin(dataCopy), end(dataCopy), begin(data));
        cout << "\tSelection\n";
        SelectionSort(data, size, assSel, compSel);

        copy(begin(dataCopy), end(dataCopy), begin(data));
        cout << "\tBubble\n";
        BubbleSort(data, size, assBub, compBub);
    }
    profiler.addSeries("sum_Insertion", "assing_Insertion", "comp_Insertion");
    profiler.addSeries("sum_Selection", "assing_Selection", "comp_Selection");
    profiler.addSeries("sum_Bubble", "assing_Bubble", "comp_Bubble");
    profiler.createGroup("assing", "assing_Insertion", "assing_Selection", "assing_Bubble");
    profiler.createGroup("comp_Insertion_Bubble", "comp_Insertion", "comp_Bubble");
    profiler.createGroup("sum_Insertion_Bubble", "sum_Insertion", "sum_Bubble");

    //Worst Case
    profiler.reset("Direct-Sorting-Methods-Worst-Case");
    for (unsigned int size = MIN_SIZE; size <= MAX_SIZE; size += STEPS)
    {
        cout << "Evaluating worst case for size: " << size << "\n";
        Operation assIns = profiler.createOperation("assing_Insertion", size);
        Operation assSel = profiler.createOperation("assing_Selection", size);
        Operation assBub = profiler.createOperation("assing_Bubble", size);
        Operation compIns = profiler.createOperation("comp_Insertion", size);
        Operation compSel = profiler.createOperation("comp_Selection", size);
        Operation compBub = profiler.createOperation("comp_Bubble", size);

        FillRandomArray(data, size, RANGE_MIN, RANGE_MAX, false, 2);
        copy(begin(data), end(data), begin(dataCopy));

        cout << "\tInsertion\n";
        InsertionSort(data, size, assIns, compIns);

        copy(begin(dataCopy), end(dataCopy), begin(data));
        cout << "\tSelection\n";
        SelectionSort(data, size, assSel, compSel);

        copy(begin(dataCopy), end(dataCopy), begin(data));
        cout << "\tBubble\n";
        BubbleSort(data, size, assBub, compBub);
    }
    profiler.addSeries("sum_Insertion", "assing_Insertion", "comp_Insertion");
    profiler.addSeries("sum_Selection", "assing_Selection", "comp_Selection");
    profiler.addSeries("sum_Bubble", "assing_Bubble", "comp_Bubble");
    profiler.createGroup("assing_Insertion_Bubble", "assing_Insertion", "assing_Bubble");
    profiler.createGroup("comp", "comp_Insertion", "comp_Selection", "comp_Bubble");
    profiler.createGroup("sum", "sum_Insertion", "sum_Selection", "sum_Bubble");

    //Average Case
    profiler.reset("Direct-Sorting-Methods-Average-Case");
    for (int m = 1; m <= NR_OF_MEASUREMENTS_AVERAGE_CASE; m++)
        for (unsigned int size = MIN_SIZE; size <= MAX_SIZE; size += STEPS)
        {
            cout << "Evaluating average case for size: " << size << " - measurement: " << m << "\n";
            Operation assIns = profiler.createOperation("assing_Insertion", size);
            Operation assSel = profiler.createOperation("assing_Selection", size);
            Operation assBub = profiler.createOperation("assing_Bubble", size);
            Operation compIns = profiler.createOperation("comp_Insertion", size);
            Operation compSel = profiler.createOperation("comp_Selection", size);
            Operation compBub = profiler.createOperation("comp_Bubble", size);

            FillRandomArray(data, size, RANGE_MIN, RANGE_MAX, false, 0);
            copy(begin(data), end(data), begin(dataCopy));

            cout << "\tInsertion\n";
            InsertionSort(data, size, assIns, compIns);

            copy(begin(dataCopy), end(dataCopy), begin(data));
            cout << "\tSelection\n";
            SelectionSort(data, size, assSel, compSel);

            copy(begin(dataCopy), end(dataCopy), begin(data));
            cout << "\tBubble\n";
            BubbleSort(data, size, assBub, compBub);
        }
    profiler.divideValues("assing_Insertion", NR_OF_MEASUREMENTS_AVERAGE_CASE);
    profiler.divideValues("assing_Selection", NR_OF_MEASUREMENTS_AVERAGE_CASE);
    profiler.divideValues("assing_Bubble", NR_OF_MEASUREMENTS_AVERAGE_CASE);
    profiler.divideValues("comp_Insertion", NR_OF_MEASUREMENTS_AVERAGE_CASE);
    profiler.divideValues("comp_Selection", NR_OF_MEASUREMENTS_AVERAGE_CASE);
    profiler.divideValues("comp_Bubble", NR_OF_MEASUREMENTS_AVERAGE_CASE);
    profiler.addSeries("sum_Insertion", "assing_Insertion", "comp_Insertion");
    profiler.addSeries("sum_Selection", "assing_Selection", "comp_Selection");
    profiler.addSeries("sum_Bubble", "assing_Bubble", "comp_Bubble");
    profiler.createGroup("assing_Insertion_Bubble", "assing_Insertion", "assing_Bubble");
    profiler.createGroup("comp", "comp_Insertion", "comp_Selection", "comp_Bubble");
    profiler.createGroup("sum", "sum_Insertion", "sum_Selection", "sum_Bubble");

    profiler.showReport();
}



int main()
{
    TestSortingAlgorithm(InsertionSort);
    TestSortingAlgorithm(SelectionSort);
    TestSortingAlgorithm(BubbleSort);
    //EvaluateSortingAlgorithms();

    return 0;
}