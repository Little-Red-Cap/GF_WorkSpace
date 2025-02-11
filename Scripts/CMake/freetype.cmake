include(FetchContent)   # CMake 3.11 or later required
# FetchContent_MakeAvailable was not added until CMake 3.14
if (${CMAKE_VERSION} VERSION_LESS 3.14)
    include(add_FetchContent_MakeAvailable.cmake)
endif ()

set(Lib_Name freetype)
#set(Git_Tag 9.2.2)
set(Git_URL https://github.com/freetype/freetype.git)
set(freetype_Source_Dir ${ThirdParty_Path}/${Lib_Name})
set(Current_Source_Dir ${freetype_Source_Dir})


# Get list of files in the directory to check if it is exists and is not empty
file(GLOB DIR_CONTENTS "${Current_Source_Dir}/*")
if (NOT DIR_CONTENTS)  # If the list is empty, the directory is empty
    message(STATUS "${Current_Source_Dir} does not exist or is empty, clone from git repository.")
    FetchContent_Declare(
            ${Lib_Name}
            GIT_REPOSITORY ${Git_URL}      # Clone from git repository
#            GIT_TAG ${Git_Tag}             # you can specify a specific version here
            SOURCE_DIR ${Current_Source_Dir}   # Clone to local directory
    )
    FetchContent_MakeAvailable(${Lib_Name})  # Download and unpack at configure time
else ()
    message(STATUS "${Current_Source_Dir} exists and is not empty, skip cloning from git repository.")
endif ()


include_directories(
        ${Current_Source_Dir}/include
        ${Current_Source_Dir}/include/freetype
        ${Current_Source_Dir}/include/freetype/config
        ${Current_Source_Dir}/include/freetype/internal
        ${Current_Source_Dir}/include/freetype/internal/services
)

file(GLOB_RECURSE freetype_SOURCES ${Current_Source_Dir}/src/*.c)

add_library(MakeLib_${Lib_Name} STATIC ${freetype_SOURCES})

unset(Lib_Name)
unset(Git_Tag)
unset(Git_URL)
unset(DIR_CONTENTS)
unset(Current_Source_Dir)
