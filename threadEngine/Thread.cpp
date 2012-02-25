/* 
 * File:   Thread.cpp
 * Author: Ben Hart
 * 
 * Created on November 21, 2011, 11:11 AM
 */

#include "Thread.h"

using namespace NetworkEngine;

Thread::Thread()
{
	m_running = false;
}


Thread::~Thread()
{
	Cancel();
}


Thread::Thread(const Thread& orig)
{
	// Copying a thread doesn't make sense.  Don't do anything.
}


bool Thread::Start(UserFunction userFunction, void *startData)
{
	return (pthread_create(&m_thread, NULL, userFunction, startData) == 0);
}


bool Thread::Join(void *returnValue)
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


bool Thread::Cancel()
{
	bool result = true;

	if (IsRunning())
	{
		result = pthread_cancel(m_thread) == 0;
	}

	return (result);
}


bool Thread::IsRunning()
{
	return (m_running);
}
