# Readme install arm x64

## Important

Take a FTDI USB/Serial converter to avoid issues. With ftdi libs it is easy, don't forget to turn of load of ftdi_sio lib, if you use direct ftdi access.

With ubuntu 20, gpio didn't work. The programm was tested on 64bit versions, should also work on 32bit, you must compile it by your own.

## Install ftdi driver

* Copy both libs to /lib
* create link sudo ln -s /lib/libftd2xx.so.1.4.27 /lib/libftd2xx.so
* remove normal ftdi lib sudo rmmod ftdi_sio
* add ftdi_sio to /etc/modprobe.d/blacklist.conf

Attention: forward mode with ftdi not possible

## Install as service

copy content to /lib/systemd/system/colorado.service

systemctl enable colorado
systemctl start colorado

```bash
[Unit]
Description=colorado
After=network.target

[Service] 
ExecStart=/usr/bin/colorado -z -f
WorkingDirectory=/opt/colorado
StandardOutput=syslog
StandardError=syslog
SyslogIdentifier=colorado
Restart=always
User=root

[Install]
WantedBy=multi-user.target
```

## Errors

### mosquitto

./serial: error while loading shared libraries: libmosquitto.so.1: cannot open shared object file: No such file or directory  

Missing install of mqtt

* sudo apt install mosquitto
* sudo apt install mosquitto-clients

### ftdi

./serial: error while loading shared libraries: libftd2xx.so: cannot open shared object file: No such file or directory

Missin ftdi libs - see above
