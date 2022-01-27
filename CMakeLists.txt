cmake_minimum_required(VERSION 3.0.0)
string(TIMESTAMP TODAY "%Y%m%d")
project(serial VERSION 0.1.${TODAY})

EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )
if (NOT WIN32)
    EXECUTE_PROCESS(COMMAND bash ${CMAKE_HOME_DIRECTORY}/checkapi RESULT_VARIABLE RASPBERRYPI)
    EXECUTE_PROCESS(COMMAND bash ${CMAKE_HOME_DIRECTORY}/checkftdi RESULT_VARIABLE ISFTDI)
endif()

message(STATUS "ISFTDI ${ISFTDI} RASPBERRYPI ${RASPBERRYPI} ARCHITECTURE ${ARCHITECTURE}")

set(SENDMODE false)

if(WIN32)
    set(ARCHITECTURE WINDOWS)
endif(WIN32)

message( STATUS "Architecture: ${ARCHITECTURE}" )

if( ${ARCHITECTURE} STREQUAL "x86_64" )
    message( STATUS "x86_64: ${ARCHITECTURE}" )
else()
    message( STATUS "nothing: ${ARCHITECTURE}" )
endif()

if (ISFTDI)
    message(STATUS "ISFTDI")
endif (ISFTDI)

if (RASPBERRYPI)
    message(STATUS "RASPBERRY")
    set(PORTNAME "/dev/ttySO")
    set(DESTPORTNAME "/dev/ttyUSB0")
    set(SENDMODE true)
else()
    message(STATUS "no RASPBERRY")
    set(PORTNAME "/dev/ttyUSB0")
    set(DESTPORTNAME "/dev/ttyUSB0")
endif()

if(APPLE)
    set(ENV{OPENSSL_ROOT_DIR} "/usr/local/opt/openssl@1.1")
    set(ENV{OPENSSL_LIBRARIES} "/usr/local/opt/openssl@1.1/lib")
    set(PORTNAME "/dev/ttys002")
    set(DESTPORTNAME "/dev/ttys010")
endif()

if(WIN32)
    set(RASPBERRYPI "0")
    # set(USER_HOME_DIR "C:/Users/Matthias/git")
    set(USER_HOME_DIR "C:/Users/Max/git")
    # set(THREAD_HOME_DIR "C:\\Users\\User\\git\\vcpkg\\installed\\x86-windows")
    # set(ENV{OPENSSL_ROOT_DIR} "C:/Users/User/git/vcpkg/installed/x86-windows")
    if (CMAKE_CL_64)
        set(OPENSSL_ROOT_DIR "${USER_HOME_DIR}/vcpkg/installed/x64-windows")
        set(ENV{OPENSSL_ROOT_DIR} "${USER_HOME_DIR}/vcpkg/installed/x64-windows")
    else (CMAKE_CL_64)
        set(OPENSSL_ROOT_DIR "${USER_HOME_DIR}/vcpkg/installed/x86-windows")
        set(ENV{OPENSSL_ROOT_DIR} "${USER_HOME_DIR}/vcpkg/installed/x86-windows")
    endif (CMAKE_CL_64)

    set(PORTNAME "com1")
    set(DESTPORTNAME "com2")
    set(SENDMODE false)
    message( STATUS "SSL: ${OPENSSL_ROOT_DIR}")
endif()

configure_file(SerialConfig.h.in SerialConfig.h)

#[cmake]   system variable OPENSSL_ROOT_DIR (missing: OPENSSL_CRYPTO_LIBRARY
#[cmake]   OPENSSL_INCLUDE_DIR)

include(CTest)
enable_testing()

add_subdirectory(mqtt)
add_subdirectory(serialconnect)
add_subdirectory(mosquitto-1.6.9)
add_subdirectory(utils)
add_subdirectory(receiver)
add_subdirectory(sender)

if(BUILD_TESTING)
   add_subdirectory(tests)
endif()

add_executable(serial main.cpp)

target_link_libraries(serial PUBLIC 
                            serialconnect
                            mqtt
                            libmosquitto
                            utils
                            receiver
                            sender
                            )

target_include_directories(serial PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/serialconnect"
                          "${PROJECT_SOURCE_DIR}/mqtt"
                          "${PROJECT_SOURCE_DIR}/utils"
                          "${PROJECT_SOURCE_DIR}/receiver"
                          "${PROJECT_SOURCE_DIR}/sender"
                          "${PROJECT_SOURCE_DIR}/mosquitto-1.6.9/lib"
                          )

