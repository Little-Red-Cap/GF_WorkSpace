# ESP32工具链定义,基于 esp-idf-v4.4/tools/cmake/toolchain-esp32.cmake 修改
set(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_C_COMPILER xtensa-esp32-elf-gcc)
set(CMAKE_CXX_COMPILER xtensa-esp32-elf-g++)
set(CMAKE_ASM_COMPILER xtensa-esp32-elf-gcc)

set(CMAKE_C_FLAGS "-mlongcalls -Wno-frame-address" CACHE STRING "C Compiler Base Flags")
set(CMAKE_CXX_FLAGS "-mlongcalls -Wno-frame-address" CACHE STRING "C++ Compiler Base Flags")

# IDF_PATH 环境变量为ESP-IDF的安装位置,执行export.sh(.bat)脚本即会定义
if(NOT $ENV{IDF_PATH} STREQUAL "")
    set(IDF_PATH $ENV{IDF_PATH})
endif()
if(NOT IDF_PATH)
    message(FATAL_ERROR "NOT DEFINE SDK PREFIX FOR CROSS COMIPILER")
endif()

# degnci 需要cJSON库支持,所以这里需要将ESP32 SDK中cJSON头文件夹加入搜索路径
include_directories(
        "${IDF_PATH}/components/json/cJSON"
)
