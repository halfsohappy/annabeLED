#include "Adafruit_seesaw.h"
#include <seesaw_neopixel.h>

#define SS_SWITCH        24
#define SS_NEOPIX        6

#define SEESAW_ADDR          0x36

#define INT_PIN 6

Adafruit_seesaw ss;
seesaw_NeoPixel sspixel = seesaw_NeoPixel(1, SS_NEOPIX, NEO_GRB + NEO_KHZ800);

int32_t ePos;

boolean coarse = true;

void setupEnc() {

  ss.begin(SEESAW_ADDR);
  sspixel.begin(SEESAW_ADDR);
  Serial.println("seesaw started");

  sspixel.setBrightness(20);
  sspixel.show();
  
  // use a pin for the built in encoder switch
  ss.pinMode(SS_SWITCH, INPUT_PULLUP);

  // get starting position
  ePos = ss.getEncoderPosition();

  Serial.println("Turning on interrupts");
  delay(10);
  ss.setGPIOInterrupts((uint32_t)1 << SS_SWITCH, 1);
  ss.enableEncoderInterrupt();
}

int getDeltAdjust() {
  int delt = ss.getEncoderDelta();
  delt += delt*4*coarse;
  return delt;
}

void updateEnc(int setting){
  if(getDeltAdjust()){
    
    if(setting == 0){ //hue, out of 256 values
      hsv[0] += getDeltAdjust();
      if(hsv[0] > 255){hsv[0] = hsv[0] - 255;}
      if(hsv[0] < 0){hsv[0] = hsv[0] + 255;}
      tellColor(hsv[0], hsv[1], hsv[2]);
      showBossNeopixel();
    }
    if(setting == 1){ //sat, out of 128 values
      hsv[1] += getDeltAdjust();
      if(hsv[1] > 127){hsv[1] = hsv[1] - 127;}
      if(hsv[1] < 0){hsv[1] = hsv[1] + 127;}
      tellColor(hsv[0], hsv[1], hsv[2]);
      showBossNeopixel();
    }
    if(setting == 2){ //val, out of 128 values
      hsv[2] += getDeltAdjust();
      if(hsv[2] > 127){hsv[2] = hsv[2] - 127;}
      if(hsv[2] < 0){hsv[2] = hsv[2] + 127;}
      tellColor(hsv[0], hsv[1], hsv[2]);
      showBossNeopixel();
    }
    if(setting == 3){ //pattern
      patternPlace += ss.getEncoderDelta();
      if(patternPlace > numOfPatterns){patternPlace -= numOfPatterns;}
      if(patternPlace < numOfPatterns){patternPlace += numOfPatterns;}
      tellPattern(patterns[patternPlace]);
    }
    
  }
}

void IRAM_ATTR onInterrupt(){
  coarse = !coarse;
}

void enableInterrupts(){
   ss.setGPIOInterrupts((uint32_t)1 << SS_SWITCH, 1); //turn on seesaw interrupts
   attachInterrupt(INT_PIN, onInterrupt, RISING); //have esp32 watch for seesaw int
}
