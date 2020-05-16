#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <DHT.h>
#include <Wire.h>

const char* ssid = "La Lanterna - L1";
const char* password = "vitrathat";

AsyncWebServer server(80);

#define dhtPin D1
#define dhtType DHT11
#define flamePin D7
DHT dht(dhtPin, dhtType);
float t = 0.0;
float h = 0.0;
float a = 0.0;

unsigned long previousMillis = 0;

String processor(const String& var) {
  if (var == "TEMPERATURE") return String(t);
  else
    if (var == "HUMIDITY") return String(h);
    else 
      if (var == "ALCOHOL") return String(a);
  return String(); 
}

void setup() {
  Serial.begin(115200);
  
  // Initialize sensor
  dht.begin();
  pinMode(flamePin, INPUT);

  // Initialize SPIFFS
  if (!SPIFFS.begin()) 
  {
    Serial.println("Error SPIFFS");
    return;
  }

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting");
    delay(5000);
  }
  Serial.println("Connected");
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/index.html", String(), false, processor);
    });
  server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/index.html", String(), false, processor);
    })
  server.on("/content.html", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/content.html", String(), false, processor);
    });
  
  server.on("/css/styles.css", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/css/styles.css", "text/css");
    });
  server.on("/css/index.css", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/css/index.css", "text/css");
    });
  server.on("/css/content.css", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/css/content.css", "text/css");
    });

  // Route for images -- No need, we use online images :>
//  server.on("/images/cuttie-cat.png", HTTP_GET, [](AsyncWebServerRequest *request) {
//      request->send(SPIFFS, "/images/cuttie-cat.png", "image/png");
//    });
//  server.on("/images/alcohol-vector.svg", HTTP_GET, [](AsyncWebServerRequest *request) {
//      request->send(SPIFFS, "/images/alcohol-vector.svg", "image/svg+xml");
//    });
//  server.on("/images/humidity-vector.svg", HTTP_GET, [](AsyncWebServerRequest *request) {
//      request->send(SPIFFS, "/images/humidity-vector.svg", "image/svg+xml");
//    });
//  server.on("/images/temperature-vector.svg", HTTP_GET, [](AsyncWebServerRequest *request) {
//      request->send(SPIFFS, "/images/temperature-vector.svg", "image/svg+xml");
//    });

  // Route to get data
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/plain", String(t).c_str());
    });  
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/plain", String(h).c_str());
    });
  server.on("/alcohol", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/plain", String(a).c_str());
    });

  // Start server
  server.begin();
}

void loop() {
  unsigned long currentMillis = millis();
  if (millis() - previousMillis >= 1000)
  {
    previousMillis = currentMillis;
    float newT = dht.readTemperature();
    float newH = dht.readHumidity();
    float newA = analogRead(A0);
    int f = digitalRead(flamePin);  

    if (f == LOW) Serial.println("FIRE");
    
    if (isnan(newT) || isnan(newH))
      Serial.println("Fail to read from DHT sensor");
    else
      if (isnan(newA))
        Serial.println("Fail to read alcohol value");
      else
      {
        t = newT;
        h = newH;
        a = newA;
        Serial.print(t); Serial.print(" "); Serial.print(h); Serial.print(" "); Serial.println(a);
      }
  }
}
