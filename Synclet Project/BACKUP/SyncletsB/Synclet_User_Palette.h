#define noNum 0

long hDeg(long d){return ((d/360)*65535);}
long sPer(long s){return ((s/100)*255);}

uint32_t pack(long h, long s){
  return strip.ColorHSV(hDeg(h), sPer(s), valVal);
}

void UPtrans(){
  if(noNum == 0 || noNum == 4){
    strip.fill(pack(199, 43), 0, ledNum); //baby blue
  }
  if(noNum == 1 || noNum == 3){
    strip.fill(pack(351, 28), 0, ledNum); //pink :D
  }
  if(noNum == 2){
    strip.fill(pack(0, 0), 0, ledNum); //white
  }
}

void userPalette(int myVal){
  int select = myVal / 10;

  if(select == 0){
    UPtrans();
    taskPatternSet.enable();
  }
  
  strip.show();
}
