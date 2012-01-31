#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>

// Needed for the boost system
#include <boost/thread.hpp>

#ifdef WIN32
#include <sys/types.h>
#include <fcntl.h>
#include <windows.h>
#include <winbase.h>
#endif

#include "Random.h"

void localSleepFunction(long stime)
{
#ifdef WIN32
  // From http://filipivianna.blogspot.com/2010/07/usleep-on-windows-win32.html
  __int64 time1 = 0, time2 = 0, sysFreq = 0;

  QueryPerformanceCounter((LARGE_INTEGER *)&time1);
  QueryPerformanceFrequency((LARGE_INTEGER *)&sysFreq);
  do{
    QueryPerformanceCounter((LARGE_INTEGER *)&time2);
    
    //  }while((((time2-time1)*1.0)/sysFreq)<waitTime);
  }while( (time2-time1) < stime);
#else
  usleep( stime );
#endif
}

const int gbl_numThreads = 10;
const int gbl_N = 100;

class threadData
{
public:
  threadData() 
    : counter(0) 
  {
  }

  int counter;
  boost::mutex local_mutex;
};

class WorkerThread
{
public:
  
  WorkerThread(threadData *td)
    : m_threadData(td)
  {
  }

  void operator()()  
  {  
    sivelab::Random prng;
    for (int li=0; li<gbl_N; li++)
      {
	// wait a random amount
	localSleepFunction( (int)(prng.uniform() * 10000) );
	
	m_threadData->local_mutex.lock();
	m_threadData->counter++;
	m_threadData->local_mutex.unlock();
    }

  // exit the thread
  std::cout << "Exiting thread" << std::endl;
  
  // The return of this function via "return 0" is identical to
  // calling a pthread_exit call and the return value is used as the
  // exit status, so return 0 is identical to:
  // pthread_exit(NULL);  
  }

private:
  threadData *m_threadData;
};


int main( int argc, char *argv[] ) 
{
  // Allocate one copy of the data that will be shared between all the
  // threads.
  threadData *data = new threadData;

  // Create threads and store their IDs in the pthread_t array below.
  int numThreads = gbl_numThreads;
  std::vector<boost::thread*> threads(numThreads);

  for (int threadNum=0; threadNum < numThreads; threadNum++)
    {
      WorkerThread wt(data);
      threads[threadNum] = new boost::thread(wt);
      std::cout << "Starting thread, number=" << threads[threadNum]->get_id() << std::endl;
    }

  // The main trunk of the application should wait for each thread to
  // complete before exiting, else all threads be lost.  The
  // join function allows this to occur.
  for (int threadNum=0; threadNum < numThreads; threadNum++)
    {
      threads[threadNum]->join();
    }

  int totalCount = gbl_N * gbl_numThreads;
  if (data->counter == totalCount)
    std::cout << "SUCCESS!  Final data counter = " << data->counter << std::endl;
  else 
    std::cout << "FAIL! Data counter = " << data->counter << ", but should have been " << totalCount << std::endl;

  return 0;
}
