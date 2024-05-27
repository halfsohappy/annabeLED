//declare global variables for patterns
int colorVal;
uint8_t brightVal;


void enablePattern(){
  
    brightVal = 128;  
    
    switch (pattern){
      case 's': //s is color set, change all leds hue to receivedNum
        taskPatternSet.enable();
      break;
 
      case 'f': //f is fade, leds fade and brighten on last colorVal
        taskPatternFade.enable();
      break;

      case 'r': //r is rainbow, static rainbow fills the strip
        taskPatternRainbow.enable();
      break;

      case 'o': //o is off, fills strip with darkness
        taskPatternOff.enable();
      break;

      case 'c': //c is cycle, cycles hues of leds throughout strip
        taskPatternCycle.enable();
      break;
    }
}

void disablePattern(){
  switch (pattern){
      case 's': //s is color set, change all leds hue to receivedNum
        taskPatternSet.disable();
      break;
 
      case 'f': //f is fade, leds fade and brighten on gold color
        taskPatternFade.disable();
      break;

      case 'r': //r is rainbow, makes a rainbow
        taskPatternRainbow.disable();
      break;

      case 'o': //o is off, turns all leds off
        taskPatternOff.disable();
      break;

      case 'c': //c is color cycle, aka advanced rainbow
        taskPatternCycle.disable();
      break;
    }
}

void patternSet(){
  
  strip.fill(strip.ColorHSV(colorVal,   255,   brightVal), 0, ledNum); //set hue from colorVal
  
  strip.show();
  
  //Serial.println("case c");
  //Serial.println(colorVal);

}

void patternFade(){
  
//  if (directionFadeUp) {brightVal += 2;}
//  else {brightVal -= 2;}
//  if (brightVal == 132) {directionFadeUp = false;}
//  if (brightVal == 0) {directionFadeUp = true;}

  brightVal +=2;

  strip.fill(strip.ColorHSV(colorVal, 255, strip.sine8(brightVal)), 0, ledNum);
  
  strip.show(); 
  
  //Serial.println("case f");
  //Serial.println(brightVal);
}

void patternRainbow(){
  
      strip.rainbow(0, 1, 255, brightVal);

      strip.show();
      
      //Serial.println("case r");
}

void patternOff(){
  
      strip.clear();

      strip.show();
      
      //Serial.println("case o");
      
}

void patternCycle(){
  
  colorVal += 512;

  for (int i = 0; i< ledNum; i++){
    strip.setPixelColor(i, strip.ColorHSV((colorVal + i*512)));
    }
  
  strip.show();
}
