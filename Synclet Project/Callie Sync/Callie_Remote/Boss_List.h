#include <esp_now.h>
#include <WiFi.h>
//mac adresses:
uint8_t baA[] = {0x34, 0x85, 0x18, 0x04, 0x4F, 0x44};
uint8_t baB[] = {0x34, 0x85, 0x18, 0x00, 0x8D, 0x7C};
uint8_t baC[] = {0x34, 0x85, 0x18, 0x03, 0xAD, 0x40};
uint8_t baD[] = {0x34, 0x85, 0x18, 0x03, 0x26, 0x9C};
uint8_t baE[] = {0x34, 0x85, 0x18, 0x03, 0x38, 0x20};
uint8_t baF[] = {0x34, 0x85, 0x18, 0x03, 0x90, 0x90};
uint8_t baG[] = {0x34, 0x85, 0x18, 0x03, 0xFF, 0xDC};
uint8_t baH[] = {0x34, 0x85, 0x18, 0x03, 0x85, 0xC0};

uint8_t friendly[] = {0xA0, 0x76, 0x4E, 0x4A, 0x75, 0x8C};


typedef struct msg_struct {
  char pat;
  uint16_t hue;
  uint8_t sat;
  uint8_t val;
  long msec;
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
