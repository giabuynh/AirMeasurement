# Air Measurement with Adruino
This is an IoT system displays parameters such as temperature, humidity, and alcohol content in the air on the web.

## Devices
* ESP8266 NodeMCU
![ESP8266 NodeMCU](/report/images/esp8266-serial-wireless-module-nodemcu-v3-lua-wifi-internet.jpg "ESP8266 NodeMCU")
* Temperature and humidity sensor DHT11
![DHT11](/report/images/DHT11-sensor.jpg "DHT11")
* Fire sensor
![Fire sensor](/report/images/Flame-sensor.jpg)
* Alcohol sensor MQ-3
![MQ3 sensor](/report/images/IMG_E0140.JPG "MQ3")

![Connection diagrams](/report/images/AirMeasurement.png "Connection diagrams")

## System model
ESP8266 is the main processor which controls and powers the sensors. Power suplly for it could be plugged directly from the computer. In addition, ESP8266 is in charge as an asynchronous HTTP server. The system also uses [ThingSpeak](https://thingspeak.com/) to visualize data.

![System model](/report/images/system.png "Model of the system")

## SPIFFS
SPIFFS stands for Serial Peripheral Interface Flash File System. SPIFFS allows accessing and manipulating flash memory as easily as a normal file system on a computer, but simpler and more limited. This system supports various storage versions (1MB, 2MB or 3MB), which can be used to stored static documents. This project using SPIFFS to separate web code, such as HTML, CSS, JS or image, from Ardruino sketch.

![Folder structure](/report/images/spiffs.png)

## Demo
Enter your wifi name and wifi password.
```
const char *ssid = "Wifi name"; // YOUR WIFI NAME
const char *password = "12345678";  
```
Enter your ThingSpeak channel ID and write API.
```
const int channelID = 1063062;           // ThingSpeck channel ID
String writeAPIKey = "RAOKCHK6EQOSPSTR"; // write API key for your ThingSpeak Channel
```
Run the sketch.
ESP8266 will automatically connect to wifi and print out an IP address of the form 192.168.xxx.xxx. This is the IP address the network modem allocates to it. Type the IP address into web browser and press enter.

Index page.
![Index page](/report/images/index.png "Index page").

Chart page.
![Chart page](/report/images/chart-page.png "Graph from ThingSpeak")

Real-time data page.
![Real-time data page](/report/images/content-page.png "Real-time data") 

When the system receives a fire detection signal, it will automatically raise a warning dialog box.
![Fire alert](/report/images/model.png "Fire alert")
