
# Add the lv_port_pc_cmake app to the build
#add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/lv_port_pc_cmake)

set(this_target_name lv_port_pc_cmake)

file(GLOB_RECURSE SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/*.c*
        ${CMAKE_CURRENT_LIST_DIR}/*.h*
)

#include_directories(${CMAKE_CURRENT_LIST_DIR})
#target_include_directories(${this_target_name} PUBLIC
#        ${CMAKE_CURRENT_LIST_DIR}/*.cpp
##        ${CMAKE_CURRENT_LIST_DIR}/lv_port_pc_cmake
#)


add_executable(${this_target_name} ${SOURCES})
