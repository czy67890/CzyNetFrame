FILE(GLOB NET_SRC *.cc)
add_library(net ${NET_SRC})
include_directories({PROJECT_SOURCE_DIR})