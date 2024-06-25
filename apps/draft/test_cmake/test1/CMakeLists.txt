# test1/CMakeLists.txt
add_executable(test1 main.cpp)
target_link_libraries(test1 common)


if (WIN32)
    add_definitions(-D_WIN32_WINNT=0x0601)
endif ()

if (WIN32)
    set(CMAKE_MFC_FLAG 2)
    add_definitions(-D_AFXDLL)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
ENDIF ()
