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

 // Config Temperature
  sensors.begin();
}

void loop()
{
  float latitud = 0.0, longitud = 0.0;

  obtenerCoordenadas(latitud, longitud); // Obtener coordenadas
  enviarDatos(latitud, longitud);

  // Leer temperatura
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");

  // Enviar los datos
  if (temperatureC != DEVICE_DISCONNECTED_C) {
    sendTemperatureData(id, temperatureC, petId);
  } else {
    Serial.println("Error al leer la temperatura.");
  }

  delay(900000);// 15 minutos
}

// Enviar datos a la API
void sendTemperatureData(int id, float temperature, int petId) {
  if (WiFi.status() == WL_CONNECTED) {
    client.begin(TemperatureEndpoint);
    client.addHeader("Content-Type", "application/json");

    StaticJsonDocument<200> jsonDoc;
    jsonDoc["id"] = id;
    jsonDoc["temperature"] = temperature;
    jsonDoc["petId"] = petId;

    String requestBody;
    serializeJson(jsonDoc, requestBody);

    int httpResponseCode = client.POST(requestBody);

    if (httpResponseCode > 0) {
      String response = client.getString();
      Serial.println("Respuesta del servidor: " + response);
    } else {
      Serial.println("Error en la solicitud POST: " + String(httpResponseCode));
    }

    client.end();
  } else {
    Serial.println("WiFi no está conectado.");
  }
}

