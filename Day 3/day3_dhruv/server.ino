#include <ESP8266WiFi.h>

const char* ssid = "iPhone "; // Remove the space at the end of the SSID
const char* password = "12345678";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());


  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    // Serial.println("Client connected");

    while (client.connected()) {
      if (Serial.available()) {
        char message = Serial.read(); // Read a character from the Serial Monitor
        client.print(message); // Send the character to the client
        Serial.print("Sent to client: ");
        Serial.println(message);
      }
    }

   
  }
}
