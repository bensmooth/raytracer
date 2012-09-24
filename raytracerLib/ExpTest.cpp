#include <iostream>
#include <math.h>
#include "Image.h"


using namespace std;


// Returns current millisecond tick count.
int64_t GetTickCount()
{
	// This clock will probably not overflow and go backwards.
	timespec time;
	clock_gettime(CLOCK_MONOTONIC, &time);

	int64_t result = time.tv_sec * 1000;
	result += time.tv_nsec /  1000000;

	return (result);
}


// Function pointer to whatever function we will be invoking.
typedef double (*ExponentFuncPtr)(double);


// Times an exponent function on two passes on the given data.  Returns the elapsed time in milliseconds.
int64_t TestLibraryExp(ExponentFuncPtr exponentFunc, vector<double> inputData)
{
	int64_t totalTime = 0;

	vector<double>::const_iterator iter = inputData.begin();
	while (iter != inputData.end())
	{
		double currentX = *iter;
		int64_t startTime = GetTickCount();
		for (int i = 0; i < 100000; i++)
		{
			exponentFunc(currentX);
		}
		totalTime += GetTickCount() - startTime;
		iter++;
	}

	return (totalTime);
}


int main()
{
	// Generate some data.
	vector<double> inputData;
	for (int i = 0; i < 1000; i++)
	{
		const double min = -1.5, max = 1.5;
		inputData.push_back(min + (double)rand()/((double)RAND_MAX/(max-min)));
	}

	cout << "Ours:\t" << TestLibraryExp(Exponent, inputData) << " ms" << endl;
	cout << "Library:\t" << TestLibraryExp(exp, inputData) << " ms" << endl;

	return (0);
}
