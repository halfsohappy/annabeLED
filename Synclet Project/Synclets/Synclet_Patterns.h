void enablePattern(){
    strip.clear();
    ledPos = 0;
    ledPos2 = 0;
    switch (pattern){
      case 's': //Static Clone
          strip.fill(strip.ColorHSV(hueVal, satVal, valVal), 0, ledNum);
          strip.show();
      break;
 
      case 'f': //Fade
        taskFadeStatic.enable();
      break;

      case 't': //Fade Color Step
        taskFadeColorStep.enable();
      break;

      case 'F': //Fade Flow
        taskFadeFlow.enable();
      break;

      case 'l': //Fade Color Flow
        taskFadeColorFlow.enable();
      break;

      case 'r': //Static Rainbow
        strip.rainbow(0, 1, 255, valVal);
        strip.show();
      break;

      case 'o': //darkness
        strip.show();
      break;

      case 'y':
        taskCycleMove.enable();
      break;
      
      case 'c': //Cycle Rainbow
        taskCycleStay.enable();
      break;
      
      case 'u': //chase single
        taskChaseSingle.enable();
      break;

      case 'w': //chase column
        taskChaseColumn.enable();
      break;

      case 'a': //task rainbow road
        taskRainbowRoad.enable();
      break;

      case 'R': //task run rainbow
        taskChaseRain.enable();
      break;
      
    }
}

void disablePattern(){
        taskFadeStatic.disable();
        taskFadeFlow.disable();
        taskFadeColorStep.disable();
        taskFadeColorFlow.disable();
        
        taskCycleStay.disable();
        taskCycleMove.disable();
        
        taskChaseSingle.disable();
        taskChaseColumn.disable();
        taskChaseRain.disable();

        taskRainbowRoad.disable();
}

void setArrow(int pos, int hv, int sv, int vv){ //takes 9 positions, 0-8
  strip.setPixelColor(pos, strip.ColorHSV(hv,sv,vv));
  strip.setPixelColor(17-pos, strip.ColorHSV(hv,sv,vv));
  strip.setPixelColor(pos+19, strip.ColorHSV(hv,sv,vv));
}

void setDiag(int pos, int hv, int sv, int vv, boolean gam = false){
  if(!gam){
  if(pos == 0){
    strip.setPixelColor(18, strip.ColorHSV(hv,sv,vv));
    strip.setPixelColor(19, strip.ColorHSV(hv,sv,vv));
  }
  else if(pos == 9){
    strip.setPixelColor(8, strip.ColorHSV(hv,sv,vv));
    strip.setPixelColor(9, strip.ColorHSV(hv,sv,vv));
  }
  else{//range 1-8
     pos--;
     strip.setPixelColor(pos, strip.ColorHSV(hv,sv,vv));
     strip.setPixelColor(17-pos, strip.ColorHSV(hv,sv,vv));
     strip.setPixelColor(20+pos, strip.ColorHSV(hv,sv,vv));
  }
  }
  else{
    if(pos == 0){
      strip.setPixelColor(18, strip.gamma32(strip.ColorHSV(hv,sv,vv)));
      strip.setPixelColor(19, strip.gamma32(strip.ColorHSV(hv,sv,vv)));
    }
    else if(pos == 9){
      strip.setPixelColor(8, strip.gamma32(strip.ColorHSV(hv,sv,vv)));
      strip.setPixelColor(9, strip.gamma32(strip.ColorHSV(hv,sv,vv)));
    }
    else{//range 1-8
       pos--;
       strip.setPixelColor(pos, strip.gamma32(strip.ColorHSV(hv,sv,vv)));
       strip.setPixelColor(17-pos, strip.gamma32(strip.ColorHSV(hv,sv,vv)));
       strip.setPixelColor(20+pos, strip.gamma32(strip.ColorHSV(hv,sv,vv)));
    }
  }
}

void FadeStatic(){

  valVal +=2;

  strip.fill(strip.ColorHSV(hueVal, satVal, strip.sine8(valVal)*vRatio), 0, ledNum);
  
  strip.show(); 
}

void FadeColorStep(){
  if(taskFadeColorStep.isFirstIteration()){
  }
  strip.fill(strip.ColorHSV(hueSix[ledPos], satVal, strip.sine8(valVal)*vRatio), 0, ledNum);
  valVal = overflow(valVal+2, 0, 256);
  if(strip.sine8(valVal) == 0){ledPos2++;}
  if(ledPos2 > 2){ledPos= overflow(ledPos+1, 0, 6); ledPos2 = 0;}
  strip.show(); 
}

void FadeColorFlow(){
  if(taskFadeColorFlow.isFirstIteration()){
    hueVal = 0;
  }
  setDiag(ledPos2, strip.gamma8(hueVal)*256, satVal, strip.sine8(ledPos)*vRatio);
  ledPos = overflow(ledPos+1, 0, 256);
  ledPos2 = overflow(ledPos2+1, 0, 10);
  strip.show();
}

void FadeFlow(){
  if(taskFadeFlow.isFirstIteration()){
  }
  for(int i = 0; i < 9; i++){
  setDiag(i, hueVal, satVal, strip.sine8(ledPos)*vRatio);
  ledPos = overflow(ledPos+2, 0, 256);
  }
  
  strip.show(); 
}


void CycleMove(){
  
  hueVal += 512;

  for (int i = 0; i< 10; i++){
    setDiag(i, hueVal + i*512, satVal, valVal, true);
  }
    strip.show();
}

void CycleStay(){

  strip.fill(strip.ColorHSV(hueVal, 255, valVal), 0, ledNum);
  hueVal += 512;
  strip.show();
}

void ChaseSingle(){
  if(taskChaseSingle.isFirstIteration()){
    strip.fill(strip.ColorHSV(0, 0, 32), 0, ledNum);
    ledPos = 0;
  }
  strip.setPixelColor(ledPos, strip.ColorHSV(hueVal, satVal, valVal));
  strip.show();
  strip.setPixelColor(ledPos, strip.ColorHSV(0,0,16));
  ledPos = overflow(ledPos+1, 0, ledNum);
}

void ChaseColumn(){
  
  strip.clear();
  setArrow(ledPos,hueVal,satVal,valVal);
  ledPos = overflow(ledPos+1, 0, 9);
  setArrow(ledPos,hueVal,satVal,valVal);
  ledPos = overflow(ledPos+1, 0, 9);
  strip.show();
}

void ChaseRain(){
  strip.clear();
  setArrow(ledPos, hueSix[ledPos2], 255, valVal);
  setArrow(ledPos+3, hueSix[ledPos2], 255, valVal);
  setArrow(ledPos+6, hueSix[ledPos2], 255, valVal);
  
  strip.show();

  setArrow(ledPos, 0,0,0);
  setArrow(ledPos+3, 0,0,0);
  setArrow(ledPos+6, 0,0,0);
  
  ledPos = overflow(ledPos+1, 0, 3);
  ledPos2 = overflow(ledPos2+1, 0, 6);
}

void RainbowRoad(){
  for(int i = 0; i <= 8; i++){
//  setArrow(i, hueNine(overflow(ledPos-i,0,9)), 255, valVal);
  strip.setPixelColor(i, p9[overflow(ledPos - i, 0, 9)]);
  strip.setPixelColor(17-i, p9[overflow(ledPos - i, 0, 9)]);
  strip.setPixelColor(i+19, p9[overflow(ledPos - i, 0, 9)]);
  }

  strip.show();
  ledPos = overflow(ledPos+1, 0, 9);

}
