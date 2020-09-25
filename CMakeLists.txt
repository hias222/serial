cmake_minimum_required(VERSION 3.0.0)
string(TIMESTAMP TODAY "%Y%m%d")
project(serial VERSION 0.1.${TODAY})

execute_process(COMMAND bash ${CMAKE_HOME_DIRECTORY}/checkapi RESULT_VARIABLE RASPBERRYPI)

set(SENDMODE false)

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
    set(USER_HOME_DIR "C:/Users/Matthias/git")
    # set(USER_HOME_DIR "C:/Users/User/git")
    # set(THREAD_HOME_DIR "C:\\Users\\User\\git\\vcpkg\\installed\\x86-windows")
    set(ENV{OPENSSL_ROOT_DIR} "C:/Users/User/git/vcpkg/installed/x86-windows")
    set(ENV{OPENSSL_ROOT_DIR} "${USER_HOME_DIR}/vcpkg/installed/x86-windows")
    set(PORTNAME "com1")
    set(DESTPORTNAME "com2")
    set(SENDMODE false)
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
add_subdirectory(terminal)

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
                          "${PROJECT_SOURCE_DIR}/terminal"
                          "${PROJECT_SOURCE_DIR}/mosquitto-1.6.9/lib"
                          )

if(WIN32)
    set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}")
    set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}")

 #   file(COPY ${PROJECT_BINARY_DIR}/mosquitto-1.6.9/lib/mosquitto.dll
  #                          DESTINATION ${PROJECT_BINARY_DIR})

    file(COPY ${USER_HOME_DIR}/vcpkg/installed/x86-windows/bin/libssl-1_1.dll
                            DESTINATION ${PROJECT_BINARY_DIR})

    file(COPY ${USER_HOME_DIR}/vcpkg/installed/x86-windows/bin/libcrypto-1_1.dll
    DESTINATION ${PROJECT_BINARY_DIR})

    file(COPY ${USER_HOME_DIR}/vcpkg/installed/x86-windows/bin/pthreadVC3.dll
    DESTINATION ${PROJECT_BINARY_DIR})
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
