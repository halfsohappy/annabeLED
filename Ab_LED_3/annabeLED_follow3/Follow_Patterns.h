#define STATIC_RAINBOW 0 //set 9 static arrows in ROYGBIV order //DONE

//"Cycle" indicates a color-changing or gradient effect
#define CYCLE_SIMPLE 1 //slow gradient, whole brace changes color together //DONE
#define CYCLE_FLOW 2 //slow gradient from color to color in lines //DONE

//"Fade" indicates breathing effect
#define FADE_SIMPLE 3 //breathes on a single color //DONE
#define FADE_FLOW 4 //brace breathes one arrow at a time, moving the other arrows over like ".oO0Oo.oO0Oo." //DONE
#define FADE_COLOR_STEP 5 // changes color on each "breath" through the rainbow //DONE

//"Random" indicates,,, random generation
#define RANDOM_WHITE 6 //randomly turns on white pixels at random brightnesses //DONE
#define RANDOM_COLOR 7 //randomly turns pixels on at random colors, set brightness //DONE

#define CHASE_SINGLE_ARROW 8 //one arrow on a set color runs down the brace //DONE
#define CHASE_RAINBOW_DIAG 9 //rainbow diagonals run down the brace (6 colors) //DONE
#define CHASE_RAINBOW_ARROW 10 //rainbow arrows run down the brace (9 colors) //DONE

#define DARKNESS 11  



void staticRainbow(){
  for(int i = 0; i<8; i++){
    setArrow(i, hueNine[i]);
  }
  strip.show();
}

void cycleSimple(){
  strip.fill(strip.gamma32(strip.ColorHSV(figure.hue, 255, figure.val)));
  figure.hue += 512;
  strip.show();
}

void cycleFlow(){
  if(dance.isFirstIteration()){
    figure.hue = 0;
  }

  for(int i = 0; i<10; i++){
    setLine(i, figure.hue + i*256, figure.sat, figure.val, true);
  }

  strip.show();
  figure.hue += 256;
}

void fadeSimple(){
  figure.val +=2;
  strip.fill(strip.ColorHSV(figure.hue, figure.sat, strip.sine8(figure.val)*vRatio));
  
  strip.show(); 
}

void fadeFlow(){
  if(dance.isFirstIteration()){
    figure.val = 0;
  }

  for(int i = 0; i<9; i++){
    setLine(i, figure.hue, figure.sat, strip.sine8(figure.val)*vRatio);
  }

  strip.show();
  figure.val += 2;
}

void fadeColorStep(){
  strip.fill(strip.ColorHSV(hueSix[led_pos[0]], figure.sat, strip.sine8(figure.val)*vRatio));

  figure.val = overflow(figure.val+2, 0, 256);
  if(strip.sine8(figure.val) == 0){led_pos[1]++;}
  if(led_pos[1] > 2){led_pos[0]= overflow(led_pos[0]+1, 0, 6); led_pos[1] = 0;}

  strip.show(); 
}

void randomWhite(){
  for(int i = 0; i<30; i++){
    uint32_t color = strip.getPixelColor(i);
    if(blue(color) + red(color) + green(color) > 30){
      if(random(0,8)==1){
        strip.setPixelColor(i, strip.Color(blue(color)*.6, red(color)*.6, green(color)*.6));
      }
    }
    else if(random(0,6)==5){
      strip.setPixelColor(i, 65535);
    }
  }
  strip.show();
}

void randomColor(){
  for(int i = 0; i<30; i++){
    uint32_t color = strip.getPixelColor(i);
    if(blue(color) + red(color) + green(color) > 30){
      if(random(0,8)==1){
        strip.setPixelColor(i, strip.Color(blue(color)*.6, red(color)*.6, green(color)*.6));
      }
    }
    else if(random(0,6)==5){
      strip.setPixelColor(i, strip.Color(random(0,256), random(0,256), random(0,256)));
    }
  }
  strip.show();
}

void chaseSingleArrow(){
  if(dance.isFirstIteration()){
    led_pos[0] = 0;
  }
  setArrow(led_pos[0], figure.hue, figure.sat, figure.val);
  strip.show();
  led_pos[0] = overflow(led_pos[0] + 1, 0, 9);
}

void chaseRainbowDiag(){
  for(int i = 0; i < 12; i++){
    setDiag(i, hueSix[overflow(led_pos[0]-i,0,9)], 255, figure.val);
  }
  strip.show();
  led_pos[0] = overflow(led_pos[0]+1, 0, 9);
}

void chaseRainbowArrow(){
  for(int i = 0; i < 9; i++){
    setArrow(i, hueNine[overflow(led_pos[0]-i,0,9)], 255, figure.val);
  }

  strip.show();
  led_pos[0] = overflow(led_pos[0]+1, 0, 9);
}

void darkness(){
  strip.fill(0);
  strip.show();
}



typedef struct fig{
  void (*point)(void);
  int interval;
} fig;

fig figure_list[12] = {
  {&staticRainbow, 480},
  {&cycleSimple, 60},{&cycleFlow, 60},
  {&fadeSimple, 120},{&fadeFlow, 60},{&fadeColorStep, 60},
  {&randomWhite, 240},{&randomColor, 240},
  {&chaseSingleArrow, 60},{&chaseRainbowDiag, 120},{&chaseRainbowArrow, 240},
  {&darkness, 480}
};



void start_figure(int figdex){
  dance.setCallback(figure_list[figdex].point);
  dance.setInterval(figure_list[figdex].interval);
}

void end_figure(){
  dance.disable();
  strip.clear();
  led_pos[0] = 0;
  led_pos[1] = 0;
}
