set(this_target_name untitled_target)

message("using target name: ${this_target_name}")

# 设置头文件搜索路径
include_directories(
        "./"
)

# 将子目录中的所需源文件收集到一个变量中
file(GLOB SRC_FILES
        "./*.c"
        "./*.h"

        "./*.cpp"
        "./*.hpp"

        "./*.cc"
        "./*.hh"

        "./*.cxx"
        "./*.hxx"
)

# 添加可执行文件或库的源文件
add_executable(${this_target_name}
        main.cpp
        ${SRC_FILES}
)
