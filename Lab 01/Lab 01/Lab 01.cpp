#include <iostream>
#include <vector>
#include <climits>
#include <cassert>
#include "Profiler.h"

using namespace std;

#define MIN_SIZE 100
#define MAX_SIZE 10000
#define STEPS 500//100
#define RANGE_MIN 10
#define RANGE_MAX 50000

//#define DEBUG

Profiler profiler("Direct-Sorting-Methods");

typedef void (*SortingAlgortithm)(int A[], size_t Size, Operation Ass, Operation Comp);

void PrintArray(int A[], size_t Size)
{
	for (unsigned i = 0; i < Size; i++)
	{
		cout << A[i] << " ";
	}
	cout << "\n";
}

void TestSortingAlgorithm(SortingAlgortithm Sort)
{
	const int size = 10;
	const int range_min = 1;
	const int range_max = 10;
	const enum { RANDOM = 0, ASC, DESC } order = RANDOM;
	const bool unique = false;

	static int data[size], data2[size];

	Operation ass = profiler.createOperation("assing_test", size);
	Operation comp = profiler.createOperation("comp_test", size);

	FillRandomArray(data, size, range_min, range_max, unique, order);
	memcpy(data2, data, size * sizeof(int));;
	PrintArray(data, size);

	Sort(data, size, ass, comp);
	PrintArray(data, size);

	sort(data2, data2 + size);
	cout << "data2: ";
	PrintArray(data2, size);
	assert(memcmp(data, data2, size * sizeof(int)) == 0);
}

void InsertionSort(int A[], size_t Size, const char* AssOp, const char* CompOp)
{
	int key, j;
	for (int i = 1; i < Size; i++)
	{
		key = A[i];
		profiler.countOperation(AssOp, Size);
		j = i - 1;
		while (j >= 0 && A[j] > key)
		{
			profiler.countOperation(CompOp, Size);
			A[j + 1] = A[j];
			profiler.countOperation(AssOp, Size);
			j--;
		}
		if (j >= 0)
			profiler.countOperation(CompOp, Size);
		A[j + 1] = key;
		profiler.countOperation(AssOp, Size);

#ifdef DEBUG
		PrintArray(A, Size);
#endif // DEBUG
	}
}

void SelectionSort(int A[], size_t Size, string AssOp, string CompOp)
{
	int min_index, j, aux;
	for (int i = 0; i < Size; i++)
	{
		min_index = i;
		for (j = i + 1; j < Size; j++)
		{
			if (A[j] < A[min_index])
			{
				min_index = j;
			}
		}

		swap(A[i], A[min_index]);
#ifdef DEBUG
		PrintArray(A, Size);
#endif // DEBUG
	}
}

void EvaluateSortingAlgorithms()
{
	static int data[MAX_SIZE];

	//Best Case - Insertion
	for (unsigned int size = MIN_SIZE; size <= MAX_SIZE; size += STEPS)
	{
		cout << "Evaluating best case for size: " << size << "\n";
		//Operation ass = profiler.createOperation("assing_Insertion_best", size);
		//Operation comp = profiler.createOperation("comp_Insertion_best", size);
		FillRandomArray(data, size, RANGE_MIN, RANGE_MAX, false, 1);
		InsertionSort(data, size, "assing_Insertion_best", "comp_Insertion_best");
	}
	profiler.addSeries("sum_Insertion_best", "assing_Insertion_best", "comp_Insertion_best");

	//Worst Case - Insertion
	for (unsigned int size = MIN_SIZE; size <= MAX_SIZE; size += STEPS)
	{
		cout << "Evaluating worst case for size: " << size << "\n";
		//Operation ass = profiler.createOperation("assing_Insertion_worst", size);
		//Operation comp = profiler.createOperation("comp_Insertion_worst", size);
		FillRandomArray(data, size, RANGE_MIN, RANGE_MAX, false, 2);
		InsertionSort(data, size, "assing_Insertion_worst", "comp_Insertion_worst");
	}
	profiler.addSeries("sum_Insertion_worst", "assing_Insertion_worst", "comp_Insertion_worst");

	//Average Case - Insertion
	/*for (unsigned int size = MIN_SIZE; size <= MAX_SIZE; size += STEPS)
	{
		Operation assIns = profiler.createOperation("assing_Insertion_average", size);
		Operation compIns = profiler.createOperation("comp_Insertion_average", size);
		FillRandomArray(data, size, RANGE_MIN, RANGE_MAX, false, 0);
		InsertionSort(data, size, assIns, compIns);
	}
	profiler.addSeries("sum_Insertion_average", "assing_Insertion_average", "comp_Insertion_average");*/

	/*//Selection sort - no case
	for (unsigned int size = MIN_SIZE; size <= MAX_SIZE; size += STEPS)
	{
		Operation ass = profiler.createOperation("assing_Selection", size);
		Operation comp = profiler.createOperation("comp_Selection", size);
		FillRandomArray(data, size, RANGE_MIN, RANGE_MAX, false, 2);
		InsertionSort(data, size, ass, comp);
	}
	profiler.addSeries("sum_Selection", "assing_Selection", "comp_Selection");*/
}



int main()
{
	//TestSortingAlgorithm(InsertionSort);
	//TestSortingAlgorithm(SelectionSort);
	EvaluateSortingAlgorithms();
	profiler.showReport();

	return 0;
}