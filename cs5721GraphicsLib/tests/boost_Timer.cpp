/*
 *  test_Timer.cpp
 *
 *  Created by Pete Willemsen on 10/6/09.
 *  Copyright 2009 Department of Computer Science, University of Minnesota-Duluth. All rights reserved.
 *
 * This file is part of SIVE Lab library (libsive).
 *
 */

#include <iostream>
#include <cmath>
#include <vector>

#ifdef WIN32
#include <sys/types.h>
#include <fcntl.h>
#include <windows.h>
#include <winbase.h>
#endif

// #include "boost/timer/timer.hpp"  // most recent boost!
#include "boost/timer.hpp"

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

int main(int argc, char *argv[])
{
  // More recent versions of boost
  // boost::timer::auto_cpu_timer overallTime;

  sivelab::Random prng;

  std::vector<long> sleepTimes;

  for (int i=1; i<=10; i++)
    sleepTimes.push_back( i*10000 );

  for (int i=0; i<10; i++)
    sleepTimes.push_back( (int)floor(prng.uniform() * 10000) );

  long avgDiff = 0;

  boost::timer t0;
  for (unsigned int i=0; i<sleepTimes.size(); i++)
    {
      t0.restart();
      localSleepFunction( sleepTimes[i] );
      double diffTime = t0.elapsed() * 1.0e6;  // in microseconds
      std::cout << "Low-res timer result after usleeping for " << sleepTimes[i] << " microseconds --> result = " 
		<< diffTime << " us [diff = " << diffTime - sleepTimes[i] << "]" << std::endl;
      avgDiff += (diffTime - sleepTimes[i]);
    }

  avgDiff /= sleepTimes.size();
  std::cout << "Average difference between specified usleep and measured time: " << avgDiff << " us (" << avgDiff/1000000.0 << " s)" << std::endl << std::endl;

  exit(EXIT_SUCCESS);
}
