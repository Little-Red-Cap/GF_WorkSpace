
set(this_target_name Hi-Link_FPM383C_80X64)

message("this_target_name: ${this_target_name}")

# 设置头文件搜索路径
include_directories(
        "hilink_src"
)

# 将子目录中的所有.c和.h文件收集到一个变量中
file(GLOB HILINK_SRC_FILES
        "hilink_src/*.c"
        "hilink_src/*.h"
)

# 添加可执行文件或库的源文件
add_executable(${this_target_name}
        main.cpp
        ${HILINK_SRC_FILES}
)

## 设置链接库
#target_link_libraries(FPM0XX-democode_test_project hilink_src/libhilink.a)
#
## 设置输出目录
#set_target_properties(FPM0XX-democode_test_project PROPERTIES OUTPUT_NAME "FPM0XX-democode_test_project")
#set_target_properties(FPM0XX-${this_target_name} PROPERTIES OUTPUT_NAME ${this_target_name})
#set_target_properties(FPM0XX-democode_test_project PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
#set_target_properties(FPM0XX-democode_test_project PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
#set_target_properties(FPM0XX-democode_test_project PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# 设置编译选项
#target_compile_options(FPM0XX-democode_test_project PUBLIC -Wall -Wextra -Werror)

# 设置链接选项
#target_link_options(FPM0XX-democode_test_project PUBLIC -Wl,--whole-archive -lstdc++ -Wl,--no-whole-archive)

# 设置目标文件类型为可执行文件
#set_target_properties(FPM0XX-democode_test_project PROPERTIES LINKER_LANGUAGE CXX)
#set_target_properties(FPM0XX-democode_test_project PROPERTIES CXX_STANDARD 11)
#set_target_properties(FPM0XX-democode_test_project PROPERTIES CXX_STANDARD_REQUIRED ON)
#set_target_properties(FPM0XX-democode_test_project PROPERTIES CXX_EXTENSIONS OFF)

# 设置目标文件类型为库文件
#add_library(FPM0XX-democode_test_project SHARED main.cpp)
#target_link_libraries(FPM0XX-democode_test_project hilink_src/libhilink.a)
#set_target_properties(FPM0XX-democode_test_project PROPERTIES LINKER_LANGUAGE CXX)
#set_target_properties(FPM0XX-democode_test_project PROPERTIES CXX_STANDARD 11)
#set_target_properties(FPM0XX-democode_test_project PROPERTIES CXX_STANDARD_REQUIRED ON)
#set_target_properties(FPM0XX-democode_test_project PROPERTIES CXX_EXTENSIONS OFF)

# 设置目标文件类型为可执行文件
#add_executable(FPM0XX-democode_test_project main.cpp)
#target_link_libraries(FPM0XX-democode_test_project hilink_src/libhilink.a)
#set_target_properties(FPM0XX-democode_test_project PROPERTIES LINKER_LANGUAGE CXX)
#set_target_properties(FPM0XX-democode_test_project PROPERTIES CXX_STANDARD 11)
#set_target_properties(FPM0XX-democode_test_project PROPERTIES CXX_STANDARD_REQUIRED ON)
#set_target_properties(FPM0XX-democode_test_project PROPERTIES CXX_EXTENSIONS OFF)
