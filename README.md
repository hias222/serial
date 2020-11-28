# live timing on colorado serial scoreboard port

## Description

This tool receives data of colorado System 6 over a serial/usb on linux/mac/raspberry/windows to local MQTT topic. After this, it can be forwarded to a website for live timing.  See [ansibleswim](https://github.com/hias222/ansibleswim) repo for details about using all softwareparts on a raspberry.

## Install

To run the programm successful a mqtt server on port 1883 without security needs to be run.

To connect the USB pc port to system6 scorboard rs232 jack 6.3 (not usb) you need something like this.

* converter cable from jack to usb build your own (it is very easy, also possible to split it to 2 ports)
* 9 Pin DB9 Female to 3.4mm male jack with adapter to 6.3mm (-> ebay, amazon)

## Detail install instructions

* [windows](binaries/win/Release/Readme.md)
* [linux64](binaries/linux64/Readme.md)
* [aarch64](binaries/aarch64/Readme.md)
* [mac](binaries/mac/Readme.md)

## Information about testing and compiling

### windows serial testing

com0com

https://www.com-port-monitoring.com/downloads.html

### Linux serial testing

Copy port 
socat -d -d pty,raw,echo=0 pty,raw,echo=0

echo -ne '\xbe\x70\x6f\x5f\x40\x30\x20\xa1\x0d\x1f\x2f\x3f' > /dev/ttys005

Receive data 
sudo minicom -D /dev/ttyS0

protocol

https://www.cmrr.umn.edu/~strupp/serial.html


### Rspberry compile addons

sudo apt-get install libssl-dev cmake

Repair the serail port with - this helps!
jpnevulator --ascii --timing-print --tty /dev/ttyUSB1:SB9600d --read

## mqtt install

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

## Windows compile 

https://github.com/Microsoft/vcpkg

```bash
git clone https://github.com/Microsoft/vcpkg.git
PS> .\bootstrap-vcpkg.bat

```

CMake projects should use: "-DCMAKE_TOOLCHAIN_FILE=C:/Users/User/git/vcpkg/scripts/buildsystems/vcpkg.cmake"

```
vcpkg install openssl:x86-windows
vcpkg.exe install pthreads
``` 

c++ compiler
https://docs.microsoft.com/de-de/cpp/build/building-on-the-command-line?view=vs-2019
--> Build Tools
--> English language pack

without compiler install 
* vc_redistx86.exe - included in binaries
* on pre windows 10 - windows 10 universal c runtime


## RPi install

sudo apt-get install libssl-dev cmake

### serial with gpio

sudo raspi-config
Select option 5, Interfacing options, then option P6, Serial, and select No. Exit raspi-config.


### build with gpio

```bash
wget https://github.com/joan2937/pigpio/archive/master.zip
unzip master.zip
cd pigpio-master
make
sudo make install
```

```bash
## there is a config file to turn gpio on and off
cd build
cmake ../

```

### checks

dmesg | grep tty

Configuartion in CMakeList.txt
serial console is off:

```bash
--> port is /dev/ttySO

[    0.000536] printk: console [tty1] enabled
[    2.812083] fe201000.serial: ttyAMA0 at MMIO 0xfe201000 (irq = 14, base_baud = 0) is a PL011 rev2
[    2.818171] fe215040.serial: ttyS0 at MMIO 0x0 (irq = 16, base_baud = 62500000) is a 16550
```

serial console is on:

```bash
-> port /dev/ttyAMA0
[    0.000541] printk: console [tty1] enabled
[    2.441992] fe201000.serial: ttyAMA0 at MMIO 0xfe201000 (irq = 19, base_baud = 0) is a PL011 rev2
[   19.953410] usb 1-1.3: ch341-uart converter now attached to ttyUSB0
```

## output of test data 

### jpnevulator

```bash
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

```bash
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

```bash
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
```

#### Arduino

```bash
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
```