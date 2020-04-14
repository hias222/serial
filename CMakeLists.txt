cmake_minimum_required(VERSION 3.0.0)
project(serial VERSION 0.1.0)

if(APPLE)
    set(ENV{OPENSSL_ROOT_DIR} "/usr/local/opt/openssl@1.1")
    set(ENV{OPENSSL_LIBRARIES} "/usr/local/opt/openssl@1.1/lib")
endif()

if(WIN32)
  #  set(ENV{OPENSSL_ROOT_DIR} "C:\Users\User\git\vcpkg\installed\x86-windows\lib")
    set(ENV{OPENSSL_ROOT_DIR} "C:/Users/User/git/vcpkg/installed/x86-windows")
endif()


#[cmake]   system variable OPENSSL_ROOT_DIR (missing: OPENSSL_CRYPTO_LIBRARY
#[cmake]   OPENSSL_INCLUDE_DIR)

include(CTest)
enable_testing()

add_subdirectory(mqtt)
add_subdirectory(serialconnect)
add_subdirectory(mosquitto-1.6.9)


if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

add_executable(serial main.cpp)

target_link_libraries(serial PUBLIC 
                            serialconnect
                            mqtt
                            )

target_include_directories(serial PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/serialconnect"
                          "${PROJECT_SOURCE_DIR}/mqtt"
                          "${PROJECT_SOURCE_DIR}/mosquitto-1.6.9/lib"
                          )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
