#include <Wire.h>
#include <SPI.h>

#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include "DFRobot_VEML7700.h"

#include <WiFi.h>
#include <ArduinoHttpClient.h>

#include <string>
#include <iostream>
#include "config.h"

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME680 bme(BME_CS);
DFRobot_VEML7700 veml;

char ssid[]{WIFI_SSID};
char pass[]{WIFI_PASSWORD};

const char serverName[]{"weatherapi.chickencurious.com"};
int port{80};

WiFiClient wifi;
HttpClient client{HttpClient(wifi, serverName, port)};
int status{WL_IDLE_STATUS};

// Timing vars
uint64_t lastMillis{};
const uint64_t interval{1000 * 60 * 10}; // aka 10 minutes, using uint64 for consistency in this logic

void setup() {
  Serial.begin(115200);
  // while (!Serial); // Not required for "production"

  // Startup bme interface
  if (!bme.begin()) {
    Serial.println("Bad BME Startup");
    while (1);
  }
  Serial.println("BME Good");

  veml.begin();  // Unfortunately no bool/truthy value for error handling

  // Attempt WiFi connection until successful
  while (status != WL_CONNECTED) {
      status = initiateConnection();
      delay(1000);
  }

  // Display networking details for debug purposes
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void loop() {
  uint64_t currentMillis = millis();

  if(currentMillis - lastMillis >= interval) {
    lastMillis = currentMillis;

    doReading();    
  }

  // Special handling for overflow and first run
  if(lastMillis - currentMillis >= UINT64_MAX - 10000 || lastMillis == 0) {
    lastMillis = currentMillis;

    doReading();
  }

  // Check on WiFi status to maintain connection in intervals between calls
  if (status != WL_CONNECTED) {
    Serial.println("WiFi connection lost.");
    while (status != WL_CONNECTED) {
      status = initiateConnection();
      delay(1000);
    }
    Serial.println("Regained.");
  }

  bme.performReading(); // Mostly for the purpose of keeping the gas sensor warm
  delay(10000); // No need to spam check (basically "are we there yet? are we there yet?")
}

int initiateConnection() {
  Serial.print("Attempting connection to: ");
  Serial.println(ssid);

  int result = WiFi.begin(ssid, pass);
  return result;
}

void doReading() {
  // Try to obtain BME reading
    if (!bme.performReading()) {
      Serial.println("Bad read");
      return;
    }

    // Console outputs
    Serial.print("Temperature = ");
    Serial.print(bme.temperature);
    Serial.println(" *C");

    Serial.print("Pressure = ");
    Serial.print(bme.pressure / 100.0);
    Serial.println(" hPa");

    Serial.print("Humidity = ");
    Serial.print(bme.humidity);
    Serial.println(" %");

    Serial.print("Gas = ");
    Serial.print(bme.gas_resistance / 1000.0);
    Serial.println(" KOhms");


    float lux;
    veml.getALSLux(lux);

    Serial.print("lux: ");
    Serial.print(lux);
    Serial.println();

    Serial.println();

    // Things get messy because of a combination of std::string and Arduino::String (if only ArduinoHttpClient accepted std::string...)
    String contentType = "application/json";
    std::string postData = "{ \"temperature\":" + std::to_string(bme.temperature) + ", \"humidity\":" + std::to_string(bme.humidity) + ", \"pressure\":" + std::to_string(bme.pressure) + ", \"gas\":" + std::to_string(bme.gas_resistance) + ", \"light\":" + std::to_string(lux) + " }";

    Serial.print(String(postData.c_str()));
    Serial.println();

    client.post("/weather/current/", contentType, String(postData.c_str())); // Post to current weather endpoint

    int statusCode = client.responseStatusCode();
    String response = client.responseBody();

    Serial.print("Status code: ");
    Serial.println(statusCode);
    Serial.print("Response: ");
    Serial.println(response);
}