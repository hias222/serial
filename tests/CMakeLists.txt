ADD_EXECUTABLE( 01-test-serial 01-test-serial.cpp )

target_link_libraries(01-test-serial PUBLIC serialconnect
mqtt
libmosquitto
utils
sender
libmosquitto
terminal
receiver)


target_include_directories(01-test-serial PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_BINARY_DIR}/utils"
                          "${PROJECT_SOURCE_DIR}/serialconnect"
                          "${PROJECT_SOURCE_DIR}/lib"
                          "${PROJECT_SOURCE_DIR}/sender"
                          "${PROJECT_SOURCE_DIR}/receiver"
                          "${PROJECT_SOURCE_DIR}/terminal"
                          "${PROJECT_SOURCE_DIR}/mosquitto-1.6.9/lib"
                          )

ADD_EXECUTABLE( 01-test-data 01-test-data.cpp )

target_link_libraries(01-test-data PUBLIC serialconnect
mqtt
libmosquitto
utils
sender
libmosquitto
terminal
receiver)

target_include_directories(01-test-data PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_BINARY_DIR}/utils"
                          "${PROJECT_SOURCE_DIR}/serialconnect"
                          "${PROJECT_SOURCE_DIR}/mqtt"
                          "${PROJECT_SOURCE_DIR}/sender"
                          "${PROJECT_SOURCE_DIR}/terminal"
                          "${PROJECT_SOURCE_DIR}/mosquitto-1.6.9/lib"
                          "${PROJECT_SOURCE_DIR}/receiver"
                          )

ADD_TEST( 01-test-data  01-test-data )
# ADD_TEST( 01-test-serial  01-test-serial )