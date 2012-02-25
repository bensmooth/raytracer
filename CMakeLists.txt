cmake_minimum_required (VERSION 2.8)
project(raytracer)

# Include libxml2 stuff.
if(WIN32)
  include_directories("C:/Program Files/libxml2/include")
  link_directories("C:/Program Files/libxml2")
else()
  include_directories(/usr/include/libxml2)
endif()

add_subdirectory(cs5721GraphicsLib)
add_subdirectory(threadEngine)
add_subdirectory(raytracerLib)
add_subdirectory(basisTest)
add_subdirectory(raytracer)
