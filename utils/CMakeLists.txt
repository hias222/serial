
set(C_SRC
data.cpp
)

add_library(utils  ${C_SRC} )

target_link_libraries(utils 
                            serialconnect
                            mqtt
                            )

target_include_directories(mqtt PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/utils"
                          "${PROJECT_SOURCE_DIR}/serialconnect"
                          "${PROJECT_SOURCE_DIR}/mosquitto-1.6.9/lib"
                          )