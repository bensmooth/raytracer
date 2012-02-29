#pragma once

#include <vector>
#include <queue>

#include "Thread.h"
#include "Mutex.h"

namespace ThreadEngine
{
void *ProcessJobs(void *inData);

/**
 * The type of function that gets called whenever a job gets done.
 * First argument is a pointer to the return value of the job.
 */
typedef void (*OnThreadComplete) (void *);

/**
 * Contains information about a job.
 */
struct Job
{
	// Function that is called when the job is complete.
	OnThreadComplete onComplete;

	// The job.
	Thread::UserFunction job;

	// The parameter to the job.
	void *param;
};


class ThreadPool
{
public:
	/**
	 * Constructs a threadpool with the given number of initial threads.
	 * @param threadCount The number of threads in the threadpool.  Defaults to the number of processors.
	 */
	ThreadPool(int threadCount = GetNumberOfProcessors());

	/**
	 * Cancels any still-running threads before exiting.
	 * This means that any jobs still in the queue will probably not get done.
	 */
	~ThreadPool();

	/**
	 * Detects the number of processors.
	 */
	static int GetNumberOfProcessors();

	/**
	 * Adds a job to the queue.  It will be executed when any other jobs in front of it are completed.
	 * Note that the jobFunction and onCompletionFunction will be called in a different thread than your application, so it is wise to use synchronization primitives when accessing shared memory.
	 * @note Jobs are usually processed in the order they were added in.
	 * @param jobFunction A pointer to the function to call.
	 * @param input The input the function will be called with. Defaults to NULL.
	 * @param onCompletionFunction The function that will be called when the job completes.  Defaults to NULL.
	 */
	void AddJob(Thread::UserFunction jobFunction, void *input = NULL, OnThreadComplete onCompletionFunction = NULL);

	/**
	 * Starts the processing threads.  Jobs will now start to be executed.
	 * @note Jobs can still safely be added after calling this function.
	 */
	void StartProcessing();

	/**
	 * Blocks until all of the jobs in the queue have been processed.
	 * @warning For a clean shutdown, this should be called before the destructor is invoked.
	 */
	void JoinAll();

private:
	/**
	 * The list of jobs.
	 */
	std::queue<Job> m_jobs;
	ThreadEngine::Mutex m_jobListLock;

	/**
	 * The list of threads.
	 */
	std::vector<ThreadEngine::Thread*> m_threads;
	ThreadEngine::Mutex m_threadListLock;

	/**
	 * This gets set to true if job processing should stop.
	 */
	volatile bool m_haltAfterProcessing;

	friend void *ProcessJobs(void *inData);
};

}

