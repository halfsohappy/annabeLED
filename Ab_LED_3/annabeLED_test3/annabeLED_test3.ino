#include <Adafruit_NeoPixel.h>
#include <esp_now.h>
#include <WiFi.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Smooth.h>

Adafruit_MPU6050 mpu;

Smooth smoothOperator(15);
#define NUM_DANCERS 8
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

uint8_t failures[NUM_DANCERS];

void logFail(const uint8_t *mac_addr){
  uint8_t lastMAC = mac_addr[5];
  uint8_t pos = 0;
  for(int i = 0; i< NUM_DANCERS; i++){
    if(lastMAC == dancerMAC[i][5]){
      failures[i] += 1;
      pos = i;
    }
  }

  if (failures[pos]>=3){esp_now_del_peer(mac_addr); Serial.print("Removed "); Serial.print(char(pos + 65)); Serial.print(" at"); Serial.println(millis());}
}
esp_now_peer_info_t myPeers[NUM_DANCERS] = {};

Adafruit_NeoPixel statusL(1, 18, NEO_GRB + NEO_KHZ800);
//18

typedef struct msg_struct {
  char pat;
  uint16_t hue;
  uint8_t sat;
  uint8_t val;
  bool state;
} msg_struct;

msg_struct myMsg;

esp_now_peer_info_t peerInfo;

uint8_t red(uint32_t pac){
  return (pac >> 16) & 0xFF;
}
uint8_t green(uint32_t pac){
  return (pac >> 8) & 0xFF;
}
uint8_t blue(uint32_t pac){
  return pac & 0xFF;
}

void pairWithDancers(){
  for(int i = 0; i < NUM_DANCERS; i++){
    memcpy(myPeers[i].peer_addr, dancerMAC[i], 6);
    myPeers[i].channel = 0;  
    myPeers[i].encrypt = false;
    esp_now_add_peer(&myPeers[i]);
  }

}

void sendPattern(uint32_t colorTest){
  myMsg.pat = 'a';
  myMsg.hue = red(colorTest);
  myMsg.sat = green(colorTest); 
  myMsg.val = blue(colorTest) ;
  myMsg.state = true;

  esp_now_send(NULL, (uint8_t *) &myMsg, sizeof(msg_struct));
}

// callback when data is sent
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if(!(status == ESP_NOW_SEND_SUCCESS) && millis()<7000){logFail(mac_addr);}
}

uint16_t globalCol = 0;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  statusL.begin();
  delay(1000);
  Serial.println("started!");
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  delay(1000);

  if (!mpu.begin(104, &Wire1)) {
  Serial.println("Failed to find MPU6050 chip");
  while (1) {
    delay(10);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.println("");
  delay(100);
  
  esp_now_register_send_cb(onDataSent);

  pairWithDancers();

  delay(1000);
}
float move = 0;

void loop() {
  delay(10);
  // if(globalCol>50){sendPattern(0); statusL.setPixelColor(0,0);}
  // else{sendPattern(255); statusL.setPixelColor(0,255,255,255);}
  // if(globalCol == 100){globalCol = 1;}

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  move = (sqrt(pow(g.gyro.x,2) + pow(g.gyro.y,2) + pow(g.gyro.z,2)));
  if(move > 2){move-=2;}
  else{move=0;}
  if(move > 7){move = 7;}
  smoothOperator += move*(48);
  globalCol += smoothOperator();

  sendPattern(statusL.gamma32(statusL.ColorHSV(globalCol)));

  // Serial.print("SumSquareG:");
  // Serial.print(int(sqrt(pow(g.gyro.x,2) + pow(g.gyro.y,2) + pow(g.gyro.z,2))));
  // Serial.println("");
}
