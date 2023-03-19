set(CMAKE_VERBOSE_MAKEFILE ON)

if (APPLE)
  if ( ${ARCHITECTURE} STREQUAL "arm64" )
    #libftd2xx.so.1.4.27
    message(STATUS "APPLE ARM search lib libftd2xx.1.4.24.dylib ")
    find_library(libftd2
    Names libftd2xx.1.4.24.dylib
    HINTS  libm )
    message(STATUS  ${libftd2})
  else()
    message(STATUS "APPLE search lib libftd2xx.1.4.16.dylib ")
    find_library(libftd2
    Names libftd2xx.1.4.16.dylib
    HINTS  lib )
    message(STATUS  ${libftd2})
  endif()
elseif ( ${ARCHITECTURE} STREQUAL "x86_64" )
    ## linux
    message(STATUS "x86_64 search lib libftd2xx.so.1.4.8 ")
    find_library(libftd2
    Names libftd2xx.so.1.4.8
    HINTS  liblinux64 )
    message(STATUS  ${libftd2})
elseif ( ${ARCHITECTURE} STREQUAL "aarch64" )
    ## linux
    message(STATUS "others search lib armv8 libftd2xx.so.1.4.8 ")
    find_library(libftd2
    Names libftd2xx.so.1.4.8
    HINTS  libarmv8 )
    message(STATUS  ${libftd2})
elseif (WIN32)
    message(STATUS "win32 search lib ftd2xx.lib")
    # set (FTDI_LIBRARIES C:\\Users\\User\\git\\serial\\receiver\\libwin32\\ftd2xx.lib)
    if (CMAKE_CL_64)
        set (FTDI_LIBRARIES ${USER_HOME_DIR}\\serial\\receiver\\libwin64\\ftd2xx.lib)
        set (LIBRARIES ${FTDI_LIBRARIES})
    else (CMAKE_CL_64)
        set (FTDI_LIBRARIES ${USER_HOME_DIR}\\serial\\receiver\\libwin32\\ftd2xx.lib)
        set (LIBRARIES ${FTDI_LIBRARIES})
    endif (CMAKE_CL_64)
else ()
    message(STATUS "others search lib libftd2xx.a ")
    find_library(libftd2
    Names libftd2xx.so.1.4.8
    HINTS  liblinux64 )
    message(STATUS  ${libftd2})
endif()


if (WIN32)
    set(RASPBERRYPI 0)
    message(STATUS "no RASPBERRY")
else (WIN32)
    execute_process(COMMAND bash ${CMAKE_HOME_DIRECTORY}/checkapi RESULT_VARIABLE RASPBERRYPI)
    if (RASPBERRYPI)
        message(STATUS "receiver RASPBERRY")
    else(RASPBERRYPI)
        message(STATUS "receiver no RASPBERRY")
    endif(RASPBERRYPI)
endif(WIN32)

if(RASPBERRYPI)
    # Find the path to the pigpio includes.
    find_path(pigpio_INCLUDE_DIR 
        NAMES pigpio.h pigpiod_if.h pigpiod_if2.h
        HINTS /usr/local/include)
        
    # Find the pigpio libraries.
    find_library(pigpio_LIBRARY 
        NAMES libpigpio.so
        HINTS /usr/local/lib)
    find_library(pigpiod_if_LIBRARY 
        NAMES libpigpiod_if.so
        HINTS /usr/local/lib)
    find_library(pigpiod_if2_LIBRARY 
        NAMES libpigpiod_if2.so
        HINTS /usr/local/lib)
        
    # Set the pigpio variables to plural form to make them accessible for 
    # the paramount cmake modules.
    set(pigpio_INCLUDE_DIRS ${pigpio_INCLUDE_DIR})
    set(pigpio_INCLUDES     ${pigpio_INCLUDE_DIR})

    # Handle REQUIRED, QUIET, and version arguments 
    # and set the <packagename>_FOUND variable.
    include(FindPackageHandleStandardArgs)

    find_package_handle_standard_args(pigpio 
        DEFAULT_MSG 
        pigpio_INCLUDE_DIR pigpio_LIBRARY pigpiod_if_LIBRARY pigpiod_if2_LIBRARY)

    set (OTHER_SRC readGPIO.cpp readFTDI.cpp)
endif(RASPBERRYPI)

if (WIN32)
    set (OTHER_SRC readwin.cpp  readFTDI.cpp)
endif (WIN32)

if (UNIX)
   if(RASPBERRYPI)
        set (OTHER_SRC readGPIO.cpp readFTDI.cpp)
   else (arm)
       set (OTHER_SRC read.cpp readFTDI.cpp)
   endif(RASPBERRYPI)
endif (UNIX)

add_library(receiver
                ${OTHER_SRC}
            )
           
if(RASPBERRYPI)
    target_link_libraries(receiver serialconnect pigpio ${libftd2})
else(RASPBERRYPI)
    if (UNIX)
        target_link_libraries(receiver serialconnect ${libftd2})
        message(STATUS "no rpi libs " ${OTHER_SRC} )
    else (UNIX)
        target_link_libraries(receiver serialconnect ${LIBRARIES})
    endif (UNIX)
endif(RASPBERRYPI)

target_include_directories(receiver PUBLIC
                            "${PROJECT_BINARY_DIR}"
                            "${PROJECT_SOURCE_DIR}/serialconnect"
                            "${PROJECT_SOURCE_DIR}/mqtt"
                            "${PROJECT_SOURCE_DIR}/sender"
                            "${PROJECT_SOURCE_DIR}/receiver"
                            "${PROJECT_SOURCE_DIR}/utils"
                            "${PROJECT_SOURCE_DIR}/receiver/lib"
                          )