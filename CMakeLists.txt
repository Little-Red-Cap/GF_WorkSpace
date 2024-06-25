cmake_minimum_required(VERSION 3.28)

set(PROJECT_NAME GF_WorkSpace)
project(${PROJECT_NAME} LANGUAGES C CXX ASM)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 20)


add_subdirectory(apps)
add_subdirectory(Modules)
add_subdirectory(Script/CMake)
add_subdirectory(ThirdPart)

message(STATUS "Compiler: ${CMAKE_C_COMPILER_ID}")
message(STATUS "\tCC: ${CMAKE_C_COMPILER}")
message(STATUS "\tCXX: ${CMAKE_CXX_COMPILER}")
message(STATUS "\tASM: ${CMAKE_ASM_COMPILER}")
message(STATUS "\tOBJCOPY: ${CMAKE_OBJCOPY}")
message(STATUS "\tOBJDUMP: ${CMAKE_OBJDUMP}")
message(STATUS "\tSIZE: ${CMAKE_SIZE}")

# 手动排除不需要的文件
#list(FILTER FATFS_SRCS EXCLUDE REGEX "ff15/source/deprecated/.*")

# 使用 aux_source_directory 收集指定目录下的所有源文件，但不递归子目录
#aux_source_directory(ff15/source FATFS_SRCS)
