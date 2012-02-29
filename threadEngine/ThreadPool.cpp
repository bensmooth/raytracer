#include "ThreadPool.h"

#include <unistd.h>
#include <time.h>

using namespace ThreadEngine;


// Processes jobs.
void *ThreadEngine::ProcessJobs(void *inData)
{
	ThreadPool *pool = (ThreadPool*)inData;
	bool halt = false;

	// This is the amount of time we will sleep in between checking for jobs when we don't have one.
	timespec sleepTime;
	memset(&sleepTime, 0, sizeof(sleepTime));
	sleepTime.tv_nsec = 10000000; // 10 milliseconds.

	while (halt == false)
	{
		pool->m_jobListLock.Lock();
		if (pool->m_jobs.size() > 0)
		{
			// Get the next job off the queue.
			Job job = pool->m_jobs.front();
			pool->m_jobs.pop();
			pool->m_jobListLock.Unlock();

			// Process the job.
			void *result = job.job(job.param);

			if (job.onComplete != NULL)
			{
				job.onComplete(result);
			}
		}
		else
		{
			// If we got here, there is nothing in the queue.
			if (pool->m_haltAfterProcessing)
			{
				// There is nothing in the queue, and we have been asked to stop after processing everything.
				halt = true;
			}
			pool->m_jobListLock.Unlock();

			// Sleep a bit here so that we don't hog CPU time while waiting for jobs.
			nanosleep(&sleepTime, NULL);
		}
	}

	return (NULL);
}


ThreadEngine::ThreadPool::ThreadPool(int threadCount)
{
	m_haltAfterProcessing = false;

	for (int i = 0; i < threadCount; i++)
	{
		m_threads.push_back(new Thread());
	}
}


ThreadEngine::ThreadPool::~ThreadPool()
{
	// Clear out all jobs.
	m_jobListLock.Lock();
	while (m_jobs.size() > 0)
	{
		m_jobs.pop();
	}
	m_jobListLock.Unlock();

	// Cancel all threads.
	m_threadListLock.Lock();
	for (int i = 0; i < m_threads.size(); i++)
	{
		m_threads[i]->Cancel();
		m_threads[i]->Join(NULL);
		delete m_threads[i];
		m_threads[i] = NULL;
	}
	m_threadListLock.Unlock();
}


int ThreadEngine::ThreadPool::GetNumberOfProcessors()
{
	return (sysconf( _SC_NPROCESSORS_ONLN ));
}


void ThreadPool::StartProcessing()
{
	// Start each thread.
	m_threadListLock.Lock();
	for (int i = 0; i < m_threads.size(); i++)
	{
		m_threads[i]->Start(ProcessJobs, this);
	}
	m_threadListLock.Unlock();
}



void ThreadPool::JoinAll()
{
	// Tell the threads to return after all the jobs are done.
	m_haltAfterProcessing = true;

	// Wait for each thread to join with us.
	m_threadListLock.Lock();
	for (int i = 0; i < m_threads.size(); i++)
	{
		m_threads[i]->Join(NULL);
	}
	m_threadListLock.Unlock();
}


void ThreadEngine::ThreadPool::AddJob(ThreadEngine::Thread::UserFunction jobFunction, void* input, OnThreadComplete onCompletionFunction)
{
	// Lock down the queue.
	m_jobListLock.Lock();
	Job job;
	job.job = jobFunction;
	job.param = input;
	job.onComplete = onCompletionFunction;
	m_jobs.push(job);
	m_jobListLock.Unlock();
}
