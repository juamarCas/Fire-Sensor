# Fire sensor network
This project conssists on a prototype of a Wireless Sensor Network(WSN) for fire detection
## Sensor node
It uses the Atmega328 (Arduino one).
connector J1 you can connect a battery to measure the energy left.

### Code
the code for each sensor is in  _Transmmiter.ino_ 
when you configure the channels you have to know that it has to be a
number in octal format like this:
```
int this_node = 01; 
```

## Base node
It uses NodeMCU with the ESP8266, 

### Code 
the code is _BaseNode2.ino_

## Libraries
-We used the nrf24L01 library to communicate each node, you can find the library here [GitHub Pages](https://github.com/nRF24/RF24)

-To make a network we used nrf24l01 network library [GitHub Pages](https://github.com/nRF24/RF24Network)

-To meassure temperature and humidity we used the DTH22 library 
[GitHub Pages](https://github.com/adafruit/DHT-sensor-library)

-To send data to the server we used ArduinoJson [ArduinoJson] (https://arduinojson.org/)

