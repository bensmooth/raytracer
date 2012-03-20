cmake_minimum_required (VERSION 2.8)

include_directories(${CMAKE_SOURCE_DIR}/cs5721GraphicsLib/src)
include_directories(${CMAKE_SOURCE_DIR}/threadEngine)

# Complain more.
add_definitions("-Werror -Wall -pedantic")

# Set fastest possible code switch for release mode.
# TODO: When raytracer is more mature, see if this actually makes a difference.
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast")


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
  CosineShader.cpp CosineShader.h
  ReferenceTileShader.cpp ReferenceTileShader.h
  BlinnPhongShader.cpp BlinnPhongShader.h
  GlazeShader.cpp GlazeShader.h
  Cylinder.cpp Cylinder.h
  Perlin.c Perlin.h
  PerlinShader.cpp PerlinShader.h
  ColorGradient.cpp ColorGradient.h
)

add_executable(perlinTest
  PerlinTest.c
)

set(CMAKE_C_FLAGS "-std=gnu99")

target_link_libraries(raytracerLib cs5721Graphics)
target_link_libraries(raytracerLib ThreadLib)
target_link_libraries(perlinTest raytracerLib)
