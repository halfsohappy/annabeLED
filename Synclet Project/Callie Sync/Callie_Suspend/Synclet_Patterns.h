#include "colorpalettes.h"
#include <cstring>
#include "colorutils.h"

void stripeify(CRGB *line){
  CRGB* temp = (CRGB*) malloc(sizeof(CRGB)*108);
  memcpy(temp, line, sizeof(CRGB)*108);
  for(int i=0; i<54; i++){
    line[53-i] = temp[2*i-1];
  }
  for(int i=1; i<55; i++){
    line[53+i] = temp[2*i];
  }
  free(temp);
}

void static_color(){//CRGB *ledarr, CHSV c0, CHSV c1, int num_led){
  fill_solid(left_strap, 108, callie_report.color0);
  //fill_solid(right_strap, 108, callie_report.color0);
  if(callie_report.speed != 0){fill_solid(right_strap, 108, callie_report.color0);}
  else{fill_solid(right_strap, 108, callie_report.color1);}
}

uint8_t position = 0;
//void c_g_over(){delete c_g_pal;}

void custom_gradient(){

  fill_gradient(left_strap, 0, callie_report.color0, 107, callie_report.color1);
  fill_gradient(right_strap, 0, callie_report.color1, 107, callie_report.color0);
  //if(callie_report.speed){position++;}
  //stripeify(left_strap);
  //stripeify(right_strap);
}

void fill_new(){
  CRGB blend_array[2*led_per_strap];
  int grad_pos = 0;
  fill_gradient(blend_array, 0, callie_report.color0, 107, callie_report.color1);
  fill_gradient(blend_array, 108, callie_report.color1, 215, callie_report.color0);

  for(int i = 0; i < led_per_strap; i++){
    //int local_pos (grad_pos>215) ? grad_pos-216 : grad_pos;
    right_strap[i] = blend_array[((grad_pos+i)>215) ? grad_pos+i-216 : grad_pos+i];

  }}

// CREATE ARRAY OF BLENDING, CREATE ANOTHER IN OTHER DIRECTION, SCROLL THROUGH ARRAY FOR BLENDING, HOORAY

CRGBPalette256 choose_pal(int pal){
  switch(pal){
   case 0: return cherry_bomb;
   case 1: return blue_candy;
   case 2: return electric_punk;
   case 3: return sweet_kiss;
   case 4: return ocean_breeze;
   case 5: return alarm_;
   case 6: return deep_space;
   case 7: return ice;
   case 8: return bisexual_lighting;
   case 9: return lesbian;
   }
}
uint64_t position;


void pre_gradient(){
  fill_palette_circular(left_strap, 108, position, (choose_pal(9)), callie_report.color0.val, LINEARBLEND, rev);
  fill_palette_circular(right_strap, 108, position, (choose_pal(9)), callie_report.color0.val, LINEARBLEND, rev);
  if(callie_report.speed){position++;}
  stripeify(left_strap);
  stripeify(right_strap);
}

void rainbow(){
  fill_rainbow(left_strap, 108, 8*position, 2);
  fill_rainbow(right_strap, 108, 8*position, 2);
  position++;
  stripeify(left_strap);
  stripeify(right_strap);
  FastLED.setBrightness(callie_report.color0.val);
  //ADD BRIGHTNESS IDIOT (i did :)
}



//Breathing

//pulse

//sparkle

//-make tasks that run cont. and switch their pixel every 256th run
//-number of tasks is based on coverage
//-frequency would be speed of overall timer


//zone




//pulse function:
//takes a color, speed, and lets say length

//have global var that tracks where along chain we are

//then starts task with speed variable that advances and re-paints

//need a

//Task runTask(100, -1);





void enablePattern(){
  switch (int(callie_report.figure)){
    case 0: //static color
     fig_task.setCallback(&static_color);
      static_color();
      fig_task.setInterval(1000);
    break;

    case 1: //preset gradient
      position = 0;
      fig_task.setCallback(&pre_gradient);
      pre_gradient();
      fig_task.setInterval(callie_report.speed*50 ? callie_report.speed : 1000);
    break;

    case 2: //custom gradient
      position = 0;
      c_g_pal = new CHSVPalette256(callie_report.color0, callie_report.color1);
      fig_task.setCallback(&custom_gradient);
      custom_gradient();
      fig_task.setInterval(callie_report.speed*50 ? callie_report.speed : 1000);
      fig_task.setOnDisable(&c_g_over);
    break;

    case 3: //rainbow
      position = 0;
      fig_task.setCallback(&rainbow);
      rainbow();
      fig_task.setInterval(callie_report.speed*50 ? callie_report.speed : 1000);
    break;
    
    // default:
    //   fig_task.setCallback(&static_color);
    //   fig_task.setInterval(250);
  }
}

void disablePattern(){
  delete c_g_pal;
}