### copy for release
# apple und Linux
if(APPLE)
    message(STATUS "files for APPLE to " ${PROJECT_BINARY_DIR})
    set(RELEASE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/binaries/mac")

    #file(COPY ${PROJECT_BINARY_DIR}/serial
    #DESTINATION ${RELEASE_OUTPUT_PATH})

    file(COPY ${PROJECT_SOURCE_DIR}/receiver/lib/libftd2xx.1.4.16.dylib
    DESTINATION ${RELEASE_OUTPUT_PATH})

    file(COPY ${PROJECT_SOURCE_DIR}/receiver/lib/Readme.md
        DESTINATION ${RELEASE_OUTPUT_PATH})

    message(STATUS "files in" ${EXECUTABLE_OUTPUT_PATH})

    set(EXECUTABLE_OUTPUT_PATH "${RELEASE_OUTPUT_PATH}")

elseif(UNIX)
    if ( ${ARCHITECTURE} STREQUAL "x86_64" )
        message(STATUS "files for Linux to " ${PROJECT_BINARY_DIR})
        set(RELEASE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/binaries/linux64")

        #file(COPY ${PROJECT_BINARY_DIR}/serial
        #DESTINATION ${RELEASE_OUTPUT_PATH})

        file(COPY ${PROJECT_SOURCE_DIR}/receiver/liblinux64/libftd2xx.so.1.4.8
        DESTINATION ${RELEASE_OUTPUT_PATH})

        file(COPY ${PROJECT_SOURCE_DIR}/receiver/liblinux64/Readme.md
        DESTINATION ${RELEASE_OUTPUT_PATH})

        message(STATUS "files in" ${EXECUTABLE_OUTPUT_PATH})

        set(EXECUTABLE_OUTPUT_PATH "${RELEASE_OUTPUT_PATH}")

    endif()
endif()

# aarch8 arm 8
if(${ARCHITECTURE} STREQUAL "aarch64")
    message(STATUS "files for aarch64 to " ${PROJECT_BINARY_DIR})
    set(RELEASE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/binaries/aarch64")

    #file(COPY ${PROJECT_BINARY_DIR}/serial
    #DESTINATION ${RELEASE_OUTPUT_PATH})

    file(COPY ${PROJECT_SOURCE_DIR}/receiver/libarmv8/libftd2xx.so.1.4.8
    DESTINATION ${RELEASE_OUTPUT_PATH})

    #file(COPY ${PROJECT_SOURCE_DIR}/receiver/libarmv8/libftd2xx.a
    #DESTINATION ${RELEASE_OUTPUT_PATH})

    file(COPY ${PROJECT_SOURCE_DIR}/receiver/libarmv8/Readme.md
        DESTINATION ${RELEASE_OUTPUT_PATH})

    message(STATUS "files in" ${EXECUTABLE_OUTPUT_PATH})

    set(EXECUTABLE_OUTPUT_PATH "${RELEASE_OUTPUT_PATH}")

endif()

if(WIN32)
    #set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}")
    #set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}")

    if (CMAKE_CL_64)
        message(STATUS "files for aarch64 to " ${PROJECT_BINARY_DIR})
        set(RELEASE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/binaries/win64")
    else (CMAKE_CL_64)
        message(STATUS "files for aarch64 to " ${PROJECT_BINARY_DIR})
        set(RELEASE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/binaries/win")
    endif (CMAKE_CL_64)

   #file(COPY ${PROJECT_BINARY_DIR}/mosquitto-1.6.9/lib/Release/mosquitto.dll
    #                       DESTINATION ${RELEASE_OUTPUT_PATH}/Release)

    if (CMAKE_CL_64)

        file(COPY ${PROJECT_SOURCE_DIR}/receiver/libwin32/Readme.md
        DESTINATION ${RELEASE_OUTPUT_PATH}/Release)

        file(COPY ${PROJECT_SOURCE_DIR}/receiver/libwin64/ftd2xx64.dll
        DESTINATION ${RELEASE_OUTPUT_PATH}/Release)
        
        # change on update
        file(COPY ${PROJECT_SOURCE_DIR}/receiver/libwin64/mosquitto.dll
        DESTINATION ${RELEASE_OUTPUT_PATH}/Release)

        file(COPY ${USER_HOME_DIR}/vcpkg/installed/x64-windows/bin/libcrypto-1_1-x64.dll
        DESTINATION ${RELEASE_OUTPUT_PATH}/Release)

        #libssl-1_1-x64.dll
        file(COPY ${USER_HOME_DIR}/vcpkg/installed/x64-windows/bin/libssl-1_1-x64.dll
        DESTINATION ${RELEASE_OUTPUT_PATH}/Release)

        file(COPY ${USER_HOME_DIR}/vcpkg/installed/x64-windows/bin/pthreadVC3.dll
        DESTINATION ${RELEASE_OUTPUT_PATH}/Release)

    #file(COPY ${PROJECT_BINARY_DIR}/serial.exe
    #DESTINATION ${RELEASE_OUTPUT_PATH})

    else (CMAKE_CL_64)
        file(COPY ${USER_HOME_DIR}/vcpkg/installed/x86-windows/bin/libssl-1_1.dll
        DESTINATION ${RELEASE_OUTPUT_PATH})

        file(COPY ${PROJECT_SOURCE_DIR}/receiver/libwin32/Readme.md
        DESTINATION ${RELEASE_OUTPUT_PATH}/Release)

        file(COPY ${PROJECT_SOURCE_DIR}/receiver/libwin32/ftd2xx.dll
        DESTINATION ${RELEASE_OUTPUT_PATH}/Release)

        file(COPY ${USER_HOME_DIR}/vcpkg/installed/x86-windows/bin/libcrypto-1_1.dll
        DESTINATION ${RELEASE_OUTPUT_PATH}/Release)

        file(COPY ${USER_HOME_DIR}/vcpkg/installed/x86-windows/bin/pthreadVC3.dll
        DESTINATION ${RELEASE_OUTPUT_PATH}/Release)

    endif (CMAKE_CL_64)

    set(EXECUTABLE_OUTPUT_PATH "${RELEASE_OUTPUT_PATH}")
    set(LIBRARY_OUTPUT_PATH "${RELEASE_OUTPUT_PATH}")

    message( STATUS "binaries in: ${RELEASE_OUTPUT_PATH}")
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
