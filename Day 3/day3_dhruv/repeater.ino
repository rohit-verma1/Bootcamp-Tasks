#include <ESP8266WiFi.h>

const char* ssid = "iPhone "; // Replace with your Wi-Fi network name
const char* password = "12345678"; // Replace with your Wi-Fi password
const int repeater_port = 80;

WiFiServer server(80);
WiFiClient senderClient;
WiFiClient receiverClient;

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

  // Set up the server
  server.begin();
}

void loop() {
  if (!senderClient.connected()) {
    // Try to connect to the sender (NodeMCU 1)
    senderClient.connect("172.20.10.2", repeater_port); // Replace with sender's IP address

    if (senderClient.connected()) {
      Serial.println("Connected to sender");
    }
  }

  if (server.hasClient()) {
    if (!receiverClient || !receiverClient.connected()) {
      receiverClient = server.available();
      Serial.println("Receiver connected");
    }
  }

  
    while (senderClient.available()) {
      char message = senderClient.read(); // Read a character from sender (NodeMCU 1)
      Serial.print("Received from sender: ");
      Serial.println(message);

      // Forward the message to the receiver (NodeMCU 3)
      receiverClient.print(message);
      Serial.print("Forwarded to receiver: ");
      Serial.println(message);
    


  }
}
