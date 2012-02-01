cmake_minimum_required ( VERSION 2.8 )

Project(graphLibTest)

include_directories(${CMAKE_SOURCE_DIR}/cs5721GraphicsLib/src)
include_directories(${CMAKE_SOURCE_DIR}/raytracerLib)

add_executable(basisTest
  main.cpp
)

#target_link_libraries(basisTest cs5721Graphics)
target_link_libraries(basisTest raytracerLib)
