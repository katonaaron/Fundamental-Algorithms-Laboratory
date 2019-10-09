#include <iostream>
#include <vector>
#include <climits>
#include <cassert>
#include "Profiler.h"

using namespace std;

#define MIN_SIZE 100
#define MAX_SIZE 10000
#define STEPS 500//100
#define NR_OF_MEASUREMENTS_AVERAGE_CASE 5
#define RANGE_MIN 10
#define RANGE_MAX 50000

//#define DEBUG

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

	static int data[size], dataCopy[size];

	Profiler profiler("test");
	Operation ass = profiler.createOperation("assing_test", size);
	Operation comp = profiler.createOperation("comp_test", size);

	FillRandomArray(data, size, range_min, range_max, unique, order);
	copy(begin(data), end(data), begin(dataCopy));
	cout << "dataCopy: ";
	PrintArray(dataCopy, size);
	PrintArray(data, size);

	Sort(data, size, ass, comp);
	PrintArray(data, size);

	sort(dataCopy, dataCopy + size);
	cout << "dataCopy: ";
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
		}
		if (j >= 0)
			Comp.count();
		if (j != i - 1)
		{
			A[j + 1] = key;
			Ass.count();
		}

#ifdef DEBUG
		PrintArray(A, Size);
#endif // DEBUG
		}
	}

void SelectionSort(int A[], size_t Size, Operation Ass, Operation Comp)
{
	int min_index, j;
	for (int i = 0; i < Size; i++)
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
		PrintArray(A, Size);
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
			}
		}
#ifdef DEBUG
		PrintArray(A, Size);
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
	//TestSortingAlgorithm(InsertionSort);
	//TestSortingAlgorithm(SelectionSort);
	//TestSortingAlgorithm(BubbleSort);
	EvaluateSortingAlgorithms();

	return 0;
}