set(CMAKE_VERBOSE_MAKEFILE ON)

if (RASPBERRYPI)
    message(STATUS "receiver RASPBERRY")
else()
    message(STATUS "receiver no RASPBERRY")
endif()

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
endif (UNIX)

if (RASPBERRYPI)
set (OTHER_SRC readGPIO.cpp )
endif()

add_library(receiver
                analyseData.cpp
                ${OTHER_SRC}
            )
if(RASPBERRYPI)
    target_link_libraries(receiver PUBLIC pigpio)
endif ()

