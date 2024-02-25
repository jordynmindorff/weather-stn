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

char ssid[] = WIFI_SSID;
char pass[] = WIFI_PASSWORD;

const char serverName[] = "weatherapi.chickencurious.com";
int port = 80;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverName, port);
int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // Crucial function to setup
  if (!bme.begin()) {
    Serial.println("Bad BME Startup");
    while (1);
  }
  Serial.println("BME Good");

  veml.begin();  // Unfortunately no bool/truthy value for error handling

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);

    status = WiFi.begin(ssid, pass);
  }

  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void loop() {
  if (!bme.performReading()) {
    Serial.println("Bad read");
    return;
  }

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

  // Things get messy because of a combination of std::string and Arduino::String
  String contentType = "application/json";
  std::string postData = "{ "temperature":" + std::to_string(bme.temperature) + ", "humidity":" + std::to_string(bme.humidity) + ", "pressure":" + std::to_string(bme.pressure) + ", "gas":" + std::to_string(bme.gas_resistance) + ", "light":" + std::to_string(lux) + " }";

  Serial.print(String(postData.c_str()));
  Serial.println();

  client.post("/weather/current/", contentType, String(postData.c_str()));

  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);

  delay(100000);
}