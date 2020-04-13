ADD_EXECUTABLE( 01-test-base 01-test-base.cpp )

target_link_libraries(01-test-base PUBLIC serialconnect)
target_include_directories(01-test-base PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/lib"
                          )

ADD_EXECUTABLE( 01-test-data 01-test-data.cpp )


target_link_libraries(01-test-data PUBLIC serialconnect)
target_include_directories(01-test-data PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/lib"
                          )

ADD_EXECUTABLE( 01-test-mqtt 01-test-mqtt.cpp )


target_link_libraries(01-test-mqtt PUBLIC 
                        serialconnect
                        mqtt
                        libmosquitto
                        mosquittopp
                        )
                        
target_include_directories(01-test-mqtt PUBLIC
                            "${PROJECT_BINARY_DIR}"
                            "${PROJECT_SOURCE_DIR}/lib"
                            "${PROJECT_SOURCE_DIR}/mqtt"
                            "${PROJECT_SOURCE_DIR}/mosquitto-1.6.9/lib"
                            "${PROJECT_SOURCE_DIR}/mosquitto-1.6.9/cpp/lib"
                          )


ADD_TEST( 01-test-data  01-test-data )
#ADD_TEST( 01-test-mqtt  01-test-mqtt )
#ADD_TEST( 01-test-base  01-test-base )