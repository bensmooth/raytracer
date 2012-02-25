// Mutex.cpp - written by Ben Hart

#include "Mutex.h"
#include <iostream>

RecMutex::RecMutex()
{
	static int s_idCounter = 0;

	// Set ID number.
	m_id = s_idCounter++;

	// Set debugging to false by default.
	m_debugInfo = false;

	if (m_debugInfo)
	{
		cout << "Mutex " << m_id << " created.\n";
	}

	// Create the mutex.
	if (pthread_mutex_init(&m_mutex, NULL) != 0)
	{
		cerr << "Error: unable to create mutex!\n";
	}

	// Wipe m_hasLock.
	ResetHasLock();

	// This is zero when we start because noone has called Lock() on the mutex yet.
	m_lockCount = 0;
}

RecMutex::~RecMutex()
{
	if (m_debugInfo)
	{
		cout << "Mutex " << m_id << " destroyed.\n";
	}

	// Wipe m_hasLock.
	ResetHasLock();

	// Destroy the mutex.
	pthread_mutex_destroy(&m_mutex);
}

void RecMutex::Lock()
{
	// Detect a deadlock condition: the same thread has called lock twice in a row.
	if (HasLock())
	{
		// Increase the lock count, since the same thread is trying to lock the object.
		m_lockCount++;

		// Simply avoid locking it again to avert a deadlock.
		if (m_debugInfo)
		{
			cout << "Mutex " << m_id << " enter (same thread): lock count is " << m_lockCount << endl;
		}

		return;
	}

	// If we got here, we are not the thead holding the lock.  Wait until that thread gets done.
	if (pthread_mutex_lock(&m_mutex) != 0)
	{
		cerr << "Error: Unable to lock mutex!\n";
		return;
	}
	// If we got here, we got into the mutex.
	m_lockCount++;

	// Store current thread's ID.
	m_hasLock = pthread_self();

	if (m_debugInfo)
	{
		cout << "Mutex " << m_id << " enter: lock count is " << m_lockCount << endl;
	}
}

void RecMutex::Unlock()
{
	// Make sure we actually have the lock.
	if (HasLock() == false)
	{
		cerr << "Mutex " << m_id << " error: thread that does not own the mutex tried to unlock mutex!\n";
		return;
	}

	// One less lock to go until we unlock the mutex.
	m_lockCount--;

	if (m_debugInfo)
	{
		cout << "Mutex " << m_id << " unlock: lock count is " << m_lockCount << endl;
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
			cerr << "Error: unable to unlock mutex!\n";
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
		cerr << "Mutex " << m_id << " error: we called unlock too many times!\n";
	}
}

bool RecMutex::HasLock()
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

void RecMutex::ResetHasLock()
{
	// This is the best way I have found to generate an invalid thread handle.
	memset(&m_hasLock, 0, sizeof (m_hasLock));
}

void RecMutex::SetDebug(bool debugging)
{
	m_debugInfo = debugging;
}
