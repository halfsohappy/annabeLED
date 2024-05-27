#include <TaskScheduler.h>
#include <TaskSchedulerDeclarations.h>
#include <TaskSchedulerSleepMethods.h>
#include <FastLED.h>
#include <esp_now.h>
#include <WiFi.h>

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

callie_message new_report;


Scheduler figure_scheduler;

Task fig_task(100, -1);

#include "Synclet_Patterns.h"


//blend()
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  // old_report.color0 = callie_report.color0;
  // old_report.color1 = callie_report.color1;
  // old_report.figure = callie_report.figure;
  // old_report.speed = callie_report.speed;

  memcpy(&new_report, incomingData, sizeof(new_report));

  // if(new_report.color0 == callie_report.color0 &&
  //   new_report.color1 == callie_report.color1 &&
  //   new_report.figure == callie_report.figure &&
  //   new_report.speed == callie_report.speed){return;}
  Serial.println("msg got!");
  //delay(10);
  disablePattern();
  Serial.println(int(callie_report.color0.hue));
  //delay(10);
  memcpy(&callie_report, incomingData, sizeof(callie_message));
  Serial.print(int(callie_report.color0.hue));
  callie_report.color0.sat = 255;
  callie_report.color1.sat = 255;

  enablePattern();
}

void setup() {

  FastLED.addLeds<WS2812B, data_left_strap, GRB>(left_strap, led_per_strap);
  FastLED.addLeds<WS2812B, data_right_strap, GRB>(right_strap, led_per_strap);  

  Serial.begin(115200);
  
  callie_report.color0.hue = 64;
  callie_report.color0.sat = 255;
  callie_report.color0.val = 160;
  callie_report.speed = 0;
  
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  //esp_now_register_recv_cb(OnDataRecv);

  figure_scheduler.addTask(fig_task);
  

  // fill_solid(left_strap, 108, callie_report.color0);
  // fill_solid(right_strap, 108, callie_report.color0);

}
int hh = 0;
void loop() {
  hh++;
  callie_report.color0.hue = hh;
   fill_solid(left_strap, 108, callie_report.color0);
   fill_solid(right_strap, 108, callie_report.color0);
  //figure_scheduler.execute();
  //FastLED.setBrightness(100);
  FastLED.show();
}
