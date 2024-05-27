#include <TaskScheduler.h>
#include <TaskSchedulerDeclarations.h>
#include <TaskSchedulerSleepMethods.h>

long timeToChange;
bool switchQ = false;

#include <Adafruit_NeoPixel.h>
#define highPin 3
#define lowPin 5
//gpio 4, 6, 7 on PCB
#define ledNumMid 10
#define ledNumEdge 9
#define ledPinTop 4
#define ledPinMid 6
#define ledPinBottom 7
Adafruit_NeoPixel stripT(ledNumEdge, ledPinTop, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripM(ledNumMid, ledPinMid, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripB(ledNumEdge, ledPinBottom, NEO_GRB + NEO_KHZ800);
char pattern;

#include <esp_now.h>
#include <WiFi.h>

int overflow(int value, int lowbound, int upbound){
  if(value >= upbound){return value - upbound + lowbound;}
  if(value < lowbound){return value + (upbound - lowbound);}
  return value;
}

//Structure example to receive data
//Must match the sender structure
typedef struct msg_struct {
  char pat;
  uint16_t hue;
  uint8_t sat;
  uint8_t val;
  bool state;
} msg_struct;

//Create a struct_message called myData
msg_struct myMsg;

Scheduler patternScheduler; // to control your personal task

Task myTask(100, -1);

uint16_t hueVal = 0;
uint8_t satVal = 255;
uint8_t valVal = 127;
uint16_t ledPos;
uint16_t ledPos2;
uint16_t ledPos3;
long bpm;
float vRatio;

int hueSix[] = {0,15*256,10923,21845,170*256,210*256};

int hueNine[] = {51044, 61839, 0, 2560, 10923, 21845, 32768, 48241, 54613};

int pack9[] = {stripT.ColorHSV(hueNine[0], 255, 128), stripT.ColorHSV(hueNine[1], 255, 128), stripT.ColorHSV(hueNine[2], 255, 128),
                  stripT.ColorHSV(hueNine[3], 255, 128), stripT.ColorHSV(hueNine[4], 255, 128), stripT.ColorHSV(hueNine[5], 255, 72),
                  stripT.ColorHSV(hueNine[6], 255, 96), stripT.ColorHSV(hueNine[7], 255, 72), stripT.ColorHSV(hueNine[8], 255, 72)};

int pack6[] = {stripT.ColorHSV(hueSix[0], 255, 128), stripT.ColorHSV(hueSix[1], 255, 128), stripT.ColorHSV(hueSix[2], 255, 128),
            stripT.ColorHSV(hueSix[3], 255, 71.5), stripT.ColorHSV(hueSix[4], 255, 76.5), stripT.ColorHSV(hueSix[5], 255, 72)};

#include "Synclet_Patterns.h"

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  disablePattern();
  
  memcpy(&myMsg, incomingData, sizeof(myMsg));

  if(myMsg.state == true){
    fillAll(stripT.Color(myMsg.hue, myMsg.sat, myMsg.val));
    break;
  }
  
  pattern = myMsg.pat;
  hueVal = myMsg.hue;
  satVal = myMsg.sat;
  valVal = myMsg.val;
  vRatio = float(valVal) / 256;

  
  if(pattern == 'X'){
    fillAll(0);
    showAll();
    delay(100);
    esp_deep_sleep_start();}
  
  Serial.println(myMsg.pat);
  Serial.println(myMsg.hue);
  Serial.println(myMsg.sat);
  Serial.println(myMsg.val);
  Serial.println(pattern);
  Serial.println(hueVal);
  Serial.println(satVal);
  Serial.println(valVal);
  enablePattern();
}

void setup() {

  pinMode(lowPin, OUTPUT); pinMode(highPin, OUTPUT);
  digitalWrite(lowPin, LOW); //digitalWrite(highPin, HIGH);
  
  Serial.begin(115200);
  
  stripT.begin(); stripM.begin(); stripB.begin();
  
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_register_recv_cb(OnDataRecv);

  patternScheduler.addTask(myTask);

  fillAll(stripT.Color(0,64,0));
  showAll();
  delay(500);
  fillAll(0);
  showAll();
}

void loop() {
  // it will run the user scheduler as well
  patternScheduler.execute();
}
