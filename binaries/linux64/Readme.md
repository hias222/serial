# Readme install linux x64

## Install ftdi driver

* Copy both libs to /lib
* remove normal ftdi lib sudo rmmod ftdi_sio
* add ftdi_sio to /etc/modprobe.d/blacklist.conf

Attention: forward mode with ftdi not possible