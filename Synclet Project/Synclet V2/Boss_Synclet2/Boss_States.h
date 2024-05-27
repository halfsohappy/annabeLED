
void stateCSVtoRGB(uint16_t h, uint8_t s, uint8_t v){
  uint32_t packed = strip.ColorHSV(h, s, v);
  hsv[0] = uint8_t ((packed >> 16) & 0xFF); //red
  hsv[1] = uint8_t ((packed >> 8) & 0xFF); //green
  hsv[2] = uint8_t (packed & 0xFF); //blue
}

uint8_t red(uint32_t pac){
  return (pac >> 16) & 0xFF;
}
uint8_t green(uint32_t pac){
  return (pac >> 8) & 0xFF;
}
uint8_t blue(uint32_t pac){
  return pac & 0xFF;
}

void sendHeteroState(uint32_t pack[]){
  myMsg.pat = NULL;
  myMsg.state = true;
  for(int i = 0; i< NUM_SYNCLETS; i++){
    uint32_t pacman = pack[i];
    myMsg.hue = uint8_t ((pacman >> 16) & 0xFF); //red
    myMsg.sat = uint8_t ((pacman >> 8) & 0xFF); //green
    myMsg.val = uint8_t (pacman & 0xFF); //blue
    esp_now_send(myPeers[i].peer_addr, (uint8_t *) &myMsg, sizeof(msg_struct));
  }
}

void sendHomoState(uint32_t pack){
  myMsg.pat = NULL;
  myMsg.hue = uint8_t ((pack >> 16) & 0xFF); //red
  myMsg.sat = uint8_t ((pack >> 8) & 0xFF); //green
  myMsg.val = uint8_t (pack & 0xFF); //blue
  myMsg.state = true;
  for(int i = 0; i< NUM_SYNCLETS; i++){
    esp_now_send(myPeers[i].peer_addr, (uint8_t *) &myMsg, sizeof(msg_struct));
  }
}