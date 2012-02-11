cmake_minimum_required (VERSION 2.8)

include_directories(${CMAKE_SOURCE_DIR}/cs5721GraphicsLib/src)

# Complain more.
add_definitions("-Werror -Wall -pedantic")

# Set fastest possible code switch for release mode.
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast")


add_library(raytracerLib
  Basis.cpp Basis.h
  RaytraceException.cpp RaytraceException.h
  Scene.cpp Scene.h
  Ray.cpp Ray.h
  Sphere.cpp Sphere.h
  SolidShader.cpp SolidShader.h
  Color.cpp Color.h
  PerspectiveCamera.cpp PerspectiveCamera.h
  Triangle.cpp Triangle.h
  Box.cpp Box.h
  PointLight.cpp PointLight.h
)

target_link_libraries(raytracerLib cs5721Graphics)
