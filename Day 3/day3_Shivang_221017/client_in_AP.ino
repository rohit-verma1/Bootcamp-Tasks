#include <ESP8266WiFi.h>

const char* ssid = "HELLO";
const char* password = "12345678";
const char* server_ip = "192.168.4.1"; // IP address of the root NodeMCU
const int server_port = 80; // Port on which the server is running

void setup() {
  Serial.begin(115200);
  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to Access Point...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to Access Point");
}

void loop() {
  WiFiClient client;
  if (client.connect(server_ip, server_port)) {
    Serial.println("Connected to Server");
    client.println("Hello from Client!"); // Send data to the server
    client.flush();
    delay(1000); // Wait for a response (adjust as needed)
    while (client.available()) {
      String response = client.readStringUntil('\r');
      Serial.println("Received: " + response); // Process server response here
    }
    client.stop();
    Serial.println("Disconnected from Server");
  }
  delay(5000); // Repeat every 5 seconds (adjust as needed)
}
