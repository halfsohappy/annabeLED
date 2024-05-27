
#include <TaskScheduler.h>
#include <TaskSchedulerDeclarations.h>
#include <TaskSchedulerSleepMethods.h>
#include "Adafruit_seesaw.h"
#include <seesaw_neopixel.h>
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel strip(1, 36, NEO_GRB + NEO_KHZ800);
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define STATIC_CLONE 'a'
#define STATIC_RAINBOW 'b'

#define CYCLE_MOVE 'c'
#define CYCLE_RAINBOW 'd'

#define FADE_SIMPLE 'e'
#define FADE_SIMPLE_FLOW 'f'
#define FADE_RAINBOW_STEP 'g'
#define FADE_COLOR_FLOW 'h'

#define CHASE_SINGLE 'i'
#define CHASE_ARROW 'j'
#define CHASE_RAINBOW 'k'
#define RAINBOW_ROAD 'l'

#define DARKNESS 'm'
#define DEEP_SLEEP 'n'

char patterns[] = {STATIC_CLONE, STATIC_RAINBOW, CYCLE_MOVE, CYCLE_RAINBOW,
FADE_SIMPLE, FADE_SIMPLE_FLOW, FADE_RAINBOW_STEP, FADE_COLOR_FLOW, 
CHASE_SINGLE, CHASE_ARROW, CHASE_RAINBOW, RAINBOW_ROAD, DARKNESS, DEEP_SLEEP};

int patP = 0;
int numPatterns = 14;//IF ADDING PATTERNS, CHANGE numPatterns!!!!!

int hsv[] = {0,127,72};
int parameter;

#include "Boss_List.h"
#include "Boss_States.h"

//messages contain a character that reoresents pattern selection, and an integer(s) to modify the pattern
void sendPattern(){
  myMsg.pat = patterns[patP];
  myMsg.hue = hsv[0]*256;
  myMsg.sat = hsv[1]*2; 
  myMsg.val = hsv[2]*2;
    for(int i = 0; i< NUM_SYNCLETS; i++){
      esp_now_send(myPeers[i].peer_addr, (uint8_t *) &myMsg, sizeof(msg_struct));
    }
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

  pairWithSynclets();

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
