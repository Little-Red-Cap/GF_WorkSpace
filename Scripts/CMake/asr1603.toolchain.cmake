INCLUDE(CMakeForceCompiler)

# This one is important
SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_PROCESSOR arm)

set(_compiler_prefix "C:/Program Files (x86)/DS-5")
if(NOT EXISTS ${_compiler_prefix})
    if(NOT $ENV{CROSS_COMPILER_PREFIX} STREQUAL "")
        set(_compiler_prefix $ENV{CROSS_COMPILER_PREFIX})
    elseif(CROSS_COMPILER_PREFIX)
        set(_compiler_prefix ${CROSS_COMPILER_PREFIX})
    else()
        find_program(_armcc_path armcc)
        if(NOT _armcc_path)
            message(FATAL_ERROR "NOT FOUND compiler armcc in system path")
        endif()
        get_filename_component(_bin ${_armcc_path} DIRECTORY )
        get_filename_component(_compiler_prefix ${_bin} DIRECTORY )
    endif()
endif()
set(_suffix)
if(WIN32)
    set(_suffix .exe)
endif()

if(NOT $ENV{ASR1603_SDK_PREFIX} STREQUAL "")
    set(ASR1603_SDK_PREFIX $ENV{ASR1603_SDK_PREFIX})
endif()
if(NOT ASR1603_SDK_PREFIX)
    message(FATAL_ERROR "NOT DEFINE SDK PREFIX FOR CROSS COMIPILER")
endif()


# Specify the cross compiler
SET(CMAKE_C_COMPILER "${_compiler_prefix}/bin/armcc${_suffix}")
SET(CMAKE_CXX_COMPILER "${_compiler_prefix}/bin/armcc${_suffix}")
SET(CMAKE_AR "${_compiler_prefix}/bin/armar${_suffix}" CACHE FILEPATH "Archiver")


UNSET(CMAKE_C_FLAGS CACHE)
SET(CMAKE_C_FLAGS "--c99 --gnu --cpu=Cortex-R4 --thumb --no_unaligned_access --loose_implicit_cast -DENV_MSVC" CACHE STRING "" FORCE)
#UNSET(CMAKE_CXX_FLAGS CACHE)
#SET(CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS} CACHE STRING "" FORCE)
UNSET(CMAKE_EXE_LINKER_FLAGS CACHE)
SET(CMAKE_EXE_LINKER_FLAGS "" CACHE STRING "" FORCE)
UNSET(CMAKE_AR_FLAGS CACHE)
SET(CMAKE_AR_FLAGS "-p -armcc,-Ospace" CACHE STRING "" FORCE)

