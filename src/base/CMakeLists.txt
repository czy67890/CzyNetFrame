file(GLOB LOCAL_SRC_LIST *.cc)
#设置库的输出路径
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
add_library(base_shared SHARED ${LOCAL_SRC_LIST})
#设置输出库的名称,以得到libbase_shared.so 和libbase_shared.lib两个文件
add_library(base_shared_static STATIC ${LOCAL_SRC_LIST})
set_target_properties(base_shared_static PROPERTIES OUTPUT_NAME "base_shared")
