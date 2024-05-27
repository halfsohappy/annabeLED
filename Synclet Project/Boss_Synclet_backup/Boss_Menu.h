//pins for three momentary switches
#define buttonPinOne 3
#define buttonPinTwo 4
#define buttonPinThree 37
#define onePot 6
#define twoPot 7
#define threePot 5
#define fourPot 2
#define potVal 4096
#define RED 0 
#define GREEN 1 
#define BLUE 2
#define HUE 0 
#define SAT 1 
#define VAL 2

int hsv[] = {0,0,0};

#include <Adafruit_NeoPixel.h>
#define bossLedPin 21
Adafruit_NeoPixel strip(1, bossLedPin, NEO_GRB + NEO_KHZ800);

//everything needed for scheduling button watching and message sending
bool lastSwitchStatus[] = {HIGH, HIGH, HIGH};
bool switchPushed = false;

void buttonWatch();
void potWatch();
void updateEnc();

Scheduler userScheduler;
Task taskButtonWatch(50, -1, &buttonWatch);
Task taskPotWatch(350, -1, &potWatch);
Task taskUpdateEnc(350, -1, &updateEnc);

uint32_t bossColorVal;
uint8_t bossBrightVal;

void showBossNeopixel(){//update strip to bossColorVal
  strip.setPixelColor(0, strip.ColorHSV(hsv[0], hsv[1], hsv[2])); //set hue from bossColorVal
  strip.show(); //update strip
}

//what do buttons/knobs do?
void buttonOnePress() { //send command with current pattern + values
      Serial.println("buttonOne Press");
      
      sendSyncletCommand(patternPlace, bossColorVal, bossBrightVal);
}

void buttonTwoPress(){ //change potControl
      Serial.println("buttonTwo Press");

}

void buttonThreePress(){ //nothing right now
//      Serial.println("buttonThree Press");
//      
//      patternPlace++;
//      if (patternPlace >= numOfPatterns){patternPlace = 0;}
//      
//      sendSyncletCommand(patternPlace, bossColorVal);


} 

void potCheckHSV(int value){ //potentiometer
  if(value == 0){if (abs(hsv[value] - hsvOld[value]) >= 5){
    tellColor(hsv[0], hsv[1], hsv[2]);
    showBossNeopixel();
    hsvOld[value] = hsv[value];
  }hsvOld[value] = hsv[value];}
  else if(hsv[value] != hsvOld[value]){
    tellColor(hsv[0], hsv[1], hsv[2]);
    showBossNeopixel();
    hsvOld[value] = hsv[value];
    }
}

void potCheckPat(){ //potentiometer
  if(patternPlace != oldP){
    tellPattern(patterns[patternPlace]);
    oldP = patternPlace;
    }
}

void potWatch(){ //potentiometer
  
    hsv[0] = analogRead(onePot) * 16;
    potCheckHSV(0);
    hsv[1] = analogRead(twoPot)/16;
    potCheckHSV(1);
    hsv[2] = analogRead(threePot)/16;
    potCheckHSV(2);
    patternPlace = analogRead(fourPot)/(4095/(numOfPatterns-1));
    potCheckPat();
}

//button watching
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
  //2nd switch
  switchStatus = digitalRead(buttonPinTwo);
  if (lastSwitchStatus[1] != switchStatus)//was there a change in switch state?
  {
   lastSwitchStatus[1] = switchStatus;//update to the new state
   if (switchStatus == switchPushed)//did the switch get pushed?
    {
        buttonTwoPress();
    } 
  }
  //3rd switch
  switchStatus = digitalRead(buttonPinThree);
  if (lastSwitchStatus[2] != switchStatus)//was there a change in switch state?
  {
   lastSwitchStatus[2] = switchStatus;//update to the new state
   if (switchStatus == switchPushed)//did the switch get pushed?
    {
        buttonThreePress();
    } 
  }
}
