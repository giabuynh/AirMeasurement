#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <DHT.h>
#include <Wire.h>

const char* ssid = "Gia Binh";
const char* password = "giabuynh";

AsyncWebServer server(80);

#define dhtPin D1
#define dhtType DHT11
#define flamePin D7
DHT dht(dhtPin, dhtType);
float t = 0.0;
float h = 0.0;
float a = 0.0;
int f = HIGH;
unsigned long previousMillis = 0;

const int channelID = 1063062; 
String writeAPIKey = "RAOKCHK6EQOSPSTR"; // write API key for your ThingSpeak Channel
const char* thingspeak_server = "api.thingspeak.com";
WiFiClient client;

String processor(const String& var) {
  if (var == "TEMPERATURE") return String(t);
  else
    if (var == "HUMIDITY") return String(h);
    else
      if (var == "ALCOHOL") return String(a);
      else
        if (var == "FLAME") return String(f);
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
    });
  server.on("/content.html", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/content.html", String(), false, processor);
    });
  server.on("/chart.html", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/chart.html", String(), false, processor);
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
  server.on("/css/chart.css", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/css/chart.css", "text/css");
    });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/script.js", "text/javascript");
    });

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
  server.on("/flame", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send_P(200, "text/plain", String(f).c_str());
    });

  // Start server
  server.begin();
}

float analogToPPM(float sensor_value) {
  float sensor_volt = (float) sensor_value / 1025 * 5.0;
  float RS = ((5 - sensor_volt)/sensor_volt);
  float R0 = 0.02;
  float ratio = RS/R0;
  float lgPPM = (log10(ratio)* (-2.6)) + 2.7;
  return (pow(10, lgPPM));
}

void loop() {
  unsigned long currentMillis = millis();
  if (millis() - previousMillis >= 1000)
  {
    previousMillis = currentMillis;
    float newT = dht.readTemperature();
    float newH = dht.readHumidity();
    float newA = analogRead(A0);
    newA = analogToPPM(newA);
    int newF = digitalRead(flamePin);
  
    f = newF;
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
        if (client.connect(thingspeak_server, 80))
        {
          String str = "field1=" + String(t, 2) + "&field2=" + String(h, 2) + "&field3=" + String(a, 2);
          client.print("POST /update HTTP/1.1\n");
          client.print("Host: api.thingspeak.com\n");
          client.print("Connection: close\n");
          client.print("X-THINGSPEAKAPIKEY: " + writeAPIKey + "\n");
          client.print("Content-Type: application/x-www-form-urlencoded\n");
          client.print("Content-Length: ");
          client.print(str.length());
          client.print("\n\n");
          client.print(str);
          client.print("\n\n");
        }
        client.stop();
      }
  }
}
