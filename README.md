| Supported Targets | ESP32 | ESP32-S2 | SHT4x | SPIFFS | 7-seg-display |
| ----------------- | ----- | -------- | ----- | ------ | ------------- |

# WLW HS23 Webserver Project
This is a small project, using an ESP32-S2 module to read out temperature and humidity values of a SHT40 sensor.
The ESP32-S2 acts as a webserver and hosts a simple web application containing the measurements.
The website is from a webbrowser accessible and values are shown as a chart.

## Project Overview 

### project schematic:
```
+-----------------------+            +--------------------------+
|      ESP32 Server     |            |       Web Browser        |
|                       |            |                          |
|   +---------------+   |            |   +------------------+   |
|   |   main.ino    |   |            |   |                  |   |
|   |     API       |   |  HTTP GET  |   |                  |   |
|   +---------------+   +------------>   |   |              |   |
|   |  Rendering    |   |  JSON Data |   |   |       ___/   |   |
|   | index.html    |   |            |   |   |   __/        |   |
|   |  chart.js     |   |            |   |   |_/__________  |   |
|   +---------------+   |            |   |                  |   |
|   | Sensor Data   |   |            |   |                  |   |
|   |   Fetch       |   |            |   |                  |   |
|   |  (Interval)   |   |            |   +------------------+   |
|   +---------------+   |            |                          |
|                       |            +--------------------------+
+-----------------------+
   |                   |
   |  I2C              |  GPIO
+---------------+     +---------------------------------+
|   SHT40       |     |    __   __   __   7-seg-display |
|   temp & hum  |     |   |__| |__| |__|                |
+---------------+     |   |__|.|__|.|__|.               |
                      +---------------------------------+

```


## What the example does

### 1. Server (ESP32):

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
PC + USB Serial adapter connected to USB port + Espressif ESP32-S2 development board + 7-segment display LEDs + SHT40 with required passive components and connected to to specified pins on development board (see below for more information).

The ESP32S2DEVKITM development board ESP32-S2-MINI-1 is used for this example, but other similar chips/boards can be used as well.

The 7-segment display LEDs are connected to GPIO pins of the development board. For the exact pin configuration see the pin definitions in the .ino file.

The typical usage of the SHT4x is shown below.

![image](https://github.com/sathyajordao/wlw_esp32s2_server/assets/103050063/62e72636-e153-4eb0-a70c-edd9c11356f4)



### Software Required
The development board is accessed with ArduinoIDE. Please make sure that the necessary libraries and tools are installed. Especially the tool SPIFFS to upload the website structure index.html file is important. 
SPIFFS (Serial Peripheral Interface Flash File System) is a file system designed for SPI NOR flash memory on embedded systems. It allows storing and retrieving files on non-volatile storage like the flash memory of an ESP32. SPIFFS is used to store and serve the HTML file ("index.html") that represents the web page served by the ESP32.
Please note that the folder structure and name has to be as in this example.


### Step-by-Step Guide
#### 1. Hardware Setup:
- Connect the ESP32 board to your computer.
- Connect the SHT40 sensor to the ESP32 using the I2C protocol. Ensure the correct wiring for SDA and SCL pins.
- Connect the 7-segment displays to the GPIO pins specified in the code (e.g., segA_display1, segB_display1, etc.).

#### 2. Software Setup:
- Install the necessary Arduino libraries:
   - ESPAsyncWebServer
   - ArduinoJson
   - Wire (already included in Arduino IDE for I2C communication)
   - Chart.js (ensure the CDN link in your HTML file is accessible)

#### 3. WiFi Configuration:
- Open the main.ino file and update the ssid and password variables with your WiFi credentials.

#### 4. Upload the Code:
- Connect the ESP32 to your computer via USB.
- Select the correct ESP32 board and port in the Arduino IDE.
- Upload the code to the ESP32.

#### 5. SPIFFS Setup:
- Ensure SPIFFS (SPI Flash File System) is mounted successfully. The code checks for this during setup.

#### 6. Access the Web Interface:
- Once uploaded, open the Serial Monitor to view the ESP32's IP address.
- Enter this IP address in a web browser to access the IoT Weather Station interface.

#### 7. View Sensor Data:
- The web interface displays real-time temperature and humidity from the SHT40 sensor.
- The temperature trend is visualized using Chart.js.

#### 8. Customization (Optional):
- Modify the code, HTML, or CSS files to customize the appearance or behavior of the temperature/humidity station.

#### 9. Configure Port Forwarding (Optional):
- Access your router settings and set up port forwarding for the port (default is 80) used by the ESP32 server.
- This allows the web interface to be accessible from anywhere on the internet using your public IP address or domain name.


## Example Output (Pictures)

### Hardware Setup

![IMG_5172](https://github.com/sathyajordao/wlw_esp32s2_server/assets/103050063/e18fe20d-c31e-462f-9a17-97bab2a30049)
