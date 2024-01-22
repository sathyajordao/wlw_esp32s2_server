| Supported Targets | ESP32 | ESP32-S2 | SHT4x | SPIFFS | 7-seg-display |
| ----------------- | ----- | -------- | ----- | ------ | ------------- |

# WLW HS23 Webserver Project
This is a small project, using an ESP32-S2 module to read out temperature and humidity values of a SHT40 sensor.
The ESP32-S2 acts as a webserver and hosts a simple web application containing the measurements.
The website is from a webbrowser accessible and values are shown as a chart.

## What the example does

### 1.Server (ESP32):

The ESP32 acts as a server using the AsyncWebServer library.
It connects to your local WiFi network using the provided SSID and password.
It initializes I2C communication using the Wire library (Wire.begin(SDA_PIN, SCL_PIN)) to communicate with the SHT40 sensor.
The readSensor() function is responsible for communicating with the SHT40 sensor, reading temperature and humidity values, and displaying them on multiple 7-segment displays.
The server hosts a simple web application with one HTML file (index.html), and the root ("/") route serves this HTML file.
The server provides an API endpoint ("/data") that responds with JSON data containing temperature and humidity values.


### 2. Client (Web Browser):

When a client (e.g., a web browser) makes a request to the ESP32 server, it gets served the HTML file, which includes JavaScript code.
The JavaScript code in the HTML file establishes communication with the server using the fetch API. Specifically, it fetches sensor data from the "/data" endpoint.
The fetched data is then displayed on the web page, and the temperature and humidity values are updated in real-time.
The Chart.js library is used to visualize temperature data in a line chart on the web page.
The loop() function continuously calls readSensor() every second, reading sensor data, updating the 7-segment displays, and sending JSON data to the client.
The server-side logic is primarily focused on handling incoming requests from clients.


### 3. Communication Flow:

The ESP32, acting as a server, continuously reads sensor data in the background and updates the 7-segment displays.
When a client (web browser) accesses the ESP32 server, it gets the HTML file, and the JavaScript in the HTML file starts fetching sensor data from the "/data" endpoint every 5 seconds.
The fetched data is then displayed on the web page, and the line chart is updated accordingly.


In summary, the ESP32 server collects sensor data, serves a web page to clients, and provides an API endpoint for clients to fetch real-time sensor data. The client (web browser) fetches data from the server, updates the web page, and visualizes the data using Chart.js.

## How to use the example

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
