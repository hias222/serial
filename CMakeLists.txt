cmake_minimum_required(VERSION 3.0.0)
project(serial VERSION 0.1.0)

include(CTest)
enable_testing()

add_subdirectory(lib)

add_executable(serial main.cpp)

target_link_libraries(serial PUBLIC lib)

target_include_directories(serial PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/lib"
                          )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
