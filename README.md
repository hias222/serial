# serial

serial connect test

## serial tools

com0com

https://www.com-port-monitoring.com/downloads.html

## mqtt

add
tar -xvzf ../../mqtt/mosquitto-1.6.9.tar.gz mosquitto-1.6.9/lib
mv mosquitto-1.6.9/lib mosquitto

cmake -DOPENSSL_ROOT_DIR="/usr/local/opt/openssl@1.1" -DOPENSSL_LIBRARIES="/usr/local/opt/openssl@1.1/lib" xxx

or

export OPENSSL_ROOT_DIR="/usr/local/opt/openssl@1.1"
export OPENSSL_LIBRARIES="/usr/local/opt/openssl@1.1/lib"

error include utl..

comment out:
#if (WITH_BUNDLED_DEPS)
        include_directories(${mosquitto_SOURCE_DIR} ${mosquitto_SOURCE_DIR}/src/deps)
#endif (WITH_BUNDLED_DEPS)

remove projects except lib and src