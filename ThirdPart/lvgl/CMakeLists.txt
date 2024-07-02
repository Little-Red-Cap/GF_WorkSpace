#file(GLOB_RECURSE SOURCES
#        ${CMAKE_CURRENT_LIST_DIR}/raw/lvgl/*.c*
#        ${CMAKE_CURRENT_LIST_DIR}/raw/lvgl/*.h*
#)
#
#include_directories(${CMAKE_CURRENT_LIST_DIR}/raw/lvgl)
#
##message("SOURCES: ${SOURCES}")
##message("GF_LIB_NAME: ${GF_LIB_NAME}")
#gf_add_sources(${GF_LIB_NAME} ${SOURCES})
#gf_add_include_directories(
#        ${CMAKE_CURRENT_LIST_DIR}/raw/
#        ${CMAKE_CURRENT_LIST_DIR}/raw/lvgl
#)

file(GLOB_RECURSE LVGL_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/raw/lvgl/*.c*
        ${CMAKE_CURRENT_LIST_DIR}/raw/lvgl/*.h*
)

add_library(lvgl STATIC
#add_library(${GF_LIB_NAME} INTERFACE
#        src/lv_core/lv_obj.c
#        src/lv_core/lv_style.c
#        ${CMAKE_CURRENT_LIST_DIR}/raw/lvgl/*.c*
#        raw/lvgl/src/core/lv_obj.c
        ${LVGL_SOURCES}
        # 添加其他源文件
)
