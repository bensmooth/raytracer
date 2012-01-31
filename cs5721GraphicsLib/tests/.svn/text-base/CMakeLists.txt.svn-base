include_directories(${CMAKE_SOURCE_DIR}/src)

FIND_PACKAGE(Boost COMPONENTS thread REQUIRED)
IF(${Boost_THREAD_FOUND})
  ADD_EXECUTABLE (boostThreadExample
    boostThreadExample.cpp
    )
  TARGET_LINK_LIBRARIES(boostThreadExample cs5721Graphics)
  TARGET_LINK_LIBRARIES(boostThreadExample ${Boost_THREAD_LIBRARIES})
  TARGET_LINK_LIBRARIES(boostThreadExample pthread)
ENDIF()

FIND_PACKAGE(Threads)
IF(THREADS_FOUND)
ADD_EXECUTABLE (pthreadsExample
  pthreadsExample.cpp
)
TARGET_LINK_LIBRARIES(pthreadsExample ${CMAKE_THREAD_LIBS_INIT})
ENDIF()

IF(${Boost_FOUND})
  ADD_EXECUTABLE (boost_Timer
    boost_Timer.cpp
    )
  TARGET_LINK_LIBRARIES(boost_Timer cs5721Graphics)
  TARGET_LINK_LIBRARIES(boost_Timer ${Boost_LIBRARIES})
ENDIF()


add_executable(test_Timer
  test_Timer.cpp
)
target_link_libraries(test_Timer cs5721Graphics)


add_executable(test_pngWrite
  test_pngWrite.cpp
)
target_link_libraries(test_pngWrite cs5721Graphics)
target_link_libraries(test_pngWrite ${Boost_PROGRAM_OPTIONS_LIBRARIES})
target_link_libraries(test_pngWrite ${PNG_LIBRARY})
target_link_libraries(test_pngWrite z)



add_executable(test_Vector3D
  test_Vector3D.cpp
)
target_link_libraries(test_Vector3D cs5721Graphics)


add_executable(test_XMLSceneParse
  test_XMLSceneParse.cpp
)
target_link_libraries(test_XMLSceneParse cs5721Graphics)
target_link_libraries(test_XMLSceneParse ${Boost_PROGRAM_OPTIONS_LIBRARIES})
target_link_libraries(test_XMLSceneParse xml2)


