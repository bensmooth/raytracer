#include <iostream>
#include <cstdlib>
#include <cstring>

// Needed for the pthread system
#include <pthread.h>

const int gbl_numThreads = 10;
const int gbl_N = 100;

class threadData
{
public:
  threadData() : counter(0) 
  {
    pthread_mutex_init( &local_mutex, NULL );    
  }

  int counter;
  pthread_mutex_t local_mutex;
};

void *thread_ProcessingFunction(void *data_ptr)
{
  threadData *data = (threadData*)data_ptr;

  pthread_mutex_lock( &(data->local_mutex) );
  // The IOStream cout is thread-safe... if you don't mutex around the
  // cout calls, your output will be interleaved.
  std::cout << "Starting thread, number=" << pthread_self() << std::endl;
  pthread_mutex_unlock( &(data->local_mutex) );

  for (int li=0; li<gbl_N; li++)
    {
      // wait a random amount
      usleep( (int)(drand48() * 10000) );

      pthread_mutex_lock( &(data->local_mutex) );
      data->counter++;
      pthread_mutex_unlock( &(data->local_mutex) );
    }

  // exit the thread
  std::cout << "Exiting thread, number=" << pthread_self() << std::endl;
  
  // The return of this function via "return 0" is identical to
  // calling a pthread_exit call and the return value is used as the
  // exit status, so return 0 is identical to:
  pthread_exit(NULL);  
}

int main( int argc, char *argv[] ) 
{
  // Allocate one copy of the data that will be shared between all the
  // threads.
  threadData *data = new threadData;

  // pthreads can have various attributes that control their behavior.
  // These are specified through pthread_attr_t values.  These are
  // specified in the second argument to the pthread create function.
  // Here's an example that creates threads that are joinable (the
  // default, but still an example):
  pthread_attr_t pthreadAttributes;
  pthread_attr_init(&pthreadAttributes);
  pthread_attr_setdetachstate(&pthreadAttributes, PTHREAD_CREATE_JOINABLE);

  // Create threads and store their IDs in the pthread_t array below.
  int numThreads = gbl_numThreads;
  pthread_t *threads = new pthread_t[numThreads];

  for (int threadNum=0; threadNum < numThreads; threadNum++)
    {
      int rtn_status = pthread_create(&threads[threadNum], 
				      (const pthread_attr_t*)&pthreadAttributes, 
				      thread_ProcessingFunction, 
				      (void*)data);
      if (rtn_status)
	{
	  std::cerr << "pthread creation failed: " << strerror(rtn_status) << std::endl;
	  exit(-1);
	}
    }

  // The main trunk of the application should wait for each thread to
  // complete before exiting, else all threads be lost.  The
  // pthread_join function allows this to occur.  Data passed into the
  // pthread_exit call is picked up by the join function and can be
  // inspected/used at that point.  In these examples, we pass NULL to
  // pthread_exit and don't expect a value from the pthread_join.
  for (int threadNum=0; threadNum < numThreads; threadNum++)
    {
      pthread_join(threads[threadNum], NULL);
    }

  int totalCount = gbl_N * gbl_numThreads;
  if (data->counter == totalCount)
    std::cout << "SUCCESS!  Final data counter = " << data->counter << std::endl;
  else 
    std::cout << "FAIL! Data counter = " << data->counter << ", but should have been " << totalCount << std::endl;

  return 0;
}
