#CMAKE 版本检查
cmake_minimum_required(VERSION 2.8)
#设定文件名
#set (BuildType "Debug" CACHE "")
PROJECT(czyServer)
SET(CMAKE_BUILD_TYPE Debug)
include_directories(${PROJECT_SOURCE_DIR})
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-lpthread" )
SET(CMAKE_CXX_STANDARD 20)
if(${CMAKE_BUILD_TYPE} MATCHES "Release")
    MESSAGE(STATUS "build with release")
    set(BuildType "release")
else()
    MESSAGE(STATUS "build with debug")
    set(BuildType "debug")
endif()    
set(RELEASE_DIR ${PROJECT_BINARY_DIR}/$${BuildType})
#添加待处理的子目录
add_subdirectory(src)
#设置可执行文件以及库文件输出目录
#使用动态库需要加上此选项生成与位置无关的代码
add_compile_options(-fPIC)
MESSAGE(STATUS "THIS IS BINARY_DIR" ${PROJECT_BINARY_DIR})
MESSAGE(STATUS "THIS IS SOURCE_DIR" ${PROJECT_SOURCE_DIR})
