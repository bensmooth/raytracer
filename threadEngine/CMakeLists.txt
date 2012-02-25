cmake_minimum_required(VERSION 2.6)

find_package(Threads)

add_library (ThreadLib
  Mutex.cpp Mutex.h
  Thread.cpp Thread.h
)

target_link_libraries( ThreadLib ${CMAKE_THREAD_LIBS_INIT} )

add_definitions( "-Wall -Werror -Wextra -Wno-unused-parameter -Wno-sign-compare" )
