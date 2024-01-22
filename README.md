| Supported Targets | ESP32 | ESP32-S2 | SHT4x | SPIFFS | 7-seg-display |
| ----------------- | ----- | -------- | ----- | ------ | ------------- |

# WLW HS23 Webserver Project
This is a small project, using an ESP32-S2 module to read out temperature and humidity values of a SHT40 sensor.
The ESP32-S2 acts as a webserver and hosts a simple web application containing the measurements.
The website is from a webbrowser accessible and values are shown as chart.

## How to use example

### Hardware Required
PC + USB Serial adapter connected to USB port + Espressif ESP32-S2 development board + SHT40 with required passive components and connected to to specified pins on development board (see below for more information).

The ESP32S2DEVKITM development board ESP32-S2-MINI-1 is used for this example below but other similar chips/boards can be used as well.

![image](https://github.com/sathyajordao/wlw_esp32s2_server/assets/103050063/62e72636-e153-4eb0-a70c-edd9c11356f4)



### Software Required
The development board is accessed with ArduinoIDE. Please make sure that the necessary libraries and tools are installed. Especially the tool SPIFFS (Serial Peripheral Interface Flash File System) to upload the website structure index.html file is important. 


#### project schematic:
```
+-----------------------+            +--------------------------+
|      ESP32 Server     |            |       Web Browser        |
|                       |            |                          |
|   +---------------+   |            |   +------------------+   |
|   |  Sensor Data  |   |  HTTP GET  |   |                  |   |
|   |    Fetch      |   +------------>   |                  |   |
|   |   (Interval)  |   |  JSON Data |   |                  |   |
|   +---------------+   |            |   |                  |   |
|                       |            |   |                  |   |
|   +---------------+   |            |   |                  |   |
|   |   Chart.js    |   |            |   |                  |   |
|   |   Rendering   |   |            |   |                  |   |
|   +---------------+   |            |   +------------------+   |
|                       |            |                          |
+-----------------------+            +--------------------------+

```

#### Connect an external RS485 serial interface to an ESP32 board
Connect a USB-to-RS485 adapter to a computer, then connect the adapter's A/B output lines with the corresponding A/B output lines of the RS485 line driver connected to the ESP32 chip (see figure above).
```
