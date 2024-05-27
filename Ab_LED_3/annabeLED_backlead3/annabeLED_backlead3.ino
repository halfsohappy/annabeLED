#include <TaskScheduler.h>
#include <TaskSchedulerDeclarations.h>
#include <TaskSchedulerSleepMethods.h>
#include <Adafruit_NeoPixel.h>
#include <esp_now.h>
#include <WiFi.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Smooth.h>
#define LSM9DS1_SCK A5
#define LSM9DS1_MISO 12
#define LSM9DS1_MOSI A4
#define LSM9DS1_XGCS 6
#define LSM9DS1_MCS 5

#define NUM_FIGURES 12

#define NUM_STATE_SETTINGS 2

Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();
Scheduler backlead_scheduler;

Smooth smooth_operator(15);
double latest_reading = 0;

uint8_t leadMAC[6] = {0x34, 0x85, 0x18, 0x05, 0xC9, 0xF0}; //need to put actual mac here
esp_now_peer_info_t lead_address;

typedef struct Report_to_lead{
  uint16_t acc_value;
  uint8_t user_hsv[3];
  bool display_flag;
  uint8_t user_selection;
} Report_to_lead;

Report_to_lead report;
uint8_t h_or_s_or_v = 0;


void pair_with_lead(){
  memcpy(lead_address.peer_addr, leadMAC, 6);
      lead_address.channel = 0;  
      lead_address.encrypt = false;
  esp_now_add_peer(&lead_address);
}

// callback when data is sent
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
}

void setupLSM()
{
  // 1.) Set the accelerometer range
  lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G);
  //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_4G);//lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_8G);//lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_16G);
  
  // 2.) Set the magnetometer sensitivity
  lsm.setupMag(lsm.LSM9DS1_MAGGAIN_4GAUSS);
  //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_8GAUSS);//lsm.setupMag(lsm.LSM9DS1_MAGGAIN_12GAUSS);//lsm.setupMag(lsm.LSM9DS1_MAGGAIN_16GAUSS);

  // 3.) Setup the gyroscope
  lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_245DPS);
  //lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_500DPS);//lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_2000DPS);
}

void read__total_kinetic(){
  lsm.read();
  sensors_event_t a, m, g, temp;
  lsm.getEvent(&a, &m, &g, &temp);
  double reading = sqrt(pow((g.gyro.x),2) + pow((g.gyro.y),2) + pow((g.gyro.z),2));
  latest_reading = reading;

  if(reading > 2){reading-=2;}
  else{reading=0;}
  if(reading > 7){reading = 7;}
  smooth_operator += reading*(48);
  report.acc_value = smooth_operator();

  send_report();
}

Task kinetic_task(100, -1, &read_total_kinetic, &backlead_scheduler, true);

void send_report(){
  esp_now_send(lead_address.peer_addr, (uint8_t *) &report, sizeof(Report_to_lead));
}

void setup(void) {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  report.user_hsv[0] = 0;
  report.user_hsv[1] = 0;
  report.user_hsv[2] = 0;
  report.update_all_flag = 0;
  report.display_flag = 0;

  pts_of_contact.pinMode(SS_SWITCH_UP, INPUT_PULLUP);
  pts_of_contact.pinMode(SS_SWITCH_DOWN, INPUT_PULLUP);
  pts_of_contact.pinMode(SS_SWITCH_LEFT, INPUT_PULLUP);
  pts_of_contact.pinMode(SS_SWITCH_RIGHT, INPUT_PULLUP);
  pts_of_contact.pinMode(SS_SWITCH_SELECT, INPUT_PULLUP);

  if (!lsm.begin()) {
    Serial.println("Failed to find lsm9ds1 chip");
    while (1) {
      delay(10);
    }
  }
  setupLSM();
}

void loop() {
  backlead_scheduler.execute();
}