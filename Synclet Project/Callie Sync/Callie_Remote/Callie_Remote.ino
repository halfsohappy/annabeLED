#include <FastLED.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <esp_now.h>
#include <WiFi.h>


int overflow(int value, int lowbound, int upbound){
  if(value >= upbound){return value - upbound + lowbound;}
  if(value < lowbound){return value + (upbound - lowbound);}
  return value;
}

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#include "Adafruit_seesaw.h"
#include "Menu_Classes.h"


#define SS_SWITCH_SELECT 1
#define SS_SWITCH_UP     2
#define SS_SWITCH_LEFT   3
#define SS_SWITCH_DOWN   4
#define SS_SWITCH_RIGHT  5
#define SEESAW_ADDR      0x49

Adafruit_seesaw ss;
int32_t encoder_position;

typedef struct callie_message {
  uint8_t figure;
  CHSV color0;
  CHSV color1;
  uint8_t speed;
} callie_message;

uint8_t friendly[] = {0xA0, 0x76, 0x4E, 0x4A, 0x75, 0x8C};

callie_message callie_report;

int tit_len[] = {12,15,15,7,9,5,7,5};

bool statone = false;
bool lastSwitchStatus[5];

uint8_t lim_arr[][7]= {
  {255,127,127,1,255,127,127},
  {8},
  {255,127,127,255,127,127,100},
  {108,255,100,127},
  {255,127,127,100},
  {255,127,127,100},
  {255,127,127,100,25},
  {0,0,0}
};

const char *menu_lines[][8] = {
    {"Static Color","Hue (L):","Saturation (L):","Brightness (L):","Match Sides? ","Hue (L):","Saturation (L):","Brightness (L):"},

    {"Preset Gradient","Preset:"},

    {"Custom Gradient","Hue1:","Sat1:","Val1:","Hue2:","Sat2:","Val2:","Speed:"},

    {"Rainbow","Length:","Start:","","Speed:","Brightness:",""},

    {"Breathing","Hue:","","Saturation:","","Brightness:","","Speed:"},

    {"Pulse","Hue:","","Saturation:","","Brightness:","","Speed:"},

    {"Sparkle","Hue:","Saturation:","Brightness:","","Frequency:","Coverage:"},

    {"Zones","Palette:","","Function:","","Speed:",""}
    };



uint8_t pattern_vals[8][7];

void create_message(int pn){
  switch (pn) {
  case 0:
  callie_report.figure = 0;
  callie_report.color0.setHSV(pattern_vals[2][0], pattern_vals[2][1], pattern_vals[2][2]);
  callie_report.color1.setHSV(pattern_vals[2][3], pattern_vals[2][4], pattern_vals[2][5]);
  callie_report.speed = pattern_vals[2][6];
  break;

  case 1:
    callie_report.figure = 1;
  callie_report.color0.hue = pattern_vals[1][0];
  break;

  case 2:
  callie_report.figure = 2;
  callie_report.color0.setHSV(pattern_vals[2][0], pattern_vals[2][1], pattern_vals[2][2]);
  callie_report.color1.setHSV(pattern_vals[2][3], pattern_vals[2][4], pattern_vals[2][5]);
  callie_report.speed = pattern_vals[2][6];
  break;

  case 3:
  callie_report.figure = 3;
  callie_report.color0.hue = pattern_vals[3][0];
  callie_report.color0.sat = pattern_vals[3][1];
  callie_report.speed = pattern_vals[3][2];
  break;

  case 4:
  callie_report.figure = 4;
  callie_report.color0.setHSV(pattern_vals[4][0], pattern_vals[4][1], pattern_vals[4][2]);
  callie_report.speed = pattern_vals[4][3];
  break;

  case 5:
  callie_report.figure = 5;
  callie_report.color0.setHSV(pattern_vals[5][0], pattern_vals[5][1], pattern_vals[5][2]);
  callie_report.speed = pattern_vals[5][3];
  break;

  case 6:
  callie_report.figure = 6;
  callie_report.color0.setHSV(pattern_vals[6][0], pattern_vals[6][1], pattern_vals[6][2]);
  callie_report.color1.setHSV(pattern_vals[6][3], pattern_vals[6][4], 0);
  break;

  case 7:
  callie_report.figure = 7;
  callie_report.color0.hue = pattern_vals[7][0];
  callie_report.color0.sat = pattern_vals[7][1];
  callie_report.speed = pattern_vals[7][2];
  break;
  }
}

int li_t[][7] = {{1,2,3,4,5,6,7},{1},{1,2,3,4,5,6,7},{1,2,4,5},{1,3,5,7},{1,3,5,7},{1,2,3,5,6},{1,3,5}};

int lnum[] = {7,1,7,4,4,4,5,3};

int line_trans(int pn, int ln){
  return li_t[pn][ln-1];
}


