# serial

serial connect test

## serial tools

### windows

com0com

https://www.com-port-monitoring.com/downloads.html

### Linux

Copy port 
socat -d -d pty,raw,echo=0 pty,raw,echo=0

Receive data 
sudo minicom -D /dev/ttyS0

protocol

https://www.cmrr.umn.edu/~strupp/serial.html

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

Linux
9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d

Windows
be (x)70 (x)6f (x)5f (x)40 (x)30 (x)20 (x)a1 (x)0d (x)1f (x)2f (x)3f (x)4d (x)5d (x)6f (x)7f (x)bc (x)0e (x)1e (x)20 (x)30 (x)4d (x)5d (x)6a (x)7e (x)bd (x)0d (x)1f (x)2f (x)3f (x)4d (x)5d (x)6f (x)7f (x)ba (x)0d (x)1a (x)20 (x)30 (x)4c (x)59 (x)66 (x)77 (x)bb (x)0d (x)1f (x)2f (x)3f (x)4d (x)5d (x)6f (x)7f (x)b8 (x)0c (x)19 (x)20 (x)30 (x)4b (x)5f (x)6e (x)79 (x)b9 (x)0d (x)1f (x)2f (x)3f (x)4d (x)5d (x)6f (x)7f (x)b6 (x)0b (x)18 (x)20 (x)30 (x)4b (x)5c (x)6d (x)77 (x)b7 (x)0d (x)1f (x)2f (x)3f (x)4d (x)5d (x)6f (x)7f (x)be (x)70 (x)6f (x)5f (x)40 (x)30 (x)20 (x)b4 (x)0a (x)17 (x)20 (x)30 (x)4a (x)5d (x)6f (x)78 (x)b5 (x)0d (x)1f (x)2f (x)3f (x)4d (x)5d (x)6f (x)7f (x)b2 (x)09 (x)1d (x)20 (x)30 (x)4d (x)58 (x)68 (x)7b (x)b3 (x)0d (x)1f (x)2f (x)3f (x)4d (x)5d (x)6f (x)7f (x)b0 (x)08 (x)1c (x)20 (x)30 (x)4c (x)5f (x)6c (x)79 (x)b1 (x)0d (x)1f (x)2f (x)3f (x)4d (x)5d (x)6f (x)7f (x)ae (x)07 (x)1b (x)20 (x)30 (x)4c (x)5c (x)6b (x)7d (x)af (x)0d (x)1f (x)2f (x)3f (x)4d (x)5d (x)6f (x)7f (x)7f (x)