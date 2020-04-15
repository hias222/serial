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

08 (1).1c (1).20 (1).30 (1).4c (1).5f (1).6c (1).79 (1).35 (1).uups 35 in_count too large 
1f (1)2f (1)3f (1)4d (1)5d (1)6f (1)7f (1)ae (1)start 
 colorado_control_bit: 0xae 00  
colorado_control_channel: 0xae 0x8  
we need 9 bytes
07 (1).1b (1).20 (1).30 (1).4c (1).5c (1).6b (1).7d (1).55 (1).uups 55 in_count too large 
d8 (1)start 
 colorado_control_bit: 0xd8 00  
colorado_control_channel: 0xd8 0x13  
we need 0 bytes
ec (1)start 
 not save 1 ctrl 13 
colorado_control_bit: 0xec 00  
colorado_control_channel: 0xec 0x9  
we need 9 bytes
d9 (1)start 
 not save 1 ctrl 09 
colorado_control_bit: 0xd9 0x1  
colorado_control_channel: 0xd9 0x13  
we need 0 bytes
4d (1).5d (1).6f (1).7f (1).7f (0).


0d (x).1f (x).2f (x).3f (x).4d (x).5d (x).6f (x).7f (x).ae (x)start
 save 9 ctrl 07
1: [7][1] 2 2: [7][3] 0 3: [7][5] 0 4: [7][7] 0 5: [7][9] 2 6: [7][11] 2 7: [7][13] 0 8: [7][15] 0
 --- lane ae
7: 3 00 30 36
gettime: lane 7 00:30,36 3
 colorado_control_bit: 0xae 00
colorado_control_channel: 0xae 0x8
we need 9 bytes
07 (x).1b (x).20 (x).30 (x).4c (x).5c (x).6b (x).7d (x).af (x)start
 save 9 ctrl 08
1: [8][0] 8 2: [8][2] 4 3: [8][4] 15 4: [8][6] 15 5: [8][8] 3 6: [8][10] 3 7: [8][12] 4 8: [8][14] 2
 --- lane af
8: 4 00 33 42
gettime: lane 8 00:33,42 4
 colorado_control_bit: 0xaf 0x1
colorado_control_channel: 0xaf 0x8
we need 9 bytes
0d (x).1f (x).2f (x).3f (x).4d (x).5d (x).6f (x).7f (x).7f (x).uups 7f in_count too large