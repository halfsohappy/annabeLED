
#include <TaskScheduler.h>
#include <TaskSchedulerDeclarations.h>
#include <TaskSchedulerSleepMethods.h>
#include "Adafruit_seesaw.h"
#include <seesaw_neopixel.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>



#include "Boss_List.h"

char patterns[] = {'s', 'f', 't', 'l', 'r', 'a', 'y', 'c', 'u', 'w', 'R', 'o', 'X'}; 
int patP = 0;
int numPatterns = 13;//IF ADDING PATTERNS, CHANGE numPatterns!!!!!

int hsv[] = {0,127,72};
int parameter;
long bpm;

//messages contain a character that reoresents pattern selection, and an integer(s) to modify the pattern
void sendSyncletCommand(){
  myMsg.pat = patterns[patP];
  myMsg.hue = hsv[0];
  myMsg.sat = hsv[1]; 
  myMsg.val = hsv[2];
  myMsg.msec = bpm;
  esp_err_t rA = esp_now_send(baA, (uint8_t *) &myMsg, sizeof(msg_struct));
  if (rA == ESP_OK) {Serial.println("sent A");}
  esp_err_t rB = esp_now_send(baB, (uint8_t *) &myMsg, sizeof(msg_struct));
  if (rB == ESP_OK) {Serial.println("sent B");}
  esp_err_t rC = esp_now_send(baC, (uint8_t *) &myMsg, sizeof(msg_struct));
  if (rC == ESP_OK) {Serial.println("sent C");}
  esp_err_t rD = esp_now_send(baD, (uint8_t *) &myMsg, sizeof(msg_struct));
  if (rD == ESP_OK) {Serial.println("sent D");}
  esp_err_t rE = esp_now_send(baE, (uint8_t *) &myMsg, sizeof(msg_struct));
  if (rE == ESP_OK) {Serial.println("sent E");}
  esp_err_t rF = esp_now_send(baF, (uint8_t *) &myMsg, sizeof(msg_struct));
  if (rF == ESP_OK) {Serial.println("sent F");}
  esp_err_t rG = esp_now_send(baG, (uint8_t *) &myMsg, sizeof(msg_struct));
  if (rG == ESP_OK) {Serial.println("sent G");}
  esp_err_t rH = esp_now_send(baH, (uint8_t *) &myMsg, sizeof(msg_struct));
  if (rH == ESP_OK) {Serial.println("sent H");}
}

#include "Boss_Screen.h"
#include "Boss_Menu.h"


void setup() {
 
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_register_send_cb(OnDataSent);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
    memcpy(peerInfo.peer_addr, baA, 6);
        if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }
    memcpy(peerInfo.peer_addr, baB, 6);
        if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }
    memcpy(peerInfo.peer_addr, baC, 6);
        if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }
    memcpy(peerInfo.peer_addr, baD, 6);
        if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }
    memcpy(peerInfo.peer_addr, baE, 6);
        if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }
    memcpy(peerInfo.peer_addr, baF, 6);
        if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }
    memcpy(peerInfo.peer_addr, baG, 6);
        if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }
    memcpy(peerInfo.peer_addr, baH, 6);
        if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }
    

  delay(100);
  pinMode(buttonPinOne, OUTPUT);
  pinMode(buttonPinOne, INPUT_PULLUP);
  //pinMode(buttonPinTwo, INPUT_PULLUP);
  
  pinMode(5, OUTPUT); pinMode(35, OUTPUT);
  digitalWrite(5, HIGH); digitalWrite(35, LOW);

  pinMode(17, OUTPUT); pinMode(7, OUTPUT);
  digitalWrite(17, LOW); digitalWrite(7, LOW);
  Serial.println("try start seesaw");
  ss.begin(0x36);
  strip.begin();
  Serial.println("seesaw started");

  strip.show();
   
  // use a pin for the built in encoder switch
  ss.pinMode(24, INPUT_PULLUP);
  
  pinMode(ppin0, INPUT_PULLUP);
  pinMode(ppin1, INPUT_PULLUP);
  pinMode(ppin2, INPUT_PULLUP);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setRotation(2);
  //display.display();
  delay(500);
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setFont();
  
  userScheduler.addTask( taskButtonWatch );
  userScheduler.addTask( taskUpdateEnc );
  userScheduler.addTask( taskParameterCheck );
  taskButtonWatch.enable();
  taskUpdateEnc.enable();
  taskParameterCheck.enable();
}

void loop() {
  userScheduler.execute();
}