bool inv = false;
int page_num = 0;
int line_num = 1;
void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  ss.begin(SEESAW_ADDR);
  ss.pinMode(SS_SWITCH_UP, INPUT_PULLUP);
  ss.pinMode(SS_SWITCH_DOWN, INPUT_PULLUP);
  ss.pinMode(SS_SWITCH_LEFT, INPUT_PULLUP);
  ss.pinMode(SS_SWITCH_RIGHT, INPUT_PULLUP);
  ss.pinMode(SS_SWITCH_SELECT, INPUT_PULLUP);
  encoder_position = ss.getEncoderPosition();

  for(int i = 0; i<8; i++){
    for(int j=0; j<7; j++){
      pattern_vals[i][j] = lim_arr[i][j];
    }
  }

#include "setup_menu.h"
esp_now_peer_info_t peerInfo;

WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    //Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  //esp_now_register_send_cb(OnDataSent);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
    memcpy(peerInfo.peer_addr, friendly, 6);
        if (esp_now_add_peer(&peerInfo) != ESP_OK){
        //Serial.println("Failed to add peer");
        return;
    }
Serial.begin(115200);
}
int track = 0;
void loop() {
  // did we move around?

  // don't overwhelm serial port
  delay(10);

  display.clearDisplay();
  display.invertDisplay(inv);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.setCursor(128-6*tit_len[page_num],0);
  display.println(menu_lines[page_num][0]);

  for(int i =1; i<8; i++){
    display.print(menu_lines[page_num][i]);
    if(i == li_t[page_num][track] && line_trans(page_num, line_num)!=i){//this line has content but isnt selected

      display.print(pattern_vals[page_num][track]); track++;
    }
    if(line_trans(page_num, line_num)==i){//our cursor is on this line
      pattern_vals[page_num][track] = overflow(pattern_vals[page_num][track] + ss.getEncoderDelta(), 0, lim_arr[page_num][track]+1);//let our scroll wheel adjust the value
      display.print(pattern_vals[page_num][track]);//print the value
      track++;//increase "number of content lines printed"
      display.write(0x11);//print cursor
      }

    display.println();
  }



  track = 0;
  create_message(page_num);

  delay(50);
  display.display();      // Show initial text

  bool switchStatus;
  //1st switch
  switchStatus = ss.digitalRead(SS_SWITCH_UP);
  if (lastSwitchStatus[0] != switchStatus)//was there a change in switch state?
  {
    lastSwitchStatus[0] = switchStatus;//update to the new state
    if (switchStatus == false){
      page_num +=1;
      if(page_num>7){page_num = 0;}
    }
  }
  switchStatus = ss.digitalRead(SS_SWITCH_DOWN);
  if (lastSwitchStatus[1] != switchStatus)//was there a change in switch state?
  {
    lastSwitchStatus[1] = switchStatus;//update to the new state
    if (switchStatus == false){
      page_num -=1;
      if(page_num<0){page_num = 7;}
    }
  }
  switchStatus = ss.digitalRead(SS_SWITCH_RIGHT);
  if (lastSwitchStatus[2] != switchStatus)//was there a change in switch state?
  {
    lastSwitchStatus[2] = switchStatus;//update to the new state
    if (switchStatus == false){
      line_num +=1;
      if(line_num> lnum[page_num]){line_num = 1;}
    }
  }
  if(line_num> lnum[page_num]){line_num = 1;}
  switchStatus = ss.digitalRead(SS_SWITCH_LEFT);
  if (lastSwitchStatus[3] != switchStatus)//was there a change in switch state?
  {
    lastSwitchStatus[3] = switchStatus;//update to the new state
    if (switchStatus == false){
      if(line_num != 1){line_num -=1;}
      //if(line_num< lnum[page_num]){line_num = 7;}
    }
  }
  switchStatus = ss.digitalRead(SS_SWITCH_SELECT);
  if (lastSwitchStatus[4] != switchStatus)//was there a change in switch state?
  {
    lastSwitchStatus[4] = switchStatus;//update to the new state
    if (switchStatus == false){
    inv = !inv;
    }
  }

  if(inv == true){
    esp_err_t rA = esp_now_send(friendly, (uint8_t *) &callie_report, sizeof(callie_message));
    if (rA == ESP_OK) {Serial.println("sent A");}
    delay(100);
  }



  // if (! ss.digitalRead(SS_SWITCH_UP)) {
  //   page_num +=1;
  //   if(page_num>7){page_num = 0;}
  // }
  // if (! ss.digitalRead(SS_SWITCH_DOWN)) {
  //   page_num -=1;
  //   if(page_num<0){page_num = 7;}
  // }
  // if (! ss.digitalRead(SS_SWITCH_RIGHT)) {
  //   line_num +=1;
  //   if(line_num>7){line_num = 1;}
  // }
  // if (! ss.digitalRead(SS_SWITCH_LEFT)) {
  //   line_num -=1;
  //   if(line_num<1){line_num = 7;}
  // }
  // if (! ss.digitalRead(SS_SWITCH_SELECT)) {
  //   inv = !inv;
  // }

}

