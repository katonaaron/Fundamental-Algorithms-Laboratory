/**
 * @author Katona Aron 
 * @group 30423
 *
 * Problem specification: 
 *	- Declare an array of integers of size MAX_SIZE– constant defined by you
 *	- Read a sequence of n numbers from the keyboard, and store them in the array
 *	- Print the n numbers in the array
 *	- Create and open a file, write the numbers from the array in the file, and close the file (check the file to see it worked)
 *	- Now open the previous file, read the contents and print them on the screen (don’t forget to close the file at the end)
 *
 */

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define MAX_SIZE 100

void Read(int Numbers[], int& N)
{
	cout << "n: ";
	cin >> N;

	for (int i = 0; i < N; i++)
		cin >> Numbers[i];
}

void Print(int Numbers[], int N)
{
	cout << "Array: ";
	for (int i = 0; i < N; i++)
		cout << Numbers[i] << " ";
	cout << "\n";
}

void WriteToFile(int Numbers[], int N, string Filename)
{
	ofstream f(Filename);
	for (int i = 0; i < N; i++)
		f << Numbers[i] << " ";
	f << "\n";
	f.close();
}

void ReadFromFile(string Filename)
{
	ifstream f(Filename);
	string row;
	while (getline(f, row))
		cout << row;
	cout << "\n";
}

int main()
{
	int numbers[MAX_SIZE], n;

	Read(numbers, n);
	Print(numbers, n);
	WriteToFile(numbers, n, "numbers.txt");
	ReadFromFile("numbers.txt");

	return 0;
}