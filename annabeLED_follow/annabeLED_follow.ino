#include <TaskScheduler.h>
#include <TaskSchedulerDeclarations.h>
#include <TaskSchedulerSleepMethods.h>
#include <Adafruit_NeoPixel.h>

#define ledNum 10
#define ledPin 10
Adafruit_NeoPixel strip(ledNum, ledPin, NEO_GRB + NEO_KHZ800);

#include <esp_now.h>
#include <WiFi.h>

typedef struct Figure {
  bool use_state;
  uint32_t state;
  uint16_t hue;
  uint8_t sat;
  uint8_t val;
} Figure;

Figure figure;

Scheduler figure_scheduler;

Task dance(100, -1);

uint16_t led_pos[] = {0,0};

float vRatio;

#include "Pattern_Util.h"
#include "Follow_Patterns.h"

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  end_figure();
  memcpy(&figure, incomingData, sizeof(Figure));

  if(figure.use_state == true){
    strip.fill(figure.state);
    strip.show();
    return;
  }
  else{
    vRatio = float(figure.val) / 256;
    led_pos[0] = 0;
    // Serial.println(activeFigure.state);
    // Serial.println(activeFigure.hue);
    // Serial.println(activeFigure.sat);
    // Serial.println(activeFigure.val);
    start_figure(figure.state);
  }
}

void setup() {
  
  Serial.begin(115200);
  
  strip.begin();
  
  WiFi.mode(WIFI_STA);

  strip.fill(strip.Color(0,64,0));
  strip.show();
  delay(500);
  strip.fill(0);
  strip.show();

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  Serial.println("Started!");
  esp_now_register_recv_cb(OnDataRecv);

  figure_scheduler.addTask(dance);
}

void loop() {
  figure_scheduler.execute();
}
