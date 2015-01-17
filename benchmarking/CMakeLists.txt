cmake_minimum_required ( VERSION 2.8 )

Project(raytracer)

add_subdirectory(hayai/)

include_directories(${CMAKE_SOURCE_DIR}/cs5721GraphicsLib/src)
include_directories(${CMAKE_SOURCE_DIR}/raytracerLib)
include_directories(${CMAKE_SOURCE_DIR}/threadEngine)
include_directories(${CMAKE_SOURCE_DIR}/benchmarking/hayai)


# Complain more.
add_definitions("-Werror -Wall -pedantic")

# Set fastest possible code switch for release mode.
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast")

add_executable(benchmarker
  renderTiming.cpp
)

target_link_libraries(benchmarker
  hayai_main
  ${LIB_TIMING}
)


target_link_libraries(benchmarker raytracerLib)
target_link_libraries(benchmarker ${PNG_LIBRARY})
target_link_libraries(benchmarker xml2)
target_link_libraries(benchmarker rt)
