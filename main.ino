/*
  ESP32 IoT Weather Station with 7-Segment Display

  This Arduino code is designed for an ESP32-based IoT weather station
  that reads temperature and humidity data from an SHT40 sensor using I2C.
  The sensor data is displayed on three 7-segment displays, and the web interface
  provides real-time updates using Chart.js for temperature trends. The server
  exposes an API endpoint for fetching sensor data, and the main HTML file and
  associated resources are served from the ESP32 SPIFFS filesystem.

  Hardware Setup:
  - ESP32 with WiFi capabilities
  - SHT40 sensor connected via I2C
  - Three 7-segment displays for temperature visualization

  Dependencies:
  - ESPAsyncWebServer for handling HTTP requests
  - ArduinoJson for creating JSON responses
  - Chart.js library for dynamic chart rendering

  Created by Sathya De Albuquerque Jordao
  Date: 15.1.2024
*/


#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <Wire.h>

#define SENSOR_ADDRESS 0x44 // I2C address of the sensor
#define SDA_PIN 20          // SDA pin number
#define SCL_PIN 21          // SCL pin number

// 7-segment display pins connected to GPIOs
// Display #1
const int segA_display1 = 8;
const int segB_display1 = 7;
const int segC_display1 = 6;
const int dot_display1 = 5;
const int segD_display1 = 4;
const int segE_display1 = 3;
const int segF_display1 = 2;
const int segG_display1 = 1;

// Display #2
const int segA_display2 = 34;
const int segB_display2 = 35;
const int segC_display2 = 36;
const int dot_display2 = 37;
const int segD_display2 = 39;
const int segE_display2 = 40;
const int segF_display2 = 41;
const int segG_display2 = 42;

// Display #3
const int segA_display3 = 17;
const int segB_display3 = 16;
const int segC_display3 = 15;
const int dot_display3 = 14;
const int segD_display3 = 13;
const int segE_display3 = 12;
const int segF_display3 = 11;
const int segG_display3 = 10;

// WLAN at home
const char *ssid = "Your_Network";
const char *password = "Your_Password";

float temp_value;
float humid_value;

AsyncWebServer server(80);

// Function to read sensor data
void readSensor() {
    // Write command to request data
    Wire.beginTransmission(SENSOR_ADDRESS);
    Wire.write(0xFD); // Command byte
    Wire.endTransmission();

    delay(10); // Wait for the sensor to process the command

    // Read data from the sensor
    Wire.requestFrom(SENSOR_ADDRESS, 6); // Request 6 bytes of data
    if (Wire.available() == 6) {
        uint8_t rx_bytes[6];
        for (int i = 0; i < 6; i++) {
            rx_bytes[i] = Wire.read();
        }

        // Process received data
        uint16_t t_ticks = (rx_bytes[0] << 8) | rx_bytes[1];
        uint16_t rh_ticks = (rx_bytes[3] << 8) | rx_bytes[4];

        float t_degC = -45.0 + 175.0 * t_ticks / 65535.0;
        float rh_pRH = -6.0 + 125.0 * rh_ticks / 65535.0;

        // Ensure RH is within valid range
        if (rh_pRH > 100.0) {
            rh_pRH = 100.0;
        } else if (rh_pRH < 0.0) {
            rh_pRH = 0.0;
        }

        temp_value = t_degC;
        humid_value = rh_pRH;

        // Print temperature and humidity
        Serial.print("Temperature: ");
        Serial.print(t_degC);
        Serial.println(" °C");
        Serial.print("Humidity: ");
        Serial.print(rh_pRH);
        Serial.println(" %");

        // Extracting individual digits
        int temp_int = int(temp_value * 10); // Convert to integer and multiply by 10 to preserve the first decimal digit
        int temp_display1 = (temp_int / 100) % 10; // Extract the hundreds digit
        int temp_display2 = (temp_int / 10) % 10;  // Extract the tens digit
        int temp_display3 = temp_int % 10;         // Extract the ones digit

        String dotOFF = "n";
        String dotON = "p";

        // Display temperature digits on respective displays
        displayNumber(temp_display1, 1, dotOFF); // Display hundreds digit on display #1 with dot OFF
        displayNumber(temp_display2, 2, dotON);  // Display tens digit on display #2 with dot ON
        displayNumber(temp_display3, 3, dotOFF); // Display ones digit on display #3 with dot OFF

        delay(1000); // Delay before next reading (adjust as needed)
    } else {
        Serial.println("Error: Unable to read data from the sensor.");
        delay(1000); // Delay before retrying
    }
}

