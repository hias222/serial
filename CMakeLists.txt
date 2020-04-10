cmake_minimum_required(VERSION 3.0.0)
project(serial VERSION 0.1.0)

set(ENV{OPENSSL_ROOT_DIR} "/usr/local/opt/openssl@1.1")
set(ENV{OPENSSL_LIBRARIES} "/usr/local/opt/openssl@1.1/lib")

include(CTest)
enable_testing()

add_subdirectory(lib)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

add_executable(serial main.cpp)

target_link_libraries(serial PUBLIC serialconnect)

target_include_directories(serial PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/lib"
                          )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
