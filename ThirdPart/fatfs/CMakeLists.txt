add_compile_definitions(_TCHAR_DEFINED)

file(GLOB_RECURSE FATFS_SRCS
    ff15/source/*.c
)
include_directories(ff15/source)

add_library(fatfs_lib ${FATFS_SRCS})
##target_include_directories(fatfs PUBLIC ff15/source)
target_include_directories(fatfs_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ff15/source)

#message(STATUS "FatFs library sources: ${FATFS_SRCS}")
#message(STATUS "FatFs library include directories: ${CMAKE_CURRENT_SOURCE_DIR}/ff15/source")
#
##set_source_files_properties(${FATFS_SRCS} PROPERTIES INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/ff15/source)
#set(FATFS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ff15/source)
#
#set(FATFS_SRCS ${FATFS_SRCS} PARENT_SCOPE)
##set(SRCS ${FATFS_SRCS} PARENT_SCOPE)
