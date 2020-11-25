# Readme install mac x64

## install ftdi drivers mode

* Copy libftd2xx.1.4.16.dylib to the /usr/local/lib directory 
* Change directory to the /usr/local/lib (cd /usr/local/lib)·
* Create a symbolic link to the library (ln -sf libftd2xx.1.4.16.dylib libftd2xx.dylib)

## test with serial-sender

socat -d -d pty,raw,echo=0 pty,raw,echo=0

## Install mqtt

* docker pull eclipse-mosquitto
* docker run --rm -it -p 1883:1883 -p 9001:9001 eclipse-mosquitto 
or
* docker run -d --rm -p 1883:1883 -p 9001:9001 eclipse-mosquitto

