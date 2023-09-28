// SenderCode 
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *ssid = "Your_Wi-Fi_Direct_SSID";
const char *password = "Your_Wi-Fi_Direct_Password";
const int udpPort = 12345; // Choose a UDP port

WiFiUDP udp;
uint8_t dataToSend ;
void setup() {
  Serial.begin(115200);
  delay(1000);

  // Connect to Wi-Fi Direct
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi Direct...");
  }

  Serial.println("Connected to Wi-Fi Direct!");
  dataToSend =  Wifi.Mac();
  udp.begin(udpPort);
}

void loop() {

  // Send the 8-bit data to the receiver
  udp.beginPacket(IPAddress(192, 168, 4, 2), udpPort); // Replace with the IP address of the receiver
  udp.write(&dataToSend, sizeof(uint8_t));
  udp.endPacket();

  Serial.println("Data sent successfully!");

  delay(1000); // Delay between sending data
}


// Reciever Code

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *ssid = "Your_Wi-Fi_Direct_SSID";
const char *password = "Your_Wi-Fi_Direct_Password";
const int udpPort = 12345; // Choose the same UDP port as the sender

WiFiUDP udp;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Connect to Wi-Fi Direct
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi Direct...");
  }

  Serial.println("Connected to Wi-Fi Direct!");

  udp.begin(udpPort);

  Serial.println("Waiting for data...");
}

void loop() {
  uint8_t receivedData;
  int packetSize = udp.parsePacket();

  if (packetSize) {
    udp.read(&receivedData, sizeof(uint8_t));
    Serial.print("Received data: ");
    Serial.println(receivedData);
  }

  // Your code here
}
