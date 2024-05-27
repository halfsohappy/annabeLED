#include <TaskScheduler.h>
#include <TaskSchedulerDeclarations.h>
#include <TaskSchedulerSleepMethods.h>

long timeToChange;
bool switchQ = false;

#include <Adafruit_NeoPixel.h>
#define highPin 3
#define lowPin 5

#define ledNum 28
#define ledPin 10//10 for normal, 4 for tie
Adafruit_NeoPixel strip(ledNum, ledPin, NEO_GRB + NEO_KHZ800);
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
  long msec;
} msg_struct;

//Create a struct_message called myData
msg_struct myMsg;

Scheduler userScheduler; // to control your personal task

void FadeStatic();
Task taskFadeStatic(25, -1, &FadeStatic);

void FadeFlow();
Task taskFadeFlow(100, -1, &FadeFlow);

void FadeColorStep();
Task taskFadeColorStep(25, -1, &FadeColorStep);

void FadeColorFlow();
Task taskFadeColorFlow(25, -1, &FadeColorFlow);

void CycleStay();
Task taskCycleStay(100, -1, &CycleStay);

void CycleMove();
Task taskCycleMove(100, -1, &CycleMove);

void ChaseSingle();
Task taskChaseSingle(200, -1, &ChaseSingle);

void ChaseColumn();
Task taskChaseColumn(200, -1, &ChaseColumn);

void ChaseRain();
Task taskChaseRain(500, -1, &ChaseRain);

void RainbowRoad();
Task taskRainbowRoad(250, -1, &RainbowRoad);

uint16_t hueVal = 0;
uint8_t satVal = 255;
uint8_t valVal = 127;
uint16_t ledPos;
uint16_t ledPos2;
uint16_t ledPos3;
long bpm;
float vRatio;

int p9[] = {strip.ColorHSV(51044, 255, 128), strip.ColorHSV(61839, 255, 128), strip.ColorHSV(0, 255, 128),
                  strip.ColorHSV(2560, 255, 128), strip.ColorHSV(10923, 255, 128), strip.ColorHSV(21845, 255, 72),
                  strip.ColorHSV(32768, 255, 96), strip.ColorHSV(48241, 255, 72), strip.ColorHSV(54613, 255, 72)};

int p6[] = {strip.ColorHSV(0, 255, 128), strip.ColorHSV(2560, 255, 128), strip.ColorHSV(10923, 255, 128), strip.ColorHSV(21845, 255, 71.5),
            strip.ColorHSV(48241, 255, 76.5), strip.ColorHSV(54613, 255, 72)};
            
int hueSix[] = {0,15*256,10923,21845,170*256,210*256};

int hueNine[] = {51044, 61839, 0, 2560, 10923, 21845, 32768, 48241, 54613};


#include "Synclet_User_Palette.h"
#include "Synclet_Patterns.h"

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  disablePattern();
  
  memcpy(&myMsg, incomingData, sizeof(myMsg));
  
  pattern = myMsg.pat;
  hueVal = myMsg.hue * 256;
  satVal = myMsg.sat * 2;
  valVal = myMsg.val * 2;
  vRatio = float(valVal) / 256;
  bpm = myMsg.msec;
  
  timeToChange = millis() + 1000;
  if(pattern == 'X'){
    strip.clear();
    strip.show();
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
  
  strip.begin();
  
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_register_recv_cb(OnDataRecv);

  userScheduler.addTask( taskFadeStatic );
  userScheduler.addTask( taskFadeFlow );
  userScheduler.addTask( taskFadeColorStep );
  userScheduler.addTask( taskFadeColorFlow );
  userScheduler.addTask( taskCycleStay );
  userScheduler.addTask( taskCycleMove );
  userScheduler.addTask( taskChaseSingle );
  userScheduler.addTask( taskChaseColumn );
  userScheduler.addTask( taskChaseRain );
  userScheduler.addTask( taskRainbowRoad );

  strip.fill(strip.Color(0,64,0));
  strip.show();
  delay(500);
  strip.clear();
  strip.show();
}

void loop() {
  // it will run the user scheduler as well
  userScheduler.execute();
}
