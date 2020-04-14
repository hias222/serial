
set(C_SRC
info.cpp 
listener.cpp
analyseData.cpp
serialUtils.cpp)

if (WIN32)
    set (OTHER_SRC readwin.cpp serialwin.cpp)
else ()
    set (OTHER_SRC read.cpp serial.cpp)
endif (WIN32)

# not shared here - Windows

add_library(serialconnect 
            ${C_SRC}
            ${OTHER_SRC}
            )


target_link_libraries(serialconnect
                    libmosquitto
                    mqtt
                   )
        #  ${LIBRARIES}

target_include_directories(serialconnect PUBLIC
      "${PROJECT_BINARY_DIR}"
      "${PROJECT_SOURCE_DIR}/serialconnect"
      "${PROJECT_SOURCE_DIR}/mqtt"
      "${PROJECT_SOURCE_DIR}/mosquitto-1.6.9/lib"
      )

                        
target_include_directories(serialconnect PUBLIC
                            "${PROJECT_BINARY_DIR}"
                            "${PROJECT_SOURCE_DIR}/serialconnect"
                            "${PROJECT_SOURCE_DIR}/mqtt"
                           # "${PROJECT_SOURCE_DIR}/mosquitto-1.6.9/lib"
                          )
