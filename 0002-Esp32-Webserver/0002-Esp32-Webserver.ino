// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "LittleFS.h"
#include <Adafruit_NeoPixel.h>

// Replace with your network credentials
const char* ssid = "REPLACE_BY_YOUR_SSID";
const char* password = "REPLACE_BY_YOUR_PWD";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// GPIO
#define LED1_PIN 2
#define LED2_PIN 4

// Replaces placeholder
String processor(const String& var){
  Serial.println(var);
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);

  // GPIO Init
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  digitalWrite(LED1_PIN, HIGH);
  digitalWrite(LED2_PIN, HIGH);

  // Initialize LittleFS
  if(!LittleFS.begin(true)){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Start server
  WebServerInit();
}

void loop() {
  // put your main code here, to run repeatedly:

}

/***************************************************************/
/*                        WEBSERVER                            */
/***************************************************************/

void  WebServerInit(void)
{
  Serial.println("Setting webserver");

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html", String(), false, processor);
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/style.css", "text/css");
  });

  // Route to load script.js file
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/script.js", "application/javascript");
  });

  server.on("/toggle", HTTP_GET, [](AsyncWebServerRequest *request){
    if (!request->hasParam("led")) {
      request->send(400, "text/plain", "Missing LED param");
      return;
    }
    int led = request->getParam("led")->value().toInt();
    int pin = (led == 1) ? LED1_PIN : (led == 2) ? LED2_PIN : -1;

    if (pin == -1) {
      request->send(400, "text/plain", "Invalid LED number");
      return;
    }

    bool current = digitalRead(pin);
    digitalWrite(pin, !current);
    request->send(200, "text/plain", !current ? "ON" : "OFF");
    Serial.print("Toggle GPIO: ");
    Serial.println(pin);
  });

  // Get LED status
  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request){
    if (!request->hasParam("led")) {
      request->send(400, "text/plain", "Missing LED param");
      return;
    }
    int led = request->getParam("led")->value().toInt();
    int pin = (led == 1) ? LED1_PIN : (led == 2) ? LED2_PIN : -1;

    if (pin == -1) {
      request->send(400, "text/plain", "Invalid LED number");
      return;
    }

    request->send(200, "text/plain", digitalRead(pin) ? "OFF" : "ON");
  });

  server.begin();
}