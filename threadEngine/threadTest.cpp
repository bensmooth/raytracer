#include <stdio.h>
#include "Thread.h"
#include "Mutex.h"

using namespace ThreadEngine;


const int MAX_COUNT = 100;

int count = 0;
Mutex countMutex;


// Only counts up on even numbers.
void *countUpEven(void *unused)
{
	bool shouldContinue = true;
	while (shouldContinue)
	{
		countMutex.Lock();
		shouldContinue = count < MAX_COUNT;
		if ((count % 2) == 0)
		{
			count++;
			printf("Even here, count is %i\n", count);
		}
		countMutex.Unlock();
	}

	return NULL;
}


// Only counts up on odd numbers.
void *countUpOdd(void *unused)
{
	bool shouldContinue = true;
	while (shouldContinue)
	{
		countMutex.Lock();
		shouldContinue = count < MAX_COUNT;
		if ((count % 2) == 1)
		{
			count++;
			printf("Odd here, count is %i\n", count);
		}
		countMutex.Unlock();
	}

	return NULL;
}


int main()
{
	Thread even, odd;
	even.Start(countUpEven, NULL);
	printf("Even started\n.");
	odd.Start(countUpOdd, NULL);
	printf("Odd started.\n");

	if (even.Join(NULL) == true)
	{
		printf("Even joined.\n");
	}

	if (odd.Join(NULL) == true)
	{
		printf("Odd joined.\n");
	}

	printf("Count is %i\n", count);
}
