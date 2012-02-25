// Mutex.h - written by Ben Hart

#pragma once
#include <pthread.h>
#include <iostream>
#include <string.h>

using namespace std;

/**
 * This class represents a recursive mutex.  The recursive part comes from the property
 * that Lock() may be called mutliple times from the same thread without deadlocking,
 * and that the mutex will not unlock for other threads until Unlock() is called once
 * for each call to Lock().
 */
class RecMutex
{
public:
	/**
	 * Creates the mutex.
	 * @param noisy Set this to true if you want this object to print debugging messages.
	 */
	RecMutex();


	/**
	 * Destroys the mutex.  Results are undefined when mutex is still locked!
	 */
	~RecMutex();


	/**
	 * Locks the mutex.  Other threads will block until the thread in the mutex
	 * calls Unlock().  If Lock() is called twice from the same thread, you will
	 * have to call Unlock() in the thread the same number of times before other
	 * threads can get in the mutex.
	 * @see Unlock
	 */
	void Lock();


	/**
	 * Unlocks the mutex.
	 * @see Lock
	 */
	void Unlock();


	/**
	 * Tests if the current thread owns the lock on this mutex.
	 * @return True if current thread owns lock, false otherwise.
	 */
	bool HasLock();


	/**
	 * Sets if we are printing out debugging info.
	 * @param debugging True if we want to print debugging info, false otherwise.
	 */
	void SetDebug(bool debugging);


private:

	/**
	 * Resets the m_hasLock thread ID to its default invalid value.
	 */
	void ResetHasLock();


	/**
	 * This is the thread that is locking the mutex.
	 * It will be cleared to 0's when the mutex is unlocked.
	 * Used to detect a deadlock.
	 */
	pthread_t m_hasLock;


	/**
	 * This is the number of times the locking thread has to call Unlock() before
	 * other threads can get the mutex.  When it is 0, other threads can get in.
	 */
	int m_lockCount;


	/**
	 * This is the actual pthreads mutex that this class wraps.
	 */
	pthread_mutex_t m_mutex;

	/**
	 * Set to true if we are printing out debugging information.
	 */
	bool m_debugInfo;

	int m_id;
};
