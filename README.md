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


### Rspberry

sudo apt-get install libssl-dev cmake

Repair the serail port with - this helps!
jpnevulator --ascii --timing-print --tty /dev/ttyUSB1:SB9600d --read

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

```bash
git clone https://github.com/Microsoft/vcpkg.git
PS> .\bootstrap-vcpkg.bat

```

CMake projects should use: "-DCMAKE_TOOLCHAIN_FILE=C:/Users/User/git/vcpkg/scripts/buildsystems/vcpkg.cmake"

vcpkg install openssl:x86-windows
vcpkg.exe install pthreads

c++ compiler
https://docs.microsoft.com/de-de/cpp/build/building-on-the-command-line?view=vs-2019
--> Build Tools


## RPi

sudo apt-get install libssl-dev cmake

### serial

sudo raspi-config
Select option 5, Interfacing options, then option P6, Serial, and select No. Exit raspi-config.


### install

```
wget https://github.com/joan2937/pigpio/archive/master.zip
unzip master.zip
cd pigpio-master
make
sudo make install
```

```bash
cd build
cmake ../

```

## output of test data 

### jpnevulator

```
jpnevulator --timing-print --tty /dev/ttyUSB0:SB9600d --read
2020-04-19 12:51:34.260967:
80 BE 70 6F 5F 40 30 20 A1 0D 1F 2F 3F 4D 5D 6F
7F BC 0E 1E 20 30 4D 5D 6A 7E BD 0D 1F 2F 3F 4D
5D 6F 7F BA 0D 1A 20 30 4C 59 66 77 BB 0D 1F 2F
3F 4D 5D 6F 7F B8 0C 19 20 30 4B 5F 6E 79 B9 0D
1F 2F 3F 4D 5D 6F 7F B6 0B 18 20 30 4B 5C 6D 77
B7 0D 1F 2F 3F 4D 5D 6F 7F BE 70 6F 5F 40 30 20
B4 0A 17 20 30 4A 5D 6F 78 B5 0D 1F 2F 3F 4D 5D
6F 7F B2 09 1D 20 30 4D 58 68 7B B3 0D 1F 2F 3F
4D 5D 6F 7F B0 08 1C 20 30 4C 5F 6C 79 B1 0D 1F
2F 3F 4D 5D 6F 7F AE 07 1B 20 30 4C 5C 6B 7D AF
0D 1F 2F 3F 4D 5D 6F 7F 7F 80
```

### serial-receiver

```
./serial-receiver 
start....

  BaudRate = 9600 
  StopBits = 1 
  Parity   = none 
ready with /dev/ttyUSB0

80 
be 70 6f 5f 40 30 20 
a1 0d 1f 2f 3f 4d 5d 6f 7f 
bc 0e 1e 20 30 4d 5d 6a 7e 
bd 0d 1f 2f 3f 4d 5d 6f 7f 
ba 0d 1a 20 30 4c 59 66 77 
bb 0d 1f 2f 3f 4d 5d 6f 7f 
b8 0c 19 20 30 4b 5f 6e 79 
b9 0d 1f 2f 3f 4d 5d 6f 7f 
b6 0b 18 20 30 4b 5c 6d 77 
b7 0d 1f 2f 3f 4d 5d 6f 7f 
be 70 6f 5f 40 30 20 
b4 0a 17 20 30 4a 5d 6f 78 
b5 0d 1f 2f 3f 4d 5d 6f 7f 
b2 09 1d 20 30 4d 58 68 7b 
b3 0d 1f 2f 3f 4d 5d 6f 7f 
b0 08 1c 20 30 4c 5f 6c 79 
b1 0d 1f 2f 3f 4d 5d 6f 7f 
ae 07 1b 20 30 4c 5c 6b 7d 
af 0d 1f 2f 3f 4d 5d 6f 7f 7f 
80
```

### Windows

be 70 6f 5f 40 30 20
a1 0d 1f 2f 3f 4d 5d 6f 7f
bc 0e 1e 20 30 4d 5d 6a 7e
bd 0d 1f 2f 3f 4d 5d 6f 7f
ba 0d 1a 20 30 4c 59 66 77
bb 0d 1f 2f 3f 4d 5d 6f 7f
b8 0c 19 20 30 4b 5f 6e 79
b9 0d 1f 2f 3f 4d 5d 6f 7f
b6 0b 18 20 30 4b 5c 6d 77
b7 0d 1f 2f 3f 4d 5d 6f 7f
be 70 6f 5f 40 30 20
b4 0a 17 20 30 4a 5d 6f 78
b5 0d 1f 2f 3f 4d 5d 6f 7f
b2 09 1d 20 30 4d 58 68 7b
b3 0d 1f 2f 3f 4d 5d 6f 7f
b0 08 1c 20 30 4c 5f 6c 79
b1 0d 1f 2f 3f 4d 5d 6f 7f
ae 07 1b 20 30 4c 5c 6b 7d
af 0d 1f 2f 3f 4d 5d 6f 7f 7f

#### Arduino

80  ---- start detected
BE  ---- start detected
70 6F 5F 40 30 20 A1  ---- start detected
D 1F 2F 3F 4D 5D 6F 7F BC  ---- start detected
E 1E 20 30 4D 5D 6A 7E BD  ---- start detected
D 1F 2F 3F 4D 5D 6F 7F BA  ---- start detected
D 1A 20 30 4C 59 66 77 BB  ---- start detected
D 1F 2F 3F 4D 5D 6F 7F B8  ---- start detected
C 19 20 30 4B 5F 6E 79 B9  ---- start detected
D 1F 2F 3F 4D 5D 6F 7F B6  ---- start detected
B 18 20 30 4B 5C 6D 77 B7  ---- start detected

### HexValues

```
\xbe\x70\x6f\x5f\x40\x30\x20\xa1\x0d\x1f\x2f\x3f\x4d\x5d\x6f\x7f\xbc\x0e\x1e\x20\x30\x4d\x5d\x6a\x7e\xbd\x0d\x1f\x2f\x3f\x4d\x5d\x6f\x7f\xba\x0d\x1a\x20\x30\x4c\x59\x66\x77\xbb\x0d\x1f\x2f\x3f\x4d\x5d\x6f\x7f\xb8\x0c\x19\x20\x30\x4b\x5f\x6e\x79\xb9\x0d\x1f\x2f\x3f\x4d\x5d\x6f\x7f\xb6\x0b\x18\x20\x30\x4b\x5c\x6d\x77\xb7\x0d\x1f\x2f\x3f\x4d\x5d\x6f\x7f\xbe\x70\x6f\x5f\x40\x30\x20\xb4\x0a\x17\x20\x30\x4a\x5d\x6f\x78\xb5\x0d\x1f\x2f\x3f\x4d\x5d\x6f\x7f\xb2\x09\x1d\x20\x30\x4d\x58\x68\x7b\xb3\x0d\x1f\x2f\x3f\x4d\x5d\x6f\x7f\xb0\x08\x1c\x20\x30\x4c\x5f\x6c\x79\xb1\x0d\x1f\x2f\x3f\x4d\x5d\x6f\x7f\xae\x07\x1b\x20\x30\x4c\x5c\x6b\x7d\xaf\x0d\x1f\x2f\x3f\x4d\x5d\x6f\x7f\x7f
```
