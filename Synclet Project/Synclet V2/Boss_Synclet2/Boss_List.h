#include <esp_now.h>
#include <WiFi.h>

#define NUM_SYNCLETS 8
//mac adresses:
uint8_t syncletMAC[NUM_SYNCLETS][6] = {
    {0x34, 0x85, 0x18, 0x04, 0x4F, 0x44}, //A
    {0x34, 0x85, 0x18, 0x00, 0x8D, 0x7C}, //B
    {0x34, 0x85, 0x18, 0x03, 0xAD, 0x40}, //C
    {0x34, 0x85, 0x18, 0x03, 0x26, 0x9C}, //D
    {0x34, 0x85, 0x18, 0x03, 0x38, 0x20}, //E
    {0x34, 0x85, 0x18, 0x03, 0x90, 0x90}, //F
    {0x34, 0x85, 0x18, 0x03, 0xFF, 0xDC}, //G
    {0x34, 0x85, 0x18, 0x03, 0x85, 0xC0} //H
};

esp_now_peer_info_t myPeers[NUM_SYNCLETS] = {};

void pairWithSynclets(){
  for(int i = 0; i < NUM_SYNCLETS; i++){
    memcpy(myPeers[i].peer_addr, syncletMAC[i], 6);
    myPeers[i].channel = 0;  
    myPeers[i].encrypt = false;
    esp_now_add_peer(&myPeers[i]);
  }
}

// void createStates(int (&myarray)[NUM_SYNCLETS]){
//   for (int i = 0; i < NUM_SYNCLETS; i++)
//      myarray [i] += 42; 
//   }  
// }

typedef struct msg_struct {
  char pat;
  uint16_t hue;
  uint8_t sat;
  uint8_t val;
  bool state;
} msg_struct;

msg_struct myMsg;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  Serial.print("Packet to: ");
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
