include(FetchContent)   # CMake 3.11 or later required
# FetchContent_MakeAvailable was not added until CMake 3.14
if (${CMAKE_VERSION} VERSION_LESS 3.14)
    include(add_FetchContent_MakeAvailable.cmake)
endif ()

set(Lib_Name lua)
set(Git_Tag v5.4.7)
#set(Git_URL https://www.lua.org/ftp/lua-5.4.7.tar.gz)
set(Git_URL https://github.com/lua/lua.git)
set(lua_Source_Dir ${ThirdParty_Path}/${Lib_Name})
set(Current_Source_Dir ${lua_Source_Dir})


# Get list of files in the directory to check if it is exists and is not empty
file(GLOB DIR_CONTENTS "${Current_Source_Dir}/*")
if (NOT DIR_CONTENTS)  # If the list is empty, the directory is empty
    message(STATUS "${Current_Source_Dir} does not exist or is empty, clone from git repository.")
    FetchContent_Declare(
            ${Lib_Name}
            GIT_REPOSITORY ${Git_URL}      # Clone from git repository
            GIT_TAG ${Git_Tag}             # you can specify a specific version here
            SOURCE_DIR ${Current_Source_Dir}   # Clone to local directory
    )
    FetchContent_MakeAvailable(${Lib_Name})  # Download and unpack at configure time
else ()
    message(STATUS "${Current_Source_Dir} exists and is not empty, skip cloning from git repository.")
endif ()


file(GLOB_RECURSE LUA_SOURCES ${Current_Source_Dir}/*.c)

file(GLOB_RECURSE UNWANTED_SOURCES ${Current_Source_Dir}/lua.c ${Current_Source_Dir}/onelua.c ${Current_Source_Dir}/testes/libs/*.c)
foreach (file ${UNWANTED_SOURCES})
    list(REMOVE_ITEM LUA_SOURCES ${file})
endforeach ()

include_directories(${Current_Source_Dir})

add_library(MakeLib_${Lib_Name} STATIC ${LUA_SOURCES})

unset(Lib_Name)
unset(Git_Tag)
unset(Git_URL)
unset(DIR_CONTENTS)
unset(Current_Source_Dir)
unset(UNWANTED_SOURCES)
#unset(LUA_SOURCES)
