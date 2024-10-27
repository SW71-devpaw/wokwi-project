#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>

// WiFi Credentials
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""

// Endpoints
#define TemperatureEndpoint "https://ENDPOINT.mockapi.io/api/v1/temperature-records"
#define LocationEndpoint "https://ENDPOINT.mockapi.io/api/v1/location"
#define GeolocationAPI "https://api.ipgeolocation.io/ipgeo?apiKey=miapikey"

// HTTP Client
HTTPClient client;


// OneWire bus
const int oneWireBus = 12;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

// Datos
const int petId = 101;  // Ejemplo de ID de mascota
const int id = 1;       // Ejemplo de ID único para el registro

void setup()
{
  Serial.begin(115200);

  // WIFI
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

