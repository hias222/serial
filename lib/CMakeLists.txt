
set(C_SRC
info.cpp 
listener.cpp
serialData.cpp)

if (WIN32)
    set (WIN_SRC readwin.cpp serialwin.cpp)
else ()
    set (LINUX_SRC read.cpp serial.cpp)
endif (WIN32)

add_library(lib ${C_SRC} ${WIN_SRC} ${LINUX_SRC})