file(GLOB_RECURSE HILINK_FPM_SRCS
    FPM383C-80x64/*.c
)
#include_directories(ff15/source)

#add_library(fatfs_lib ${FATFS_SRCS})
###target_include_directories(fatfs PUBLIC ff15/source)
#target_include_directories(fatfs_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ff15/source)
