# Windows

## Base Install 

### install npm

Install node.js `12.16.2`  
<https://nodejs.org/en/download/>

### instal mqtt

Install 1.6.9  
<https://projects.eclipse.org/projects/iot.mosquitto>  
check with mosquitto_sub -t raw, if its running (files under c:\program\mosquitto)

### install nginx

Install 1.17.10
nginx download
put nginx into autostart

### Test Installation

* C:\Program Files\mosquitto>mosquitto_sub.exe -t rawdata should work, mqtt server is running
* Open localhost in Browser - nginx
* npm -version -> e.g. 6.14.3 depends on install date

### using pm2

* Create a new folder `c:\etc\.pm2`
* Create a new PM2_HOME variable (at System level, not User level) and set the value c:\etc\.pm2
* Close all your open terminal windows (or restart Windows)
* Ensure that your PM2_HOME has been set properly, running `echo %PM2_HOME%`


## App Install

### npm start with pm2

* npm install pm2 -g 
* in datampapping: pm2 start app.js --name datamapping
* in websocket: pm2 start app.js --name ws
* pm2 save

### copy website to nginx

* in frontend: npm run build
* copy folder content of build ro nginx html