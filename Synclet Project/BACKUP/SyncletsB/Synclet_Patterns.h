

void enablePattern(){
    
    switch (pattern){
      case 's': //s is color set, change all leds hue to colorVal
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

      case 'm': //m is "my", changes synclet to defined color
        taskPatternMy.enable();
      break;
      
      case 'u': //u is run, like a chase of a single color led
        strip.fill(strip.ColorHSV(0, 0, 32), 0, ledNum);
        int ledPos = 0;
        taskPatternRun.enable();
      break;

//      case 'X':
//        mesh.stop();
//        mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );

      
    }
}

void disablePattern(){
  switch (pattern){
      case 's': 
        taskPatternSet.disable();
      break;
 
      case 'f':
        taskPatternFade.disable();
      break;

      case 'r':
        taskPatternRainbow.disable();
      break;

      case 'o':
        taskPatternOff.disable();
      break;

      case 'c': 
        taskPatternCycle.disable();
      break;

      case 'm':
        taskPatternMy.disable();
      break;

      case 'u':
        taskPatternRun.disable();
      break;
    }
}

void patternSet(){
  
  strip.fill(strip.ColorHSV(hueVal, satVal, valVal), 0, ledNum); //set hue from colorVal
  
  strip.show();

}

void patternFade(){
  
//  if (directionFadeUp) {brightVal += 2;}
//  else {brightVal -= 2;}
//  if (brightVal == 132) {directionFadeUp = false;}
//  if (brightVal == 0) {directionFadeUp = true;}

  valVal +=2;

  strip.fill(strip.ColorHSV(hueVal, satVal, strip.sine8(valVal)), 0, ledNum);
  
  strip.show(); 
  
  //Serial.println("case f");
  //Serial.println(brightVal);
}

void patternRainbow(){
  
      strip.rainbow(0, 1, 255, valVal);

      strip.show();
      
      //Serial.println("case r");
}

void patternOff(){
  
      strip.clear();

      strip.show();
      
      //Serial.println("case o");
      
}

void patternCycle(){
  
  hueVal += 512;

  for (int i = 0; i< ledNum; i++){
    strip.setPixelColor(i, strip.ColorHSV(hueVal + i*512, satVal, valVal));
    }
  
  strip.show();
}

void patternMy(){
  userPalette(satVal);
}

void patternRun(){
  strip.setPixelColor(ledPos, strip.ColorHSV(hueVal, satVal, valVal));
  strip.show();
  strip.setPixelColor(ledPos, strip.ColorHSV(0,0,32));
  ledPos++;
  if(ledPos == ledNum){ledPos = 0;}
}
