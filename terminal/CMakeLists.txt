set(CMAKE_VERBOSE_MAKEFILE ON)

if (WIN32)
    set (OTHER_SRC readwin.cpp )
endif (WIN32)

if (UNIX)
    set (OTHER_SRC read.cpp)
endif (UNIX)

add_library(terminal
                ${OTHER_SRC}
            )
           
target_link_libraries(terminal mqtt utils )

target_include_directories(terminal PUBLIC
                            "${PROJECT_BINARY_DIR}"
                            "${PROJECT_SOURCE_DIR}/mqtt"
                            "${PROJECT_SOURCE_DIR}/utils"
                          )