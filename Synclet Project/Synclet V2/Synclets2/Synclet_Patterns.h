void setArrow(int pos, int hv, int sv, int vv){ //takes 9 positions, 0-8
  stripT.setPixelColor(pos, stripT.ColorHSV(hv,sv,vv));
  stripM.setPixelColor(pos+1, stripT.ColorHSV(hv,sv,vv));
  stripB.setPixelColor(pos, stripT.ColorHSV(hv,sv,vv));
}

void showAll(){
  stripT.show();
  stripM.show();
  stripB.show();
}
void setDiag(int pos, int hv, int sv, int vv, boolean gam = false){
  if(!gam){
  if(pos == 0){
    stripT.setPixelColor(0, stripT.ColorHSV(hv,sv,vv));
    stripM.setPixelColor(0, stripT.ColorHSV(hv,sv,vv));
  }
  else if(pos == 9){
    stripM.setPixelColor(9, stripT.ColorHSV(hv,sv,vv));
    stripB.setPixelColor(8, stripT.ColorHSV(hv,sv,vv));
  }
  else{//range 1-8
     stripT.setPixelColor(pos, stripT.ColorHSV(hv,sv,vv));
     stripM.setPixelColor(pos, stripT.ColorHSV(hv,sv,vv));
     stripB.setPixelColor(pos-1, stripT.ColorHSV(hv,sv,vv));
  }
  }
  else{
    if(pos == 0){
      stripT.setPixelColor(0, stripT.gamma32(stripT.ColorHSV(hv,sv,vv)));
      stripM.setPixelColor(0, stripT.gamma32(stripT.ColorHSV(hv,sv,vv)));
    }
    else if(pos == 9){
      stripM.setPixelColor(9, stripT.gamma32(stripT.ColorHSV(hv,sv,vv)));
      stripB.setPixelColor(8, stripT.gamma32(stripT.ColorHSV(hv,sv,vv)));
    }
    else{//range 1-8
       stripT.setPixelColor(pos, stripT.gamma32(stripT.ColorHSV(hv,sv,vv)));
       stripM.setPixelColor(pos, stripT.gamma32(stripT.ColorHSV(hv,sv,vv)));
       stripB.setPixelColor(pos-1, stripT.gamma32(stripT.ColorHSV(hv,sv,vv)));
    }
  }
}

void fillAll(int32_t packColor){
  stripT.fill(packColor);
  stripM.fill(packColor);
  stripB.fill(packColor);
}

void FadeStatic(){

  valVal +=2;

  fillAll(stripT.ColorHSV(hueVal, satVal, stripT.sine8(valVal)*vRatio), 0, ledNum);
  
  showAll(); 
}

void FadeColorStep(){
  if(myTask.isFirstIteration()){
  }
  fillAll(stripT.ColorHSV(hueSix[ledPos], satVal, stripT.sine8(valVal)*vRatio), 0, ledNum);
  valVal = overflow(valVal+2, 0, 256);
  if(stripT.sine8(valVal) == 0){ledPos2++;}
  if(ledPos2 > 2){ledPos= overflow(ledPos+1, 0, 6); ledPos2 = 0;}
  showAll(); 
}

void FadeColorFlow(){
  if(myTask.isFirstIteration()){
    hueVal = 0;
  }
  setDiag(ledPos2, stripT.gamma8(hueVal)*256, satVal, stripT.sine8(ledPos)*vRatio);
  ledPos = overflow(ledPos+1, 0, 256);
  ledPos2 = overflow(ledPos2+1, 0, 10);
  showAll();
}

void FadeFlow(){
  if(myTask.isFirstIteration()){
  }
  for(int i = 0; i < 9; i++){
  setDiag(i, hueVal, satVal, stripT.sine8(ledPos)*vRatio);
  ledPos = overflow(ledPos+2, 0, 256);
  }
  
  showAll(); 
}


void CycleMove(){
  
  hueVal += 512;

  for (int i = 0; i< 10; i++){
    setDiag(i, hueVal + i*512, satVal, valVal, true);
  }
    showAll();
}

