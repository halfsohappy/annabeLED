//pins for three momentary switches
#include "Adafruit_seesaw.h"
#include <Adafruit_NeoPixel.h>

#define SS_SWITCH_SELECT 1
#define SS_SWITCH_UP     2
#define SS_SWITCH_LEFT   3
#define SS_SWITCH_DOWN   4
#define SS_SWITCH_RIGHT  5

#define SEESAW_ADDR      0x49

Adafruit_seesaw pts_of_contact;

void button_watch();
void update_encoder();

Task task_update_encoder(150, -1, &update_encoder, &backlead_scheduler, true);

Task listen_button(150, -1, &button_watch, &backlead_scheduler, true);

int overflow(int value, int lowbound, int upbound){
  if(value >= upbound){return value - upbound + lowbound;}
  if(value < lowbound){return value + (upbound - lowbound);}
  return value;
}

void button_up_press() { //increases value of user_selection
  if(figure.user_selection <= NUM_FIGURES+NUM_STATE_SETTINGS){
    figure.user_selection++;
  }
  else{figure.user_selection = 0;}
}

void button_down_press(){ //
  if(figure.user_selection){
    figure.user_selection--;
  }
  else(figure.user_selection = NUM_FIGURES+NUM_STATE_SETTINGS);
}

void button_left_press() { //decreases index of what is being edited
 if(h_or_s_or_v){
  h_or_s_or_v -= 1;
  }
}

void button_right_press(){ //increases index of what is being edited
 if(h_or_s_or_v < 2){
  h_or_s_or_v += 1;
  }
}

void button_center_press() { //
  
}

int get_adjusted_delta() {
  int delt = ss.getEncoderDelta();
  delt += delt*2;
  return delt * -1;
}

void update_encoder(){
      report.user_hsv[h_or_s_or_v] = overflow(report.user_hsv[h_or_s_or_v] + get_adjusted_delta(), 0, 256);
}

bool lastSwitchStatus[] = {1,1,1,1,1};
bool switchPushed = false;
void button_watch() {//tracks button states, triggers functions when buttons pressed
  byte switchStatus;

  switchStatus = digitalRead(SS_SWITCH_UP);
  if (lastSwitchStatus[0] != switchStatus){
    lastSwitchStatus[0] = switchStatus;
    if (switchStatus == switchPushed){button_up_press();}
  }
  switchStatus = digitalRead(SS_SWITCH_DOWN);
  if (lastSwitchStatus[1] != switchStatus){
    lastSwitchStatus[1] = switchStatus;
    if (switchStatus == switchPushed){button_down_press();}
  }
    switchStatus = digitalRead(SS_SWITCH_LEFT);
  if (lastSwitchStatus[2] != switchStatus){
    lastSwitchStatus[2] = switchStatus;
    if (switchStatus == switchPushed){button_left_press();}
  }
    switchStatus = digitalRead(SS_SWITCH_RIGHT);
  if (lastSwitchStatus[3] != switchStatus){
    lastSwitchStatus[3] = switchStatus;
    if (switchStatus == switchPushed){button_right_press();}
  }
    switchStatus = digitalRead(SS_SWITCH_SELECT);
  if (lastSwitchStatus[4] != switchStatus){
    lastSwitchStatus[4] = switchStatus;
    if (switchStatus == switchPushed){button_center_press();}
  }
}

