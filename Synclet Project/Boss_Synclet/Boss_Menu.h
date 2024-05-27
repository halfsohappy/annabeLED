//pins for three momentary switches
#define buttonPinOne 0
#define SEESAW_ADDR          0x36

#define ppin0 10
#define ppin1 3
#define ppin2 1

Adafruit_NeoPixel strip(1, 36, NEO_GRB + NEO_KHZ800);

Adafruit_seesaw ss;
boolean coarse = true;

bool lastSwitchStatus[] = {HIGH, HIGH};
bool switchPushed = false;
bool para[] = {HIGH, HIGH, HIGH};

void buttonWatch();
void updateEnc();
void parameterCheck();

Scheduler userScheduler;
Task taskButtonWatch(250, -1, &buttonWatch);
Task taskUpdateEnc(150, -1, &updateEnc);
Task taskParameterCheck(300, -1, &parameterCheck);

int parameterSwitch(){
  para[0] = digitalRead(ppin0);
  para[1] = digitalRead(ppin1);
  para[2] = digitalRead(ppin2);

  if(!para[0] && para[1] && para[2]){return 3;}
  if(para[0] && !para[1] && para[2]){return 0;}
  if(para[0] && para[1] && !para[2]){return 1;}
  if(para[0] && para[1] && para[2]){return 2;}
}
void showBossNeopixel(){//update strip to bossColorVal
  strip.setPixelColor(0, strip.ColorHSV(256*hsv[0], 2*hsv[1], hsv[2])); //set hue from bossColorVal
  strip.show(); //update strip
}

void parameterCheck(){
  parameter = parameterSwitch();
  //showBossNeopixel();
  Serial.println(parameter);
}

int getDeltAdjust() {
  int delt = ss.getEncoderDelta();
  delt += delt*4*coarse;
  return delt * -1;
}

int overflow(int value, int lowbound, int upbound){
  if(value >= upbound){return value - upbound + lowbound;}
  if(value < lowbound){return value + (upbound - lowbound);}
  return value;
}

//what do buttons do?
void buttonOnePress() { //send command with current pattern + values
      Serial.println("buttonOne Press");
      
      sendSyncletCommand();
}

void buttonTwoPress(){ //nothing now
      coarse = !coarse;
      Serial.println("buttonTwo Press");

}

void updateEnc(){
    if(parameter == 0){ //hue, out of 256 values
      hsv[0] = overflow(hsv[0] + getDeltAdjust(), 0, 256);
      tellColor(hsv[0], hsv[1], hsv[2]);
      showBossNeopixel();
    }
    if(parameter == 1){ //sat, out of 128 values
      hsv[1] = overflow(hsv[1] + getDeltAdjust(), 0, 128);
      tellColor(hsv[0], hsv[1], hsv[2]);
      showBossNeopixel();
    }
    if(parameter == 2){ //val, out of 128 values
      hsv[2] = overflow(hsv[2] + getDeltAdjust(), 0, 128);
      tellColor(hsv[0], hsv[1], hsv[2]);
      showBossNeopixel();
    }
    if(parameter == 3){ //pattern 
      patP = overflow(patP - ss.getEncoderDelta(), 0, numPatterns);
      tellPattern(patterns[patP]);
    }
   
}

void buttonWatch() {//tracks button states, triggers functions when buttons pressed
  byte switchStatus;
  //1st switch
  switchStatus = digitalRead(buttonPinOne);
  if (lastSwitchStatus[0] != switchStatus)//was there a change in switch state?
  {
    lastSwitchStatus[0] = switchStatus;//update to the new state
    if (switchStatus == switchPushed)//did the switch get pushed?
    {
      buttonOnePress();
    }
  }
  switchStatus = ss.digitalRead(24);
  if (lastSwitchStatus[1] != switchStatus)//was there a change in switch state?
  {
    lastSwitchStatus[1] = switchStatus;//update to the new state
    if (switchStatus == switchPushed)//did the switch get pushed?
    {
      buttonTwoPress();
    }
  }
}
