/* 
 * File:   Thread.cpp
 * Author: Ben Hart
 * 
 * Created on November 21, 2011, 11:11 AM
 */

#include "Thread.h"

ThreadEngine::Thread::Thread()
{
	m_running = false;
}


ThreadEngine::Thread::~Thread()
{
	Cancel();
}


ThreadEngine::Thread::Thread(const Thread& orig)
{
	// Copying a thread doesn't make sense.  Don't do anything.
}


bool ThreadEngine::Thread::Start(UserFunction userFunction, void *startData)
{
	if (pthread_create(&m_thread, NULL, userFunction, startData) == 0)
	{
		m_running = true;
		return (true);
	}
	else
	{
		return (false);
	}
}


bool ThreadEngine::Thread::Join(void *returnValue)
{
	if (IsRunning())
	{
		int result = pthread_join(m_thread, &returnValue);

		if (result == 0)
		{
			// Success.
			m_running = false;
			return (true);
		}
		else
		{
			return (false);
		}
	}
	else
	{
		return (false);
	}
}


bool ThreadEngine::Thread::Cancel()
{
	bool result = true;

	if (IsRunning())
	{
		result = pthread_cancel(m_thread) == 0;
	}

	return (result);
}


bool ThreadEngine::Thread::IsRunning()
{
	return (m_running);
}
