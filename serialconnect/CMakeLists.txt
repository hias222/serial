
set(C_SRC
info.cpp 
listener.cpp
analyseData.cpp
serialUtils.cpp)

# not shared here - Windows

add_library(serialconnect 
            ${C_SRC}
            )


#target_link_libraries(serialconnect
#                    libmosquitto
#                    mqtt
#                    utils
#                    receiver
#                   )
        #  ${LIBRARIES}

target_include_directories(serialconnect PUBLIC
      "${PROJECT_BINARY_DIR}"
      "${PROJECT_SOURCE_DIR}/mosquitto-1.6.9/lib"
      )

                        
target_include_directories(serialconnect PUBLIC
                            "${PROJECT_BINARY_DIR}"
                            "${PROJECT_SOURCE_DIR}/serialconnect"
                            "${PROJECT_SOURCE_DIR}/mqtt"
                            "${PROJECT_SOURCE_DIR}/receiver"
                           # "${PROJECT_SOURCE_DIR}/mosquitto-1.6.9/lib"
                          )
