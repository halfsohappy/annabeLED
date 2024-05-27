//pins for three momentary switches
#define buttonPinOne 18
#define SEESAW_ADDR          0x36

#define ppin0 1
#define ppin1 3
#define ppin2 7

Adafruit_NeoPixel strip(1, 40, NEO_GRB + NEO_KHZ800);

Adafruit_seesaw ss;
seesaw_NeoPixel sspixel = seesaw_NeoPixel(1, 15, NEO_GRB + NEO_KHZ800);
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
Task taskParameterCheck(400, -1, &parameterCheck);

void setupEnc() {
  pinMode(11, OUTPUT);
  digitalWrite(11, LOW);
  Serial.println("try start seesaw");
  ss.begin(0x36);
  sspixel.begin(0x36);
  Serial.println("seesaw started");

  sspixel.setBrightness(20);
  sspixel.show();
   
  // use a pin for the built in encoder switch
  ss.pinMode(24, INPUT_PULLUP);
}

void setupPara(){
  pinMode(ppin0, INPUT_PULLUP);
  pinMode(ppin1, INPUT_PULLUP);
  pinMode(ppin2, INPUT_PULLUP);
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
}

int parameterSwitch(){
  para[0] = digitalRead(ppin0);
  para[1] = digitalRead(ppin1);
  para[2] = digitalRead(ppin2);

  if(!para[0] && para[1] && para[2]){return 2;}
  if(para[0] && !para[1] && para[2]){return 1;}
  if(para[0] && para[1] && !para[2]){return 0;}
  if(para[0] && para[1] && para[2]){return 3;}
}

void parameterCheck(){
  parameter = parameterSwitch();
  //Serial.println(parameter);
}

int getDeltAdjust() {
  int delt = ss.getEncoderDelta();
  delt += delt*4*coarse;
  return delt * -1;
}

void showBossNeopixel(){//update strip to bossColorVal
  sspixel.setPixelColor(0, strip.ColorHSV(256*hsv[0], 2*hsv[1], 2*hsv[2])); //set hue from bossColorVal
  sspixel.show(); //update strip
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
      hsv[0] += getDeltAdjust();
      if(hsv[0] > 255){hsv[0] = hsv[0] - 255;}
      if(hsv[0] < 0){hsv[0] = hsv[0] + 255;}
      tellColor(hsv[0], hsv[1], hsv[2]);
      showBossNeopixel();
    }
    if(parameter == 1){ //sat, out of 128 values
      hsv[1] += getDeltAdjust();
      if(hsv[1] > 127){hsv[1] = hsv[1] - 127;}
      if(hsv[1] < 0){hsv[1] = hsv[1] + 127;}
      tellColor(hsv[0], hsv[1], hsv[2]);
      showBossNeopixel();
    }
    if(parameter == 2){ //val, out of 128 values
      hsv[2] += getDeltAdjust();
      if(hsv[2] > 127){hsv[2] = hsv[2] - 127;}
      if(hsv[2] < 0){hsv[2] = hsv[2] + 127;}
      tellColor(hsv[0], hsv[1], hsv[2]);
      showBossNeopixel();
    }
    if(parameter == 3){ //pattern
      patP += ss.getEncoderDelta();
      if(patP >= numPatterns){patP -= numPatterns;}
      if(patP < 0){patP += numPatterns;}
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
