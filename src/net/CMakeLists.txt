FILE(GLOB NET_SRC *.cc)
add_library(net ${NET_SRC})
target_link_libraries(net base_shared_static)
include_directories({PROJECT_SOURCE_DIR})