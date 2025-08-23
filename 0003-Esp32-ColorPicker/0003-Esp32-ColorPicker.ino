/*********
  Trevor Pham
  Complete project details at https://ohmbedkernel.substack.com/
*********/

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

// Declare WS2812 Objec
#define PIN_WS2812B 16
#define NUM_PIXELS 192
Adafruit_NeoPixel ws2812b(NUM_PIXELS, PIN_WS2812B, NEO_GRB + NEO_KHZ800);

byte r, g, b;

// Replaces placeholder
String processor(const String& var){
  Serial.println(var);
}
 
void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  // Initialize ws2812
  ws2812b.begin();
  ws2812b.clear();
  ws2812b.show();

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

void loop(){
  ws2812b.clear();
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++) {
    ws2812b.setPixelColor(pixel, ws2812b.Color(r, g, b));
  }
  ws2812b.show();
  delay(100);
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

  server.on("/setcolor", HTTP_GET, [](AsyncWebServerRequest *request){
  if (request->hasParam("color")) {
    String colorHex = request->getParam("color")->value();
    Serial.println("Color received: " + colorHex);
    setLedColorFromHex(colorHex);
  }
  });

  server.begin();
}

/***************************************************************/
/*                          WS2812                            */
/***************************************************************/

void setLedColorFromHex(String hex) {

  if (hex.charAt(0) == '#') {
    hex.remove(0, 1);
  }

  long color = strtol(hex.c_str(), NULL, 16);
  
  r = (color >> 16) & 0xFF;
  g = (color >> 8) & 0xFF;
  b = color & 0xFF;
}