# set(CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> cr <TARGET> <LINK_FLAGS> <OBJECTS>")
SET(CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> ${CMAKE_AR_FLAGS} -o <TARGET> <OBJECTS>" CACHE STRING "C Archive Create")
# set(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> cr <TARGET> <LINK_FLAGS> <OBJECTS>")
SET(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> ${CMAKE_AR_FLAGS} -o <TARGET> <OBJECTS>" CACHE STRING "CXX Archive Create")

#include_directories("${_compiler_prefix}/include")

# Where is the target environment
SET(CMAKE_FIND_ROOT_PATH "${_compiler_prefix}")

include_directories(
        "${ASR1603_SDK_PREFIX}/include/asr1601/hop/telephony/yuge/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/hal/UART/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/hal/core/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/hal/PMU/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/hal/GPIO/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/diag/diag_logic/src"
        "${ASR1603_SDK_PREFIX}/include/asr1601/csw/SysCfg/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/csw/platform/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/env/win32/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/csw/BSP/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/csw/platform/dev_plat/build"
        "${ASR1603_SDK_PREFIX}/include/asr1601/os/osa/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/os/threadx/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/os/nu_xscale/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/pcac/lwipv4v6/src/include/lwip"
        "${ASR1603_SDK_PREFIX}/include/asr1601/diag/diag_logic/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/hop/timer/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/hop/intc/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/csw/PM/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/hop/pm/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/softutil/TickManager/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/hop/BSP/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/hop/telephony/atcmdsrv/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/hop/telephony/atparser/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/hop/telephony/sdk/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/pcac/httpclient/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/pcac/ci/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/pcac/lwipv4v6/src/include/arch"
        "${ASR1603_SDK_PREFIX}/include/asr1601/pcac/lwipv4v6/src/include/ipv4"
        "${ASR1603_SDK_PREFIX}/include/asr1601/pcac/lwipv4v6/src/include/ipv6"
        "${ASR1603_SDK_PREFIX}/include/asr1601/hop/mmi_mat/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/tavor/Arbel/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/tavor/env/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/hop/telephony/modem/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/pcac/duster/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/pcac/fota/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/hal/I2C/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/hal/ACIPC/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/pcac/mbedTLS/include"
        "${ASR1603_SDK_PREFIX}/include/asr1601/pcac/mbedTLS/include/mbedtls"
        "${ASR1603_SDK_PREFIX}/include/asr1601/pcac/mbedTLS/include/psa"
        "${ASR1603_SDK_PREFIX}/include/asr1601/softutil/fatsys/flash"
        "${ASR1603_SDK_PREFIX}/include/asr1601/softutil/FDI/src/INCLUDE"
        "${ASR1603_SDK_PREFIX}/include/asr1601/hal/MMU/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/softutil/FDI/src/FDI_ADD"
        "${ASR1603_SDK_PREFIX}/include/asr1601/softutil/FDI/src/FM_INC"
        "${ASR1603_SDK_PREFIX}/include/asr1601/softutil/fatsys/fs/hdr"
        "${ASR1603_SDK_PREFIX}/include/asr1601/softutil/littlefs/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/pcac/tts/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/pcac/dial/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/pcac/paho_mqtt/mqttclient"
        "${ASR1603_SDK_PREFIX}/include/asr1601/pcac/paho_mqtt/mqtt"
        "${ASR1603_SDK_PREFIX}/include/asr1601/pcac/paho_mqtt/common"
        "${ASR1603_SDK_PREFIX}/include/asr1601/pcac/paho_mqtt/platform"
        "${ASR1603_SDK_PREFIX}/include/asr1601/softutil/csw_memory/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/pcac/paho_mqtt/network"
        "${ASR1603_SDK_PREFIX}/include/asr1601/hop/utilities/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/hop/commpm/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/softutil/nvm/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/softutil/EEhandler/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/softutil/EEhandler/src"
        "${ASR1603_SDK_PREFIX}/include/asr1601/hop/RTC/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/hop/telephony/ci_client/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/hal/BT_device/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/hal/UART/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/hop/mrd/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/softutil/softutil/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/pcac/mbedTLS/asros"
        "${ASR1603_SDK_PREFIX}/include/asr1601/hal/SPI/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/pcac/websocket/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/pcac/atnet_srv/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/softutil/fotacomm/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/aud_sw/Audio/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/aud_sw/ACM_COMM/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/aud_sw/audio_stub/src"
        "${ASR1603_SDK_PREFIX}/include/asr1601/hop/aam/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/aud_sw/AudioHAL/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/hal/dbgshell/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/pcac/amr/include"
        "${ASR1603_SDK_PREFIX}/include/asr1601/aud_sw/AudioService/inc"
        "${ASR1603_SDK_PREFIX}/include/asr1601/pcac/tuya/interface"
        "${ASR1603_SDK_PREFIX}/include/asr1601/pcac/tuya/libraries/coreHTTP/source/include"
        "${ASR1603_SDK_PREFIX}/include/asr1601/pcac/tuya/libraries/coreHTTP/source/dependency/3rdparty/http_parser"
        "${ASR1603_SDK_PREFIX}/include/asr1601/pcac/tuya/libraries/coreJSON/source/include"
        "${ASR1603_SDK_PREFIX}/include/asr1601/pcac/tuya/libraries/coreMQTT/source/include"
        "${ASR1603_SDK_PREFIX}/include/asr1601/pcac/tuya/middleware"
        "${ASR1603_SDK_PREFIX}/include/asr1601/pcac/tuya/utils"
        "${ASR1603_SDK_PREFIX}/include/asr1601/pcac/mp3/include"
        "${ASR1603_SDK_PREFIX}/include/asr1601/pcac/opencpu/inc"

)

# static library required
SET(BUILD_SHARED_LIBS FALSE)
# Search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# For libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

unset(_compiler_prefix)

