
set(C_SRC
info.cpp 
listener.cpp
analyseData.cpp
serialUtils.cpp)

if (WIN32)
    set (WIN_SRC readwin.cpp serialwin.cpp)
else ()
    set (LINUX_SRC read.cpp serial.cpp)
endif (WIN32)

add_library(serialconnect SHARED ${C_SRC} ${WIN_SRC} ${LINUX_SRC})

target_link_libraries(serialconnect 
                    mqtt
                    libmosquitto
                    ${LIBRARIES})

                        
target_include_directories(serialconnect PUBLIC
                            "${PROJECT_BINARY_DIR}"
                            "${PROJECT_SOURCE_DIR}/lib"
                            "${PROJECT_SOURCE_DIR}/mqtt"
                            "${PROJECT_SOURCE_DIR}/mosquitto-1.6.9/lib"
                            "${PROJECT_SOURCE_DIR}/mosquitto-1.6.9/cpp/lib"
                          )
