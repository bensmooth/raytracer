cmake_minimum_required ( VERSION 2.8 )

Project(raytracer)

include_directories(${CMAKE_SOURCE_DIR}/cs5721GraphicsLib/src)
include_directories(${CMAKE_SOURCE_DIR}/raytracerLib)

# Complain more.
add_definitions("-Werror -Wall -pedantic")

# Set fastest possible code switch for release mode.
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast")

add_executable(raytracer
  main.cpp
)

find_package(Boost COMPONENTS program_options)
target_link_libraries(raytracer raytracerLib)
target_link_libraries(raytracer ${Boost_PROGRAM_OPTIONS_LIBRARIES})
target_link_libraries(raytracer ${PNG_LIBRARY})
target_link_libraries(raytracer xml2)
target_link_libraries(raytracer rt)
