# test_windows_app/CMakeLists.txt
set(this_target_name test_windows_app)
add_executable(${this_target_name} main.cpp)
#target_link_libraries(test_windows_app common)

#参考链接：
#https://blog.csdn.net/weixin_43344151/article/details/134888394
#使CLion支持Windows编程和MFC 编程

#if (WIN32)
#    add_definitions(-D_WIN32_WINNT=0x0601)
#endif ()
#
#if (WIN32)
#    set(CMAKE_MFC_FLAG 2)
#    add_definitions(-D_AFXDLL)
#    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
#ENDIF ()


