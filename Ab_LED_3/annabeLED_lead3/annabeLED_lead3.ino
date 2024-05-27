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
#include <esp_now.h>
#include <WiFi.h>

Adafruit_NeoPixel strip(1, 36, NEO_GRB + NEO_KHZ800);
#define NUM_DANCERS 8

// char figNames[12][8] = {
//  {'S', 'T', 'A', 'T', 'R', 'A', 'I', 'N'},
//  {'C', 'Y', 'C', 'L', 'S', 'I', 'M', 'P'},
//  {'C', 'Y', 'C', 'L', 'F', 'L', 'O', 'W'},
//  {'F', 'A', 'D', 'E', 'S', 'I', 'M', 'P'},
//  {'F', 'A', 'D', 'E', 'F', 'L', 'O', 'W'},
//  {'F', 'A', 'D', 'E', 'S', 'T', 'E', 'P'},
//  {'R', 'A', 'N', 'D', 'W', 'H', 'T', 'E'},
//  {'R', 'A', 'N', 'D', 'C', 'O', 'L', 'R'},
//  {'C', 'H', 'S', 'E', '1', 'A', 'R', 'O'},
//  {'C', 'H', 'S', 'E', 'R', 'D', 'A', 'G'},
//  {'C', 'H', 'S', 'E', 'R', 'A', 'R', 'O'},
//  {'D', 'A', 'R', 'K', 'N', 'E', 'S', 'S'}
// };

typedef struct Figure {
 bool use_state;
 uint32_t state;
 uint16_t hue;
 uint8_t sat;
 uint8_t val;
} Figure;

Figure figure;

Scheduler lead_scheduler;

#include "Lead_Pairing.h"
#include "Lead_Backlead.h"
#include "Lead_States.h"

void send_figure(){
     esp_now_send(NULL, (uint8_t *) &figure, sizeof(Figure));
}

Task broadcast(10, -1);

Task get_nicla(30, -1);


void setup() {
 Serial.begin(115200);
 WiFi.mode(WIFI_STA);

 if (esp_now_init() != ESP_OK) {
   Serial.println("Error initializing ESP-NOW");
   return;
 }

 esp_now_register_send_cb(onDataSent);
 pairWithDancers();
 esp_now_register_recv_cb(OnDataRecv);

lead_scheduler.addTask(broadcast);

}


void loop() {
 lead_scheduler.execute();
}
