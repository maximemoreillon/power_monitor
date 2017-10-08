# Power monitor

WiFi and MQTT enabled home energy monitor.
Allows monitoring of the apparent power consumption of a household.
The system also features a DHT22 temperature and humidity sensor as well as the ability to get OTA updates.

## Requirements
* Arduino IDE with [support for ESP8266](https://github.com/esp8266/Arduino)
* [Async MQTT client](https://github.com/marvinroger/async-mqtt-client)
* [ESPAsyncTCP](https://github.com/me-no-dev/ESPAsyncTCP)
* [emonLib](https://github.com/openenergymonitor/EmonLib)
* [ArduinoJson](https://github.com/bblanchon/ArduinoJson)

## Hardware
### Parts

* Wemos D1 mini
* YHDC SCT-013-000 current transformer
* DHT22 temperature and humidity sensor

### Schematic
#### Main board
![alt text](https://moreillon.duckdns.org/projects/iot/images/power_schematic.png)

Note: the DS18B20 temperature sensor has been replaced with a DHT 22 temperature and humidity sensor.

#### CT sensor breakout board
![alt text](https://moreillon.duckdns.org/projects/iot/images/power_breakout_schematic.png)



