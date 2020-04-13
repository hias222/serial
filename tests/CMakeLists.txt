ADD_EXECUTABLE( 01-test-serial 01-test-serial.cpp )

target_link_libraries(01-test-serial PUBLIC serialconnect)
target_include_directories(01-test-serial PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/lib"
                          )

ADD_EXECUTABLE( 01-test-data 01-test-data.cpp )


target_link_libraries(01-test-data PUBLIC serialconnect)
target_include_directories(01-test-data PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/lib"
                          )

ADD_TEST( 01-test-data  01-test-data )
# ADD_TEST( 01-test-serial  01-test-serial )