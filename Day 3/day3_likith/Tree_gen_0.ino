#include <ESP8266WiFi.h>
#include <espnow.h>
#include <HTTPClient.h>

// Replace with your SSID and password
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";

typedef struct struct_message {
    String address;
    String message;
    int branch ;
} struct_message;


// MAC address of the receiver NodeMCU 
uint8_t receiverMac[5];
receiverMac[0] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
receiverMac[1] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
receiverMac[2] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
receiverMac[3] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
receiverMac[4] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};

// Callback function to handle received data
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&incomingmessage, incomingData, sizeof(incomingmessage));
  Serial.print("Bytes received: ");
  Serial.println(len);
  address = incomingmessage.address;
  message = incomingmessage.message;
  branch_num = incomingmessage.branch ;
  Serial.print("This is ");
  Serial.print(branch_num);
  Serial.println(" Generation")
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the callback function for receiving data
  esp_now_register_recv_cb(onDataReceived);

for( int i =0 ;i<6 ;i++){
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, receiverMac[i], 6);
  peerInfo.channel = 1; // Choose the channel you want
  peerInfo.encrypt = false; // You can enable encryption if needed

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.print("Error adding the reciever") ;
    Serial.print(i+1);
    Seial.println("as a peer");
    return;
  }
}
}

void loop() {
  // Create an HTTP client object
  HTTPClient http;

  // Your website URL to scrape data from
  String websiteURL = "http://example.com"; // Replace with your website URL

  // Send an HTTP GET request to the website
  http.begin(websiteURL);
  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    // Read the response from the website
    String websiteData = http.getString();

    // Close the HTTP client
    http.end();

    // Send the scraped data to the receiver NodeMCU (#2)
    uint8_t dataToSend[websiteData.length()];
    websiteData.getBytes(dataToSend, websiteData.length());

    if (esp_now_send(receiverMac, dataToSend, websiteData.length()) == ESP_OK) {
      Serial.println("Data sent successfully");
    } else {
      Serial.println("Error sending data");
    }
  }
   else {
    Serial.println("HTTP request failed");
    http.end();
  }

  delay(1000); // Scrape and send data every 1 seconds (adjust as needed)
}
