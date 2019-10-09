/**
 * @author Katona Aron
 * @group 30423
 *
 * Problem specification:
 *	1. Write a function which generates an array of n random integers between Low and High, and returns the array; print the contents of the array in a file
 *  2. Write a function which generates a sorted array of random integers; print the contents of the array in a file
 *
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

void Read(int& N, int& Low, int& High)
{
	cout << "n: ";
	cin >> N;
	cout << "Low: ";
	cin >> Low;
	cout << "High: ";
	cin >> High;
}

void WriteToFile(int Numbers[], int N, string Filename)
{
	ofstream f(Filename);
	for (int i = 0; i < N; i++)
		f << Numbers[i] << " ";
	f << "\n";
	f.close();
}


int* GenerateRandomArray(int N, int Low, int High)
{
	int* numbers = new int[N];
	srand(time(0));

	for (int i = 0; i < N; i++)
	{
		numbers[i] = rand() % (High - Low + 1) + Low;
	}
	return numbers;
}

int* GenerateSortedArray(int N, int Low, int High)
{
	int* numbers = GenerateRandomArray(N, Low, High);
	
	sort(numbers, numbers + N);

	return numbers;
}

int main()
{
	int n, low, high;

	Read(n, low, high);
	WriteToFile(GenerateRandomArray(n, low, high), n, "random.txt");
	WriteToFile(GenerateSortedArray(n, low, high), n, "sorted.txt");

	return 0;
}