#include <ESP8266WiFi.h>

const char* ssid = "HELLO";
const char* password = "12345678";
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  server.begin();
  Serial.println("Access Point Mode: ON");
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Client connected");
    while (client.connected()) {
      if (client.available()) {
        String request = client.readStringUntil('\r');
        Serial.println("Received: " + request);
        // You can process the received data here and send a response if needed.
        client.println("Hello from Access Point!");
        client.flush();
      }
    }
    client.stop();
    Serial.println("Client disconnected");
  }
}
