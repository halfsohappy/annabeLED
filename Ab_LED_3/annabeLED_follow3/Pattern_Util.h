int hueSix[] = {0,15*256,10923,21845,170*256,210*256};

int hueNine[] = {51044, 61839, 0, 2560, 10923, 21845, 32768, 48241, 54613};

int pack9[] = {strip.ColorHSV(hueNine[0], 255, 128), strip.ColorHSV(hueNine[1], 255, 128), strip.ColorHSV(hueNine[2], 255, 128),
                  strip.ColorHSV(hueNine[3], 255, 128), strip.ColorHSV(hueNine[4], 255, 128), strip.ColorHSV(hueNine[5], 255, 72),
                  strip.ColorHSV(hueNine[6], 255, 96), strip.ColorHSV(hueNine[7], 255, 72), strip.ColorHSV(hueNine[8], 255, 72)};

int pack6[] = {strip.ColorHSV(hueSix[0], 255, 128), strip.ColorHSV(hueSix[1], 255, 128), strip.ColorHSV(hueSix[2], 255, 128),
            strip.ColorHSV(hueSix[3], 255, 71.5), strip.ColorHSV(hueSix[4], 255, 76.5), strip.ColorHSV(hueSix[5], 255, 72)};

// ^pre-chosen colors for rainbow effects

void setArrow(int pos, int hv, int sv = 255, int vv = figure.val){ //takes 9 positions, 0-8
  strip.setPixelColor(pos, strip.ColorHSV(hv,sv,vv));
  strip.setPixelColor(19 - pos - 1, strip.ColorHSV(hv,sv,vv));
  strip.setPixelColor(pos + 20, strip.ColorHSV(hv,sv,vv));
}

void setDiag(int pos, int hv, int sv, int vv, boolean gam = false){ //accepts 12 positions, 0-11
  if(!gam){
    if(pos == 0){
      strip.setPixelColor(20, strip.ColorHSV(hv,sv,vv));
    }
    else if(pos == 11){
      strip.setPixelColor(9, strip.ColorHSV(hv,sv,vv));
    }
    else if(pos == 1){
      strip.setPixelColor(21, strip.ColorHSV(hv,sv,vv));
      strip.setPixelColor(19, strip.ColorHSV(hv,sv,vv));
    }
    else if(pos == 10){
      strip.setPixelColor(10, strip.ColorHSV(hv,sv,vv));
      strip.setPixelColor(8, strip.ColorHSV(hv,sv,vv));
    }
    else{//range 2-9
      strip.setPixelColor(pos-2, strip.ColorHSV(hv,sv,vv));
      strip.setPixelColor(20-pos, strip.ColorHSV(hv,sv,vv));
      strip.setPixelColor(20+pos, strip.ColorHSV(hv,sv,vv));
    }
  }
  else{
    if(pos == 0){
      strip.setPixelColor(20, strip.gamma32(strip.ColorHSV(hv,sv,vv)));
    }
    else if(pos == 11){
      strip.setPixelColor(9, strip.gamma32(strip.ColorHSV(hv,sv,vv)));
    }
    else if(pos == 1){
      strip.setPixelColor(21, strip.gamma32(strip.ColorHSV(hv,sv,vv)));
      strip.setPixelColor(19, strip.gamma32(strip.ColorHSV(hv,sv,vv)));
    }
    else if(pos == 10){
      strip.setPixelColor(10, strip.gamma32(strip.ColorHSV(hv,sv,vv)));
      strip.setPixelColor(8, strip.gamma32(strip.ColorHSV(hv,sv,vv)));
    }
    else{//range 2-9
       strip.setPixelColor(pos-2, strip.gamma32(strip.ColorHSV(hv,sv,vv)));
       strip.setPixelColor(20-pos, strip.gamma32(strip.ColorHSV(hv,sv,vv)));
       strip.setPixelColor(20+pos, strip.gamma32(strip.ColorHSV(hv,sv,vv)));
    }
  }
}

void setLine(int pos, int hv, int sv, int vv, boolean gam = false){ //accepts 10 positions, 0-9
  if(!gam){
      strip.setPixelColor(pos, strip.ColorHSV(hv,sv,vv));
      strip.setPixelColor(19-pos, strip.ColorHSV(hv,sv,vv));
      strip.setPixelColor(20+pos, strip.ColorHSV(hv,sv,vv));
    }
  else{
    strip.setPixelColor(pos, strip.gamma32(strip.ColorHSV(hv,sv,vv)));
    strip.setPixelColor(29-pos, strip.gamma32(strip.ColorHSV(hv,sv,vv)));
    strip.setPixelColor(20+pos, strip.gamma32(strip.ColorHSV(hv,sv,vv)));
  }
}

uint8_t red(uint32_t pac){
  return (pac >> 16) & 0xFF;
}
uint8_t green(uint32_t pac){
  return (pac >> 8) & 0xFF;
}
uint8_t blue(uint32_t pac){
  return pac & 0xFF;
}

int overflow(int value, int lowbound, int upbound){
  if(value >= upbound){return value - upbound + lowbound;}
  if(value < lowbound){return value + (upbound - lowbound);}
  return value;
}