void CycleStay(){

  fillAll(stripT.ColorHSV(hueVal, 255, valVal), 0, ledNum);
  hueVal += 512;
  showAll();
}

void ChaseSingle(){
  if(myTask.isFirstIteration()){
    fillAll(stripT.ColorHSV(0, 0, 32), 0, ledNum);
    ledPos = 0;
  }
  stripT.setPixelColor(ledPos, stripT.ColorHSV(hueVal, satVal, valVal));
  showAll();
  stripT.setPixelColor(ledPos, stripT.ColorHSV(0,0,16));
  ledPos = overflow(ledPos+1, 0, ledNum);
}

void ChaseArrow(){
  
  fillAll(0);
  setArrow(ledPos,hueVal,satVal,valVal);
  ledPos = overflow(ledPos+1, 0, 9);
  setArrow(ledPos,hueVal,satVal,valVal);
  ledPos = overflow(ledPos+1, 0, 9);
  showAll();
}

void ChaseRain(){
  fillAll(0);
  setArrow(ledPos, hueSix[ledPos2], 255, valVal);
  setArrow(ledPos+3, hueSix[ledPos2], 255, valVal);
  setArrow(ledPos+6, hueSix[ledPos2], 255, valVal);
  
  showAll();

  setArrow(ledPos, 0,0,0);
  setArrow(ledPos+3, 0,0,0);
  setArrow(ledPos+6, 0,0,0);
  
  ledPos = overflow(ledPos+1, 0, 3);
  ledPos2 = overflow(ledPos2+1, 0, 6);
}

void RainbowRoad(){
  for(int i = 0; i <= 8; i++){
//  setArrow(i, hueNine(overflow(ledPos-i,0,9)), 255, valVal);
  strip.setPixelColor(i, pack9[overflow(ledPos - i, 0, 9)]);
  strip.setPixelColor(17-i, pack9[overflow(ledPos - i, 0, 9)]);
  strip.setPixelColor(i+19, pack9[overflow(ledPos - i, 0, 9)]);
  }

  showAll();
  ledPos = overflow(ledPos+1, 0, 9);

}

void enablePattern(){
  switch (pattern){
    case 's': //Static Clone
        fillAll(stripT.ColorHSV(hueVal, satVal, valVal), 0, ledNum);
        showAll();
    break;

    case 'f': //Fade
      myTask.setCallback(&FadeStatic);
      myTask.setInterval(25);
    break;

    case 't': //Fade Color Step
      myTask.setCallback(&FadeColorStep);
      myTask.setInterval(25);
    break;

    case 'F': //Fade Flow
      myTask.setCallback(&FadeFlow);
      myTask.setInterval(100);
    break;

    case 'l': //Fade Color Flow
      myTask.setCallback(&FadeColorFlow);
      myTask.setInterval(25);
    break;

    case 'r': //Static Rainbow
      strip.rainbow(0, 1, 255, valVal);
      showAll();
    break;

    case 'o': //darkness
      showAll();
    break;

    case 'y':
      myTask.setCallback(&CycleMove);
      myTask.setInterval(100);
    break;
    
    case 'c': //Cycle Rainbow
      myTask.setCallback(&CycleStay);
      myTask.setInterval(100);
    break;
    
    case 'u': //chase single
      myTask.setCallback(&ChaseSingle);
      myTask.setInterval(200);
    break;

    case 'w': //chase column
      myTask.setCallback(&ChaseColumn);
      myTask.setInterval(200);
    break;

    case 'a': //task rainbow road
      myTask.setCallback(&RainbowRoad);
      myTask.setInterval(250);
    break;

    case 'R': //task run rainbow
      myTask.setCallback(&ChaseRain);
      myTask.setInterval(500);
    break;
    
  }
}

void disablePattern(){
  myTask.disable();
  stripT.clear();
  ledPos = 0;
  ledPos2 = 0;
}