if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    message(STATUS "build with debug")
endif()
SET(LIBRARY_OUTPUT_DIRECTORY ${RELEASE_DIR})
SET(SRC_LIST main.cc)
include_directories(${PROJECT_SOURCE_DIR}/src)
add_subdirectory(base)
add_subdirectory(net)
add_executable(server ${SRC_LIST} )
target_link_libraries(server  net)
INSTALL(TARGETS server  RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin)