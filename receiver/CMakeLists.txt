set(CMAKE_VERBOSE_MAKEFILE ON)

if (ISFTDI)
    message(STATUS "search lib libftd2xx.a ")
    find_library(libftd2xx
    Names libftd2xx.a
    HINTS  lib )
    message(STATUS  ${libftd2xx})

    find_library(libftd2
    Names libftd2xx.1.4.16.dylib
    HINTS  lib )
    message(STATUS  ${libftd2})

endif (ISFTDI)


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


    set (OTHER_SRC readGPIO.cpp )
endif(RASPBERRYPI)

if (WIN32)
    set (OTHER_SRC readwin.cpp )
endif (WIN32)

if (UNIX)
   if(RASPBERRYPI)
        set (OTHER_SRC readGPIO.cpp )
   else (arm)
       set (OTHER_SRC read.cpp)
   endif(RASPBERRYPI)
   if (ISFTDI)
        set (OTHER_SRC readFTDI.cpp )
   endif(ISFTDI)
endif (UNIX)

if (RASPBERRYPI)
set (OTHER_SRC readGPIO.cpp )
endif()

add_library(receiver
                ${OTHER_SRC}
            )
           
if(RASPBERRYPI)
    target_link_libraries(receiver serialconnect pigpio)
else(RASPBERRYPI)
    if (ISFTDI)
        target_link_libraries(receiver serialconnect ${libftd2})
    else(ISFTDI)
        target_link_libraries(receiver serialconnect)
    endif(ISFTDI)
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