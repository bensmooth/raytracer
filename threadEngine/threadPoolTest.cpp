#include <stdio.h>
#include "ThreadPool.h"

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


void *helloCount(void *unused)
{
	// Print out current value of count.
	printf("INTERRUPTING COW!\tCount is %i\n", count);

	return (NULL);
}


void *countReallyHigh(void *unused)
{
	printf("Starting to count really high!\n");
	int i;
	for (i = 1; i != 0; i++);
	printf("Finished counting high, ended at %i\n", i);

	return (NULL);
}


int main()
{
	int cpuCount = ThreadPool::GetNumberOfProcessors();
	cpuCount += 1;
	printf("Starting pool with %i threads.\n", cpuCount);
	ThreadPool pool(cpuCount);

	pool.AddJob(countUpOdd);
	printf("Added odd job to queue.\n");

	pool.AddJob(countUpEven);
	printf("Added even job to queue.\n");

	pool.StartProcessing();
	printf("Pool started\n.");

	pool.AddJob(helloCount);
	pool.AddJob(countReallyHigh);
	pool.AddJob(helloCount);

	pool.JoinAll();
	printf("Joined all threads in pool.\n");

	printf("Count is %i\n", count);
}
