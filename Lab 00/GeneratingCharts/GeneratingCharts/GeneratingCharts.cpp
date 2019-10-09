/**
 * @author Katona Aron
 * @group 30423
 *
 * Problem specification:
 *	Write a C/C++ program which writes in a file, for n starting from 100 to 10.000 (with a 100 increment), the
 *  following values (for each value of n use a separate line):
 *    n, 100*log(n), 10*n, 0.5*n^2, 2^n, 2*n!
 *  Use the values in the file to build scatter plots for these functions, either by using MS Excel or the Profiler Framework
 *
 *	Observations:
 *	  The computed values on the charts represent the growth rate of algorithms with logarithmic, linear, polynomial, exponential and factorial complexity.
 *	  For n > 20 the calculated values for 2*n!  are so big that they cannot be stored in a long long, similarly, we can calculate 2^n only for n <= 30.
 *	  Thus we can deduce that the two are inefficient, and the growth order is: logarithmic < linear < polynomial < exponential(2^n) < factorial
 */
#include <fstream>
#include <math.h>
#include "Profiler.h"

using namespace std;

Profiler profiler("GeneratingCharts");

long long factorial(int n)
{
	long long res = 1;
	for (int i = 2; i <= n; i++)
	{
		res *= i;
	}
	return res;
}


int main()
{
	ofstream f("values.txt");

	for (int n = 1; n <= 20; n++)
	{
		f << n << "; " << 100 * log(n) << "; " << 10 * n << "; " << 0.5 * pow(n, 2) << "; " << (1 << n) << "; " << 2 * factorial(n) << "\n";
		profiler.countOperation("n", n, n);
		profiler.countOperation("100 * log(n)", n, 100 * log(n));
		profiler.countOperation("10 * n", n, 10 * n);
		profiler.countOperation("0.5 * n^2 ", n, 0.5 * pow(n, 2));
		profiler.countOperation("2^n", n, (1 << n));
		profiler.countOperation("2*n!", n, 2 * factorial(n));
	}
	profiler.showReport();
	f.close();
	return 0;
}