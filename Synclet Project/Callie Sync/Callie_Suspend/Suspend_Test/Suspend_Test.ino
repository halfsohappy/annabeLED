#include <TaskScheduler.h>
#include <TaskSchedulerDeclarations.h>
#include <TaskSchedulerSleepMethods.h>
#include <FastLED.h>
#include <esp_now.h>
#include <WiFi.h>
#include "colorpalettes.h"
#include <cstring>
#include "colorutils.h"
#include "Annie_Palettes.h"
#define led_per_strap 108


#define data_left_strap 1
#define data_right_strap 2


CRGB left_strap[led_per_strap];
CRGB right_strap[led_per_strap]; 



typedef struct callie_message {
  uint8_t figure;
  CHSV color0;
  CHSV color1;
  uint8_t speed;
} callie_message;

callie_message callie_report;

//#include "Synclet_Patterns.h"

void setup() {

  FastLED.addLeds<WS2812B, data_left_strap, GRB>(left_strap, led_per_strap);
  FastLED.addLeds<WS2812B, data_right_strap, GRB>(right_strap, led_per_strap);  

  Serial.begin(115200);
  
  callie_report.color0.hue = 1;
  callie_report.color0.sat = 255;
  callie_report.color0.val = 160;
  callie_report.speed = 1;
  
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
}

void stripeify(CRGB *line){
  CRGB* temp = (CRGB*) malloc(sizeof(CRGB)*108);
  memcpy(temp, line, sizeof(CRGB)*108);
  for(int i=0; i<54; i++){
    line[53-i] = temp[(2*i)+1];
  }
  for(int i=1; i<55; i++){
    line[53+i] = temp[2*i];
  }
  free(temp);
}


CRGBPalette256 choose_pal(int pal){
  switch(pal){
   case 0: return cherry_bomb;
   case 1: return blue_candy;
   case 2: return electric_punk;
   case 3: return sweet_kiss;
   case 4: return ocean_breeze;
   case 5: return alarm_;
   case 6: return deep_space;
   case 7: return ice;
   case 8: return bisexual_lighting;
   case 9: return lesbian;
   }
}
uint16_t position;



// int bounce(){
  //   if(b == 255){
  //     decrease = true;
  //     //rev = true;
  //   }
  //   if(b == 0){
  //     decrease = false;
  //     rev = ~rev;
  //   }
  //   if (decrease){return b--;}
  //   else {return b++;}
  // }


void pre_gradient(){
  //if(position == 255){rev = ~rev;}
  //CRGBPalette16 chosen_pal = choose_pal(callie_report.color0.hue);
  fill_palette_circular(left_strap, 108, position, (choose_pal(9)), callie_report.color0.val, LINEARBLEND, rev);
  fill_palette_circular(right_strap, 108, position, (choose_pal(9)), callie_report.color0.val, LINEARBLEND, rev);
  if(callie_report.speed){position++;}
  stripeify(left_strap);
  stripeify(right_strap);
}

void rainbow(){
  fill_rainbow(left_strap, 108, 8*position, 2);
  fill_rainbow(right_strap, 108, 8*position, 2);
  position++;
  stripeify(left_strap);
  stripeify(right_strap);
  FastLED.setBrightness(callie_report.color0.val);
  //ADD BRIGHTNESS IDIOT (i did :)
}

void loop() {
  //pre_gradient();
  //rainbow();
  //fill_rainbow(left_strap, 108, 0, 4);
  //fill_rainbow(right_strap, 108, 0, 4);
  //bounce();
  FastLED.setBrightness(60);
  FastLED.show();
  FastLED.delay(40);
  Serial.println("loop");
}


