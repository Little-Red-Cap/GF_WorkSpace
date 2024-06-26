set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.28)

# specify cross-compilers and tools
set(platform "arm-none-eabi")
#set(platform "clang")
include(Script/CMake/compilers.cmake)

# project settings
set(PROJECT_NAME GF_WorkSpace)
project(${PROJECT_NAME} LANGUAGES C CXX ASM)

#set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

print_compiler_info()


add_subdirectory(apps)
add_subdirectory(Modules)
add_subdirectory(Script/CMake)
add_subdirectory(ThirdPart)


# 手动排除不需要的文件
#list(FILTER FATFS_SRCS EXCLUDE REGEX "ff15/source/deprecated/.*")

# 使用 aux_source_directory 收集指定目录下的所有源文件，但不递归子目录
#aux_source_directory(ff15/source FATFS_SRCS)
