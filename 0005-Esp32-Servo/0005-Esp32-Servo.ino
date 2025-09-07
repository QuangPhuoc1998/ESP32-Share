/*********
  Trevor Pham
  Complete project details at https://ohmbedkernel.substack.com/
*********/

// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "LittleFS.h"
#include <ESP32Servo.h>

// Replace with your network credentials
const char* ssid = "REPLACE_BY_YOUR_SSID";
const char* password = "REPLACE_BY_YOUR_PWD";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Declare for servo motor
Servo MyServo;
static const int MyServoPin = 13;

// Replaces placeholder in HTML file
String processor(const String& var) {
  return String();
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);

  // Initialize Servo motor 
  MyServo.attach(MyServoPin);

  // Initialize LittleFS
  if (!LittleFS.begin(true)) {
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

  // Start web server
  WebServerInit();
}

void loop() {
  delay(1000);
}

/***************************************************************/
/*                        WEBSERVER                            */
/***************************************************************/

void WebServerInit(void) {
  Serial.println("Setting webserver");

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html", String(), false, processor);
  });

  // Route to load style.css
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/style.css", "text/css");
  });

  // Route to load script.js
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/script.js", "application/javascript");
  });

  // Route for set servo angle
  server.on("/servo", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("angle")) {
      String angleParam = request->getParam("angle")->value();
      int angle = angleParam.toInt();
      angle = constrain(angle, 0, 90);
      MyServo.write(angle*2);
      Serial.printf("Servo set to: %d°\n", angle);
      request->send(200, "text/plain", "Servo angle set to " + String(angle));
    } else {
      request->send(400, "text/plain", "Missing angle parameter");
    }
  });

  server.begin();
}
