cmake_minimum_required (VERSION 2.8)
project (CS5721Graphics)

# Attempt to locate the PNG include directories, link directories and
# libraries.
find_package(PNG REQUIRED)
if (PNG_FOUND)
  MESSAGE(STATUS "Found png: ${PNG_INCLUDE_DIR}, ${PNG_LIBRARY}")
  INCLUDE_DIRECTORIES(${PNG_INCLUDE_DIR})
endif (PNG_FOUND)

# If you want to get more info on what FindBoost is doing...
# SET(Boost_DEBUG 1)

# Necessary to get Win32 version to work.
SET(Boost_USE_STATIC_LIBS ON)

FIND_PACKAGE(Boost REQUIRED)
IF(${Boost_FOUND})
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
ENDIF()

FIND_PACKAGE(Boost COMPONENTS program_options REQUIRED)
IF(${Boost_PROGRAM_OPTIONS_FOUND})
  MESSAGE(${Boost_PROGRAM_OPTIONS_LIBRARIES})
  # TARGET_LINK_LIBRARIES(boostThreadExample 
ENDIF()

IF(WIN32)
INCLUDE_DIRECTORIES("C:/Program Files/libxml2/include")
LINK_DIRECTORIES("C:/Program Files/libxml2")
ELSE()
INCLUDE_DIRECTORIES(/usr/include/libxml2)
ENDIF()

add_subdirectory(src)
add_subdirectory(tests)


