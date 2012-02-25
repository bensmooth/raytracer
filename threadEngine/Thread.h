/* 
 * File:   Thread.h
 * Author: Ben Hart
 */

#pragma once

#include <pthread.h>
#include <signal.h>

namespace ThreadEngine
{

/**
 * Encapsulation of a POSIX thread.
 */
class Thread
{
public:
	// TODO: Implement a global thread table so that threads can get a safe, usable Thread object of themselves.
	// TODO: This would also allow us to not use confusing cancellation points, and instead use an Interrumpt(Thread)
	// TODO: and Interrupted() mechanism like in Java.

	/**
	 * Constructs the thread.
     */
	Thread();

	/**
	 * Destructs the thread.  If the thread is currently running, it will be cancelled.
     */
	virtual ~Thread();

	/**
	 * Function pointer type for the user-defined function we expect to call on thread start.
	 */
	typedef void *(*UserFunction) (void *);

	/**
	 * Starts the thread with the given user function.
	 * @warn This function will fail if the thread has already been started.
	 * @warn Results are undefined if this is called more than once.
     * @param userFunction The function to call in the new thread.
	 * @param startData The data to pass into the new thread.
     * @return True if we were successful, false otherwise.
     */
	bool Start(UserFunction userFunction, void *startData);

	/**
	 * Gets if the thread is running.
     * @return True if thread is running, false otherwise.
     */
	bool IsRunning();

	/**
	 * Waits for this thread to terminate.
	 * @warn This function will fail if the thread has not been started.
	 * @param returnValue The value that the user's function returned.
     * @return True if successfully joined, false otherwise.
     */
	bool Join(void *returnValue);

	/**
	 * Sends cancellation signal to the thread.  It will terminate when it reaches a cancellation point.
	 * To be sure that the thread has been canceled, call Join() after calling Cancel().
	 * @warn Results undefined if the thread has not yet been started with Start().
	 * @warn The thread will still be counted as running until a successful Join().
     * @return True if sending cancellation signal successful, false otherwise.
     */
	bool Cancel();

private:
	/**
	 * Copying a thread doesn't really make sense, so we will make it private.
     */
	Thread(const Thread& orig);

	/**
	 * The pthread structure that this class wraps.
	 */
	pthread_t m_thread;

	/**
	 * Set to true if the thread is running.
	 */
	volatile bool m_running;
};

}
