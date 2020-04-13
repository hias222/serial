
set(C_SRC
mqttUtils.cpp
)

add_library(mqtt SHARED ${C_SRC} )

target_link_libraries(mqtt PUBLIC
                            libmosquitto
                            )

target_include_directories(mqtt PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/lib"
                          "${PROJECT_SOURCE_DIR}/mosquitto-1.6.9/lib"
                          )