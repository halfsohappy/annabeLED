//mac adresses:

uint8_t dancerMAC[NUM_DANCERS][6] = {
 {0x34, 0x85, 0x18, 0x05, 0xC9, 0xF0}, //A
 {0x34, 0x85, 0x18, 0x05, 0xD8, 0x0C}, //B
 {0x34, 0x85, 0x18, 0x07, 0x10, 0xBC}, //C
 {0x34, 0x85, 0x18, 0x04, 0x44, 0x10}, //D
 {0x34, 0x85, 0x18, 0x03, 0x9F, 0x64}, //E
 {0x34, 0x85, 0x18, 0x05, 0x63, 0x7C}, //F
 {0x34, 0x85, 0x18, 0x05, 0x73, 0x20}, //G
 {0x34, 0x85, 0x18, 0x03, 0xA7, 0x28} //H
};

esp_now_peer_info_t myPeers[NUM_DANCERS] = {};

void pairWithDancers(){
 for(int i = 0; i < NUM_DANCERS; i++){
   memcpy(myPeers[i].peer_addr, dancerMAC[i], 6);
   myPeers[i].channel = 0; 
   myPeers[i].encrypt = false;
   esp_now_add_peer(&myPeers[i]);
 }
}

uint8_t failures[NUM_DANCERS];

void logFail(const uint8_t *mac_addr){
 uint8_t lastMAC = mac_addr[5];
 uint8_t pos = 0;
 for(int i = 0; i< NUM_DANCERS; i++){
   if(lastMAC == dancerMAC[i][5]){failures[i] += 1; pos = i;}
 }
 if(failures[pos]>=3){
   esp_now_del_peer(mac_addr);
   Serial.print("Removed "); Serial.print(char(pos + 65)); Serial.print(" at"); Serial.println(millis());}
}

// callback when data is sent
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
   if(!(status == ESP_NOW_SEND_SUCCESS) && millis()<7000){logFail(mac_addr);}
}
