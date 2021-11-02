# Overall Install

## Overview

the software has these components:

* arduino serial converter (not included)
* Mosquito pub/sub -> mosquito (docker, Server on windows linux)
* backend -> datamapping (Nodejs)
* frontend backend with websocket -> websocket_backend (Nodejs)
* react frontend -> react_fontend/swimfrontend (react, Nodejs)
* scripts for generating RPi image -> raspberry (bash, pi-gen, <https://github.com/RPi-Distro/pi-gen)>
* scripts -> generate packages for RPi pi-gen (bash)
* monitor

## Colorado

[preferences](documentation_for_colorado.docx)

## Details

[RPi](rpi.md)

## Installs

### mosquito

<https://hub.docker.com/_/eclipse-mosquitto>
docker pull eclipse-mosquitto

```bash
docker run -it -p 1883:1883 -p 9001:9001 -v mosquitto.conf:/mosquitto/config/mosquitto.conf eclipse-mosquitto

docker run -it -p 1883:1883 -p 9001:9001 -v mosquitto.conf:/mosquitto/config/mosquitto.conf -v /mosquitto/data -v /mosquitto/log eclipse-mosquitto
```

Stay in mosquitto folder:  

```bash
docker run -it -p 1883:1883 -p 9001:9001 -v $(pwd)/mosquitto.conf:/mosquitto/config/mosquitto.conf -v /mosquitto/data -v $(pwd)/log:/mosquitto/log eclipse-mosquitto

docker run -d -p 1883:1883 -p 9001:9001 -v $(pwd)/mosquitto.conf:/mosqtto/config/mosquitto.conf -v /mosquitto/data -v $(pwd)/log:/mosquitto/log eclipse-mosquitto
```

### mosquitto on mac

brew install mosquitto
brew list mosquitto
ln -s /usr/local/Cellar/mosquitto/1.6.8/bin/mosquitto_sub /usr/local/bin/mosquitto_sub

### node + pm2 

LTS Version 15.x downlode from nodejs.org as windows installer

npm install pm2@latest -g

## Apps node

### datamapping

```bash
git clone git@github.com:hias222/datamapping.git
cd datamapping 
npm install
copy simple.env .env
node app.js
# or
pm2 start app.js
```

### websocket-backend

```bash
git clone git@github.com:hias222/displaysocket.git
cd displaysocket
npm install
node app.js
# or
pm2 start app.js
```

## react-frontend

### livetiming display

```bash
git clone git@github.com:hias222/displayweb.git
cd displayweb
npm install
copy simple.env .env
npm start
```

```bash
git clone git@github.com:hias222/adminweb.git
cd adminweb
npm i
copy simple.env .env
npm install

```

## Tests

### Subscripe to channel

```bash
mosquitto_sub -h localhost -t mainchannel
mosquitto_sub -h localhost -t rawdata
```

### Generate some messages

```bash
mosquitto_pub -h localhost -t rawdata -m hello
```

Use something like postman to send messages

```bash
http://localhost:3001/send-mqtt
```

### Check with postman

The datamapping backend has a post webservice on send-mqtt

```bash
{message: <stop>/<start>/<lane 1 2 3>}
```

## Frontends

```bash
datamapping http://localhost:3001/
react http://localhost:3000/
websocket http://localhost:4001/
```

## Deployment

### build on raspberry

run script in folder scripts

```bash
creat_apps.sh
```

Create new rspberry image out of pi-gen, create a config file out of example

```bash
./build-docker -c client
```

<https://www.digitalocean.com/community/tutorials/how-to-build-a-node-js-application-with-docker>

https://mherman.org/blog/dockerizing-a-react-app/

### React only

React hast two environment files

* .env.development -> for start with npm start
* .env.production -> for prod build with npm build

To build a docker container:

```bash
docker build -f Dockerfile -t sample:prod .
docker run -it -p 3000:80 --rm sample:prod

docker run -d -p 3000:80 --rm sample:prod

docker-compose -f docker-compose-prod.yml up -d --build
```

### Websocket

docker-compose up


# Addons

## 7 zoll

```bash
hdmi_group=2
hdmi_mode=1
hdmi_mode=87
hdmi_cvt 1024 600 60 6 0 0 0
max_usb_current=1 
```