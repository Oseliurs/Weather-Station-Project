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
#define DHTTYPE    DHT22     // DHT 22 (AM2302)

//------------------------------------------------------------------------------------------
//Parameters
const char* ssid = "Livebox-68D7"; // Wifi Name
const char* mqttServer = "rutaceae.ddns.net"; // Wifi Password
const int mqttPort = 1883; // MQTT Port
const char* mqttUser = "user"; // MQTT User
const char* mqttPassword = "Ov96Hf66&*";// MQTT Password

//------------------------------------------------------------------------------------------
//Variables
uint32_t delayMS = 2000; // Delay Between Mesurements (in Ms)

float temp;
float humi;
float pres;

char temp_s[10];
char humi_s[10];
char pres_s[10];

//------------------------------------------------------------------------------------------
//Objects
DHT_Unified dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);
Adafruit_BMP085 bmp;

//------------------------------------------------------------------------------------------
//Setup
void setup() {
  //----------------------------------------------
  Serial.begin(9600); //Start Serial

  //----------------------------------------------
  //DHT22 Sensor Init
  dht.begin();

  //----------------------------------------------
  //BMP085 Sensor Init
  if (!bmp.begin()) {
  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  while (1) {}
  }

  //----------------------------------------------
  //Wifi Init
  WiFi.begin(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

  //----------------------------------------------
  //Wifi Init
  client.setServer(mqttServer, mqttPort);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
  //----------------------------------------------
}

//------------------------------------------------------------------------------------------
//Loop
void loop() {
  delay(delayMS); // Delay between measurements.

  //
  // Verify Connection to MQTT & Wifi

  // Connect to Wifi
  if (WiFi.status() != WL_CONNECTED){
    WiFi.begin(ssid);
  }
  
  // Wait for Connexion
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  // Connect to MQTT
  while (!client.connected()){
    client.connect("ESP8266Client", mqttUser, mqttPassword ))
    delay(2000)
  }

  
  //----------------------------------------------
  //DHT22 Sensors Part
  sensors_event_t event;
  dht.humidity().getEvent(&event);

  if (isnan(event.relative_humidity)) {
    Serial.println("Error Reading Humidity");
  } else {
    humi = event.relative_humidity;
    Serial.println("Sucessfully Read Humidity");
  }

  //----------------------------------------------
  //BMP085 Sensor Part

  temp = bmp.readTemperature();
  pres = bmp.readPressure();
  
  //----------------------------------------------
  //String Formatting
  sprintf(temp_s, "%g", temp);
  sprintf(humi_s, "%g", humi);
  sprintf(pres_s, "%g", pres);

  //----------------------------------------------
  // Publish the recordings
  client.publish("sensor0/temp", temp_s);
  client.publish("sensor0/humi", humi_s);
  client.publish("sensor0/pres", pres_s);

  //
  //
  Serial.println("Reporting the Data:");
  Serial.print("Temperature ");
  Serial.print(temp);
  Serial.print(" / ");
  Serial.println(temp_s);
  Serial.print("Humidité ");
  Serial.print(humi);
  Serial.print(" / ");
  Serial.println(humi_s);
  Serial.print("Pression ");
  Serial.print(pres);
  Serial.print(" / ");
  Serial.println(pres_s);
}
