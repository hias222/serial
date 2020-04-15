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

or

    set(ENV{OPENSSL_ROOT_DIR} "/usr/local/opt/openssl@1.1")
    set(ENV{OPENSSL_LIBRARIES} "/usr/local/opt/openssl@1.1/lib")

error include utl..

comment out:
#if (WITH_BUNDLED_DEPS)
        include_directories(${mosquitto_SOURCE_DIR} ${mosquitto_SOURCE_DIR}/src/deps)
#endif (WITH_BUNDLED_DEPS)

remove projects except lib and src

## Windows

https://github.com/Microsoft/vcpkg

CMake projects should use: "-DCMAKE_TOOLCHAIN_FILE=C:/Users/User/git/vcpkg/scripts/buildsystems/vcpkg.cmake"

vcpkg install openssl:x86-windows
vcpkg.exe install pthreads

## RPi

sudo apt-get install libssl-dev cmake

```bash
cd build
cmake ../

```

## errors

```bash
........save 9 ctrl 09
--- update header ad
........save 9 ctrl 09
--- update header aa
........save 9 ctrl 0a
--- update header ab
........save 9 ctrl 0a
--- update header a8
........not save 9 ctrl 0b
........not save 9 ctrl 0b
........save 9 ctrl 0c
--- update header be
......save 7 ctrl 00
........save 9 ctrl 0c
--- update header a4
........not save 9 ctrl 0d
........not save 9 ctrl 0d
........save 9 ctrl 0e
--- update header a3
........save 9 ctrl 0e
--- update header a0
........save 9 ctrl 0f
--- update header a1
........save 9 ctrl 0f
--- update header bc
........save 9 ctrl 01
```