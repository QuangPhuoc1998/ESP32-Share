/*********
  Trevor Pham
  Complete project details at https://ohmbedkernel.substack.com/
*********/

// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "LittleFS.h"
#include "DHT.h"

// Replace with your network credentials
const char* ssid = "REPLACE_BY_YOUR_SSID";
const char* password = "REPLACE_BY_YOUR_PWD";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// DHT sensor settings
#define DHTPIN  16       // GPIO where the DHT is connected
#define DHTTYPE DHT11   // DHT11 or DHT22
DHT dht(DHTPIN, DHTTYPE);

float temperature = 0.0;
float humidity = 0.0;

// Replaces placeholder in HTML file
String processor(const String& var) {
  if (var == "TEMPERATURE") {
    return String(temperature);
  }
  else if (var == "HUMIDITY") {
    return String(humidity);
  }
  return String();
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);

  // Initialize DHT sensor
  dht.begin();

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
  // Read temperature and humidity from DHT sensor
  float t = dht.readTemperature();   // Default °C
  float h = dht.readHumidity();

  // Check if any reads failed
  if (isnan(t) || isnan(h)) {
    Serial.println("❌ Failed to read from DHT sensor!");
    return;
  }

  // Update global variables
  temperature = t;
  humidity = h;

  // Debugging output
  Serial.print("🌡 Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, ");
  Serial.print("💧 Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  delay(2000); // DHT sensor needs ~2s between reads
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

  // API route to get JSON sensor data
  server.on("/sensor", HTTP_GET, [](AsyncWebServerRequest *request) {
    String json = "{\"temperature\":" + String(temperature, 2) + 
                  ",\"humidity\":" + String(humidity, 2) + "}";
    request->send(200, "application/json", json);
  });

  server.begin();
}
