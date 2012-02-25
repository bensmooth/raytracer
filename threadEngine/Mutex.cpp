// Mutex.cpp - written by Ben Hart

#include <stdio.h>
#include "Mutex.h"


ThreadEngine::Mutex::Mutex()
{
	static int s_idCounter = 0;

	// Set ID number.
	m_id = s_idCounter++;

	// Set debugging to false by default.
	m_debugInfo = false;

	if (m_debugInfo)
	{
		printf("Mutex %i created.\n", m_id);
	}

	// Create the mutex.
	if (pthread_mutex_init(&m_mutex, NULL) != 0)
	{
		printf("Error: unable to create mutex!\n");
	}

	// Wipe m_hasLock.
	ResetHasLock();

	// This is zero when we start because noone has called Lock() on the mutex yet.
	m_lockCount = 0;
}

ThreadEngine::Mutex::~Mutex()
{
	if (m_debugInfo)
	{
		printf("Mutex %i destroyed.\n", m_id);
	}

	// Wipe m_hasLock.
	ResetHasLock();

	// Destroy the mutex.
	pthread_mutex_destroy(&m_mutex);
}

void ThreadEngine::Mutex::Lock()
{
	// Detect a deadlock condition: the same thread has called lock twice in a row.
	if (HasLock())
	{
		// Increase the lock count, since the same thread is trying to lock the object.
		m_lockCount++;

		// Simply avoid locking it again to avert a deadlock.
		if (m_debugInfo)
		{
			printf("Mutex %i enter (same thread): lock count is %i\n", m_id, m_lockCount);
		}

		return;
	}

	// If we got here, we are not the thead holding the lock.  Wait until that thread gets done.
	if (pthread_mutex_lock(&m_mutex) != 0)
	{
		printf("Error: Unable to lock mutex!\n");
		return;
	}
	// If we got here, we got into the mutex.
	m_lockCount++;

	// Store current thread's ID.
	m_hasLock = pthread_self();

	if (m_debugInfo)
	{
		printf("Mutex %i enter (same thread): lock count is %i\n", m_id, m_lockCount);
	}
}

void ThreadEngine::Mutex::Unlock()
{
	// Make sure we actually have the lock.
	if (HasLock() == false)
	{
		printf("Mutex %i error: thread that does not own the mutex tried to unlock mutex!\n", m_id);
		return;
	}

	// One less lock to go until we unlock the mutex.
	m_lockCount--;

	if (m_debugInfo)
	{
		printf("Mutex %i exit: lock count is %i\n", m_id, m_lockCount);
	}

	// Decide if we can unlock the mutex.
	if (m_lockCount == 0)
	{
		// Since we have called Unlock() as many times as we have called Lock(),
		// we can unlock the mutex.

		// Clear thread ID that has lock.
		ResetHasLock();

		if (pthread_mutex_unlock(&m_mutex) != 0)
		{
			printf("Error: unable to unlock mutex!\n");
		}
	}
	else if (m_lockCount > 0)
	{
		// We still have to call unlock a few more times.
		// Pretty much do nothing.
	}
	else
	{
		// If we got here, m_lockCount is negative.  This means we called Unlock() too much.
		printf("Mutex %i error: we called unlock too many times!\n", m_id);
	}
}

bool ThreadEngine::Mutex::HasLock()
{
	pthread_t callingThread = pthread_self();
	if (pthread_equal(callingThread, m_hasLock))
	{
		return (true);
	}
	else
	{
		return (false);
	}
}

void ThreadEngine::Mutex::ResetHasLock()
{
	// This is the best way I have found to generate an invalid thread handle.
	memset(&m_hasLock, 0, sizeof (m_hasLock));
}

void ThreadEngine::Mutex::SetDebug(bool debugging)
{
	m_debugInfo = debugging;
}
