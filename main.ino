/*

This is the main file for the ESP8266 based Weather Station

*/

//------------------------------------------------------------------------------------------
//Libraries
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

//------------------------------------------------------------------------------------------
//Hardware
#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22     // DHT 22 (AM2302)

//------------------------------------------------------------------------------------------
//Parameters
const char* ssid = "Livebox-68D7"; // Enter your WiFi name
const char* mqttServer = "rutaceae.ddns.net";
const int mqttPort = 1883;
const char* mqttUser = "user";
const char* mqttPassword = "Ov96Hf66&*";

//------------------------------------------------------------------------------------------
//Variables
uint32_t delayMS = 2000; // Delay Between Mesurements (in Ms)

//------------------------------------------------------------------------------------------
//Objects
DHT_Unified dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

//------------------------------------------------------------------------------------------
//Setup
void setup() {
  Serial.begin(9600); //Start Serial

  dht.begin();
  delayMS = sensor.min_delay / 1000;
  
  
}

//------------------------------------------------------------------------------------------
//Loop
void loop() {
  delay(delayMS); // Delay between measurements.

}
