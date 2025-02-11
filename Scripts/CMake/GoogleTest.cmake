
# 添加 Google Test 库，CMake 3.11 以上支持 FetchContent
include(FetchContent)

# 下载 Google Test
FetchContent_Declare(
        gtest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG master  # 你可以指定特定的版本，例如 v1.11.0
)

# 下载并添加到项目中
FetchContent_MakeAvailable(gtest)

# 将测试代码编译为可执行文件
add_executable(test_my_project test_my_project.cpp)

# 将 Google Test 库链接到测试可执行文件
target_link_libraries(test_my_project gtest gtest_main)

# 在构建过程中启用测试
enable_testing()

# 添加测试目标
add_test(NAME MyTest COMMAND test_my_project)
