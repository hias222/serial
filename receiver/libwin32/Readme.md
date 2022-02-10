# Readme install windows

## Install

Download driver from <https://ftdichip.com/drivers/d2xx-drivers/>  
For windows you can use the setup file
## Start

Copy alle files in one directory and start serial like  

```bash
serial.exe -f 
or
serial.exe -s <portname like com3>
```

## Start as service

Two steps

* Trust serial system to system
* Install tool NSSM and create a service with serial.exe 

## Errors

### FTD2XX.dll is missing

Install drivers and check that ftd2xx.dll is in the serial folder