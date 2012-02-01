cmake_minimum_required (VERSION 2.8)

include_directories(${CMAKE_SOURCE_DIR}/cs5721GraphicsLib/src)

add_library(raytracerLib
  Basis.cpp Basis.h
)

target_link_libraries(raytracerLib cs5721Graphics)
