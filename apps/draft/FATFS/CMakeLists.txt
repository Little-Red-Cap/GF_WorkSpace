set(this_target_name FATFS_example)



add_executable(${this_target_name}
        main.c
        ${FATFS_SRCS}
)

include_directories()

#message(STATUS "Beginning to build ${this_target_name} in ${CMAKE_CURRENT_BINARY_DIR}")
#message(STATUS "Beginning to build ${this_target_name} in ${CMAKE_CURRENT_SOURCE_DIR}")
#message(STATUS "Beginning to ${SRCS}")


message(STATUS "Files: ${FATFS_SRCS}")
#message(STATUS "FatFs library source directory: ${FATFS_SOURCE_DIR}")

target_link_libraries(${this_target_name} fatfs_lib)
#target_include_directories(this_project_name PRIVATE ${CMAKE_SOURCE_DIR}/ff15/source)
