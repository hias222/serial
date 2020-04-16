# serial

serial connect test

## serial tools

### windows

com0com

https://www.com-port-monitoring.com/downloads.html

### Linux

Copy port 
socat -d -d pty,raw,echo=0 pty,raw,echo=0

echo -ne '\xbe\x70\x6f\x5f\x40\x30\x20\xa1\x0d\x1f\x2f\x3f' > /dev/ttys005

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
(x)7b (x)5f (x)40 (x)30 (x)20 (x)a1 (x)0d (x)1f (x)2f (x)3f (x)4d (x)5d (x)6f (x)7f (x)bc (x)0e (x)1e (x)20 (x)30 (x)4d (x)5d (x)6a (x)7e (x)35 (x)1f (x)2f (x)3f (x)4d (x)5d (x)6f (x)7f (x)ba (x)0d (x)1a (x)20 (x)30 (x)4c (x)59 (x)66 (x)77 (x)57 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)31 (x)19 (x)20 (x)30 (x)4b (x)5f (x)6e (x)79 (x)b9 (x)0d (x)1f (x)2f (x)3f (x)4d (x)5d (x)6f (x)7f (x)b6 (x)0b (x)18 (x)20 (x)30 (x)4b (x)5c (x)6d (x)77 (x)35 (x)1f (x)2f (x)3f (x)4d (x)5d (x)6f (x)7f (x)05 (x)7b (x)5f (x)40 (x)30 (x)20 (x)2b (x)41 (x)21 (x)06 (x)4a (x)5d (x)6f (x)78 (x)b5 (x)0d (x)1f (x)2f (x)3f (x)4d (x)5d (x)6f (x)7f (x)56 (x)50 (x)08 (x)06 (x)4d (x)58 (x)68 (x)7b (x)b3 (x)0d (x)1f (x)2f (x)3f (x)4d (x)5d (x)6f (x)7f (x)b0 (x)08 (x)1c (x)20 (x)30 (x)4c (x)5f (x)6c (x)79 (x)35 (x)1f (x)2f (x)3f (x)4d (x)5d (x)6f (x)7f (x)ae (x)07 (x)1b (x)20 (x)30 (x)4c (x)5c (x)6b (x)7d (x)55 (x)d8 (x)ec (x)d9 (x)4d (x)5d (x)6f (x)7f (x)05 (x)7b (x)5f (x)40 (x)30 (x)20 (x)a1 (x)0d (x)1f (x)2f (x)3f (x)4d (x)5d (x)6f

Windows
be (x)70 (x)6f (x)5f (x)40 (x)30 (x)20 (x)a1 (x)0d (x)1f (x)2f (x)3f (x)4d (x)5d (x)6f (x)7f (x)bc (x)0e (x)1e (x)20 (x)30 (x)4d (x)5d (x)6a (x)7e (x)bd (x)0d (x)1f (x)2f (x)3f (x)4d (x)5d (x)6f (x)7f (x)ba (x)0d (x)1a (x)20 (x)30 (x)4c (x)59 (x)66 (x)77 (x)bb (x)0d (x)1f (x)2f (x)3f (x)4d (x)5d (x)6f (x)7f (x)b8 (x)0c (x)19 (x)20 (x)30 (x)4b (x)5f (x)6e (x)79 (x)b9 (x)0d (x)1f (x)2f (x)3f (x)4d (x)5d (x)6f (x)7f (x)b6 (x)0b (x)18 (x)20 (x)30 (x)4b (x)5c (x)6d (x)77 (x)b7 (x)0d (x)1f (x)2f (x)3f (x)4d (x)5d (x)6f (x)7f (x)be (x)70 (x)6f (x)5f (x)40 (x)30 (x)20 (x)b4 (x)0a (x)17 (x)20 (x)30 (x)4a (x)5d (x)6f (x)78 (x)b5 (x)0d (x)1f (x)2f (x)3f (x)4d (x)5d (x)6f (x)7f (x)b2 (x)09 (x)1d (x)20 (x)30 (x)4d (x)58 (x)68 (x)7b (x)b3 (x)0d (x)1f (x)2f (x)3f (x)4d (x)5d (x)6f (x)7f (x)b0 (x)08 (x)1c (x)20 (x)30 (x)4c (x)5f (x)6c (x)79 (x)b1 (x)0d (x)1f (x)2f (x)3f (x)4d (x)5d (x)6f (x)7f (x)ae (x)07 (x)1b (x)20 (x)30 (x)4c (x)5c (x)6b (x)7d (x)af (x)0d (x)1f (x)2f (x)3f (x)4d (x)5d (x)6f (x)7f (x)7f (x)

\xbe\x70\x6f\x5f\x40\x30\x20\xa1\x0d\x1f\x2f\x3f


\xbe\x70\x6f\x5f\x40\x30\x20\xa1\x0d\x1f\x2f\x3f\x4d\x5d\x6f\x7f\xbc\x0e\x1e\x20\x30\x4d\x5d\x6a\x7e\xbd\x0d\x1f\x2f\x3f\x4d\x5d\x6f\x7f\xba\x0d\x1a\x20\x30\x4c\x59\x66\x77\xbb\x0d\x1f\x2f\x3f\x4d\x5d\x6f\x7f\xb8\x0c\x19\x20\x30\x4b\x5f\x6e\x79\xb9\x0d\x1f\x2f\x3f\x4d\x5d\x6f\x7f\xb6\x0b\x18\x20\x30\x4b\x5c\x6d\x77\xb7\x0d\x1f\x2f\x3f\x4d\x5d\x6f\x7f\xbe\x70\x6f\x5f\x40\x30\x20\xb4\x0a\x17\x20\x30\x4a\x5d\x6f\x78\xb5\x0d\x1f\x2f\x3f\x4d\x5d\x6f\x7f\xb2\x09\x1d\x20\x30\x4d\x58\x68\x7b\xb3\x0d\x1f\x2f\x3f\x4d\x5d\x6f\x7f\xb0\x08\x1c\x20\x30\x4c\x5f\x6c\x79\xb1\x0d\x1f\x2f\x3f\x4d\x5d\x6f\x7f\xae\x07\x1b\x20\x30\x4c\x5c\x6b\x7d\xaf\x0d\x1f\x2f\x3f\x4d\x5d\x6f\x7f\x7f