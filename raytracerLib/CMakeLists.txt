cmake_minimum_required (VERSION 2.8)

include_directories(${CMAKE_SOURCE_DIR}/cs5721GraphicsLib/src)

add_library(raytracerLib
  Basis.cpp Basis.h
  RaytraceException.cpp RaytraceException.h
  Scene.cpp Scene.h
  Ray.cpp Ray.h
  Sphere.cpp Sphere.h
  SolidShader.cpp SolidShader.h
  Color.cpp Color.h
  PerspectiveCamera.cpp PerspectiveCamera.h
)

target_link_libraries(raytracerLib cs5721Graphics)

add_definitions("-g")
