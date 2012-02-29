cmake_minimum_required(VERSION 2.6)

find_package(Threads)

add_library (ThreadLib
  Mutex.cpp Mutex.h
  Thread.cpp Thread.h
  ThreadPool.cpp ThreadPool.h
)

target_link_libraries( ThreadLib ${CMAKE_THREAD_LIBS_INIT} )


add_executable(threadTest
  threadTest.cpp
)
target_link_libraries( threadTest ThreadLib )

add_executable(threadPoolTest
  threadPoolTest.cpp
)
target_link_libraries( threadPoolTest ThreadLib )

add_definitions( "-Wall -Werror -Wextra -Wno-unused-parameter -Wno-sign-compare" )
