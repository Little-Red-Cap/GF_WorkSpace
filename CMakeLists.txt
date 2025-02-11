set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.30)

set(GF_LIB_NAME gf)
add_library(${GF_LIB_NAME} INTERFACE)

function(gf_add_sources)
    target_sources(${GF_LIB_NAME} INTERFACE ${ARGN})
endfunction()
function(gf_add_include_directories)
    target_include_directories(${GF_LIB_NAME} INTERFACE ${ARGN})
endfunction()

# target define
#set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

# specify cross-compilers and tools
#set(platform "arm-none-eabi")
set(platform "clang")
include(Scripts/CMake/compilers.cmake)

# project settings
set(PROJECT_NAME GF_WorkSpace)
project(${PROJECT_NAME} LANGUAGES C CXX ASM)

if(NOT CMAKE_BUILD_TYPE)
#    set(CMAKE_BUILD_TYPE Debug)
#    set(CMAKE_BUILD_TYPE Release)
#    set(CMAKE_BUILD_TYPE MinSizeRel)
#    set(CMAKE_BUILD_TYPE RelWithDebInfo)
    message(STATUS "Setting build type to 'Debug' as none was specified.")
endif()

print_compiler_info()
print_language_standard_info()


add_subdirectory(apps)
add_subdirectory(Modules)
add_subdirectory(Scripts/CMake)
add_subdirectory(ThirdPart)

message("-- Build Type: ${CMAKE_BUILD_TYPE}")
message("-- Platform: ${platform}")
message("-- Compiler: ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
message("target sources: ${CMAKE_C_SOURCES}")

# 手动排除不需要的文件
#list(FILTER FATFS_SRCS EXCLUDE REGEX "ff15/source/deprecated/.*")

# 使用 aux_source_directory 收集指定目录下的所有源文件，但不递归子目录
#aux_source_directory(ff15/source FATFS_SRCS)
