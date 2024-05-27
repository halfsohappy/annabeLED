
uint32_t individual_states[NUM_DANCERS] = {};

// void stateCSVtoRGB(uint16_t h, uint8_t s, uint8_t v){
//   uint32_t packed = strip.ColorHSV(h, s, v);
//   hsv[0] = uint8_t ((packed >> 16) & 0xFF); //red
//   hsv[1] = uint8_t ((packed >> 8) & 0xFF); //green
//   hsv[2] = uint8_t (packed & 0xFF); //blue
// }


uint8_t red(uint32_t pac){
 return (pac >> 16) & 0xFF;
}
uint8_t green(uint32_t pac){
 return (pac >> 8) & 0xFF;
}
uint8_t blue(uint32_t pac){
 return pac & 0xFF;
}


void send_hetero_state(){
 figure.use_state = true;
 for(int i = 0; i< NUM_DANCERS; i++){
   figure.state = individual_states[i];
   esp_now_send(myPeers[i].peer_addr, (uint8_t *) &figure, sizeof(Figure));
 }
}


void send_homo_state(){
 figure.use_state = true;
 for(int i = 0; i< NUM_DANCERS; i++){
   esp_now_send(myPeers[i].peer_addr, (uint8_t *) &figure, sizeof(Figure));
 }
}

// SETTING STATE: FOR ADJUSTING COLOR VALUES AND SEEING YOUR WORK

void setting_state(){
  
}


