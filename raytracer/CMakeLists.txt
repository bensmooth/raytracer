cmake_minimum_required ( VERSION 2.8 )

Project(raytracer)

include_directories(${CMAKE_SOURCE_DIR}/cs5721GraphicsLib/src)
include_directories(${CMAKE_SOURCE_DIR}/raytracerLib)

add_executable(raytracer
  main.cpp
)

target_link_libraries(raytracer raytracerLib)
