#include <ESP8266WiFi.h>
#include <espnow.h>
int branch_num = 0 ;
// REPLACE WITH THE MAC Address of your receiver 
uint8_t broadcast1Address[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t broadcast2Address[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t broadcast3Address[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t broadcast4Address[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t parentAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Updates readings every 10 Microseconds
const long interval = 10; 
unsigned long previousMillis = 0;   
String address = "";
String message = "";

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
    String address;
    String message;
    int branch ;
} struct_message;

struct_message outgoingmessage;

// Create a struct_message to hold incoming sensor readings
struct_message incomingmessage;

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0) {
    Serial.println("Delivery success");
  } else {
    Serial.println("Delivery fail");
  }
}

// Callback when data is received
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
  // Init Serial Monitor
  Serial.begin(115200);
  Serial.print("This node AP mac: "); 
  Serial.println(WiFi.softAPmacAddress());
  Serial.print("This node STA mac: ");
  Serial.println(WiFi.macAddress());
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  
  // Initialize ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Set ESP-NOW Role
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

  // Register for Send CB to get the status of Transmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peers
  esp_now_add_peer(broadcast1Address, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  esp_now_add_peer(broadcast2Address, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  esp_now_add_peer(broadcast3Address, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  esp_now_add_peer(broadcast4Address, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  esp_now_add_peer(parentAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    char secondChar = message[0];
    message = message.substring(1);
    outgoingmessage.address = address;
    outgoingmessage.message = message;
    outgoingmessage.branch = branch_num+1 ;
    
    if (message.length() >= 1) {
      switch (secondChar) {
        case 'a':
          esp_now_send(broadcast1Address, (uint8_t *)&outgoingmessage, sizeof(outgoingmessage));
          break;
        case 'b':
          esp_now_send(broadcast2Address, (uint8_t *)&outgoingmessage, sizeof(outgoingmessage));
          break;
        case 'c':
          esp_now_send(broadcast3Address, (uint8_t *)&outgoingmessage, sizeof(outgoingmessage));
          break;
        case 'd':
          esp_now_send(broadcast4Address, (uint8_t *)&outgoingmessage, sizeof(outgoingmessage));
          break;
      }
    }
    else {
      // Write the function you want to perform with the message
      esp_now_send(parentAddress, (uint8_t *)&outgoingmessage, sizeof(outgoingmessage));
    }
  }
}
