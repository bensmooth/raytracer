cmake_minimum_required ( VERSION 2.8 )

Project(raytracer)

include_directories(${CMAKE_SOURCE_DIR}/cs5721GraphicsLib/src)
include_directories(${CMAKE_SOURCE_DIR}/raytracerLib)

add_executable(raytracer
  main.cpp
)

FIND_PACKAGE(Boost COMPONENTS program_options)
target_link_libraries(raytracer raytracerLib)
target_link_libraries(raytracer ${Boost_PROGRAM_OPTIONS_LIBRARIES})
target_link_libraries(raytracer ${PNG_LIBRARY})
target_link_libraries(raytracer xml2)
target_link_libraries(raytracer rt)

add_definitions("-g")