// Function to display a number on the 7-segment display
void displayNumber(int number, int displayNum, String dotControl) {
    // Define pins for all displays as outputs
    // ... (pinMode declarations for all displays)

    // Set pins for Display #1 as outputs
    pinMode(segA_display1, OUTPUT);
    pinMode(segB_display1, OUTPUT);
    pinMode(segC_display1, OUTPUT);
    pinMode(dot_display1, OUTPUT);
    pinMode(segD_display1, OUTPUT);
    pinMode(segE_display1, OUTPUT);
    pinMode(segF_display1, OUTPUT);
    pinMode(segG_display1, OUTPUT);

    // Set pins for Display #2 as outputs
    pinMode(segA_display2, OUTPUT);
    pinMode(segB_display2, OUTPUT);
    pinMode(segC_display2, OUTPUT);
    pinMode(dot_display2, OUTPUT);
    pinMode(segD_display2, OUTPUT);
    pinMode(segE_display2, OUTPUT);
    pinMode(segF_display2, OUTPUT);
    pinMode(segG_display2, OUTPUT);

    // Set pins for Display #3 as outputs
    pinMode(segA_display3, OUTPUT);
    pinMode(segB_display3, OUTPUT);
    pinMode(segC_display3, OUTPUT);
    pinMode(dot_display3, OUTPUT);
    pinMode(segD_display3, OUTPUT);
    pinMode(segE_display3, OUTPUT);
    pinMode(segF_display3, OUTPUT);
    pinMode(segG_display3, OUTPUT);

    // Define the patterns for displaying numbers
    const int numbers[10][7] = {
        {0, 0, 0, 0, 0, 0, 1},  // 0
        {1, 0, 0, 1, 1, 1, 1},  // 1
        {0, 0, 1, 0, 0, 1, 0},  // 2
        {0, 0, 0, 0, 1, 1, 0},  // 3
        {1, 0, 0, 1, 1, 0, 0},  // 4
        {0, 1, 0, 0, 1, 0, 0},  // 5
        {0, 1, 0, 0, 0, 0, 0},  // 6
        {0, 0, 0, 1, 1, 1, 1},  // 7
        {0, 0, 0, 0, 0, 0, 0},  // 8
        {0, 0, 0, 0, 1, 0, 0}   // 9
    };

    int displayPins[7]; // Array to hold pin numbers for each segment
    int dotPin;         // Variable to hold the dot pin number

    // Determine the dot pin based on the displayNum
    if (displayNum == 1) {
        dotPin = dot_display1;
        // Set the dot segment based on dotControl input
        if (dotControl == "n") {
            digitalWrite(dotPin, HIGH); // Turn off dot segment
        } else if (dotControl == "p") {
            digitalWrite(dotPin, LOW); // Turn on dot segment
        }
        // Assign displayPins based on displayNum (remaining code remains the same as the previous function)
        displayPins[0] = segA_display1;
        displayPins[1] = segB_display1;
        displayPins[2] = segC_display1;
        displayPins[3] = segD_display1;
        displayPins[4] = segE_display1;
        displayPins[5] = segF_display1;
        displayPins[6] = segG_display1;
    } else if (displayNum == 2) {
        dotPin = dot_display2;
        // Set the dot segment based on dotControl input
        if (dotControl == "n") {
            digitalWrite(dotPin, HIGH); // Turn off dot segment
        } else if (dotControl == "p") {
            digitalWrite(dotPin, LOW); // Turn on dot segment
        }
        displayPins[0] = segA_display2;
        displayPins[1] = segB_display2;
        displayPins[2] = segC_display2;
        displayPins[3] = segD_display2;
        displayPins[4] = segE_display2;
        displayPins[5] = segF_display2;
        displayPins[6] = segG_display2;
    } else if (displayNum == 3) {
        dotPin = dot_display3;
        // Set the dot segment based on dotControl input
        if (dotControl == "n") {
            digitalWrite(dotPin, HIGH); // Turn off dot segment
        } else if (dotControl == "p") {
            digitalWrite(dotPin, LOW); // Turn on dot segment
        }
        displayPins[0] = segA_display3;
        displayPins[1] = segB_display3;
        displayPins[2] = segC_display3;
        displayPins[3] = segD_display3;
        displayPins[4] = segE_display3;
        displayPins[5] = segF_display3;
        displayPins[6] = segG_display3;
    }

    // Display the selected number on the chosen display
    for (int i = 0; i < 7; ++i) {
        digitalWrite(displayPins[i], numbers[number][i]);
    }
}

void setup() {
    Serial.begin(115200);

    Wire.begin(SDA_PIN, SCL_PIN);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    Serial.println("");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    if (!SPIFFS.begin(true)) {
        Serial.println("An error occurred while mounting SPIFFS");
        return;
    }

    // Serve the main HTML page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", "text/html");
    });

    readSensor(); // Initial sensor read

    // Handle requests for sensor data
    server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request) {
        // Create a JSON object with temperature and humidity values
        DynamicJsonDocument doc(1024);
        doc["sensor"] = "SHT40";
        doc["temperature"] = temp_value;
        doc["humidity"] = humid_value;

        // Convert the JSON object to a string
        String jsonString;
        serializeJson(doc, jsonString);

        // Respond with the JSON string
        request->send(200, "application/json", jsonString);
    });

    server.begin();
}

void loop() {
    readSensor(); // Periodically read sensor data
    delay(1000);
}
