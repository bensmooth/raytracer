cmake_minimum_required (VERSION 2.8)
project(raytracer)

# Find libxml2 stuff.
IF(WIN32)
INCLUDE_DIRECTORIES("C:/Program Files/libxml2/include")
LINK_DIRECTORIES("C:/Program Files/libxml2")
ELSE()
INCLUDE_DIRECTORIES(/usr/include/libxml2)
ENDIF()

add_subdirectory(cs5721GraphicsLib)
add_subdirectory(raytracerLib)
add_subdirectory(basisTest)
add_subdirectory(raytracer)
