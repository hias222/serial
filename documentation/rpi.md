# RPi

## RP

Create or Download the RP Image

* use image 32 bit - easy
* use ubuntu 64 bit - for server 
* Install wit balenaetcher

## Prepare Image

* put empty file with name ssh in root boot partition for ssh access
* ssh access is open at first startup

## First Startup

Start the RP, WLAN is configured during Image creation, if you use the image only use a lan cable  

* connect to the rpi

```bash
ssh pi@<IP>
password: <password>
```

## use vnc server

```bash
# if not installed - normally it is installed with rpi image
sudo apt update
sudo apt install realvnc-vnc-server realvnc-vnc-viewer

sudo raspi-config
```

Now, enable VNC Server by doing the following:

* Navigate to Interfacing Options.
* Scroll down and select VNC > Yes.


### set video mode

Prepare static display. Display is not needed on reboot.

* sudo /opt/vc/bin/tvservice -d /boot/edid.dat
* and add hdmi_edid_file=1 to config.txt.


### disable wlan sleep mode

iwconfig
iw wlan0 set power_save off
--> rc.local

## installs

use ansibleswim repo for installation
## Urls

### Main

There are two modes, the prefered mode can configured during build of react app, base is static  
The startup text is a configured value in websocke_tbackend/app.js - staticbasemessage, can be edited with vi  

```html
http://[IP Address]
```

#### For static use

http://[IP Address>]/static

#### For web

http://[IP Address]/responsive

### Admin

http://[IP]:8080/

You can upload new lenex file under settings, if cloud is connected, this part is also updated  

### MQTT logs

http://[IP]:3002/ - no automatic update, browser refresh must be used

this is an monitoring app. It collects the last messages and put it to website - no auto reload!! When a arduino/esp connects you see somethin like MQTT started an every few seconds something like no serial data

# Addons

## 7 zoll

```bash
hdmi_group=2
hdmi_mode=1
hdmi_mode=87
hdmi_cvt 1024 600 60 6 0 0 0
max_usb_current=1 
```