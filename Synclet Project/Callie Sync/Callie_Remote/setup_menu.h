typedef struct page_line{
  char *content; //* to string of what the line says
  uint8_t position; //which line it is on (values 1-7 inclusive) 
  uint8_t lim; //max value it can be
  uint8_t message_factor; //what to multiply limit by before sending (not displaying)
  uint8_t display_factor; //optional, what to multiply by on the display
} page_line;

typedef struct page_line_s{
  char *content; //* to string of what the line says
  uint8_t position; //which line it is on (values 1-7 inclusive) 
  char *options[];
} page_line_s;

struct whole_page{
  char *title; //* to string of what the line says
  page_line *lines; //lines on this page
  page_line_s string_lines[3]; //string lines on this page
};


char *pal_array[] = {"Cloud","Lava","Ocean","Forest","Rainbow","Rainbow Stripes","Party","Heat"};

page_line Static_Color[] = {
  {"Hue [1]:", 1, 256, 1},
  page_line{"Saturation [1]:", 2, 128, 2},
  page_line{"Brightness [1]:", 3, 128, 2},
  
  page_line{"Hue [R]:", 5, 256, 1},
  page_line{"Saturation [R]:", 6, 128, 2},
  page_line{"Brightness [R]:", 7, 128, 2}
};
page_line_s Static_Color_S = page_line_s{"Match Sides?", 4, {"Yes, No"}};

//PRESET GRADIENT HAS NO NONSTRING LINES
page_line_s Preset_Gradient_S = page_line_s{"Preset:", 1, *pal_array};

page_line Custom_Gradient[] = {
  page_line{"Hue [1]:", 1, 256, 1},
  page_line{"Saturation [1]:", 2, 128, 2},
  page_line{"Brightness [1]:", 3, 128, 2},
  
  page_line{"Hue [2]:", 4, 256, 1},
  page_line{"Saturation [2]:", 5, 128, 2},
  page_line{"Brightness [2]:", 6, 128, 2},

  page_line{"Speed (ms):", 7, 20+1, 1, 100}
};

page_line Rainbow[4] = {
  page_line{"Length:",1,108,1},
  page_line{"Start:",2,108,1},

  page_line{"Speed (ms):", 4, 20+1, 1, 100},
  page_line{"Brightness:", 5, 128, 2}
};

page_line Breathing[4] = {
  page_line{"Hue:",1,256,1},
  page_line{"Saturation:",3,128,2},
  page_line{"Brightness:", 5, 128, 2},
  page_line{"Speed (ms):", 7, 20+1, 1, 100}
};

page_line Pulse[4] = {
  page_line{"Hue:",1,256,1},
  page_line{"Saturation:",3,128,2},
  page_line{"Brightness:", 5, 128, 2},
  page_line{"Speed (ms):", 7, 20+1, 1, 100}
};

page_line Sparkle[5] = {
  page_line{"Hue:",1,256,1},
  page_line{"Saturation:",2,128,2},
  page_line{"Brightness:", 3, 128, 2},
  //use palette?
  //what palette
  page_line{"Speed:", 6, 20+1, 1, 100},
  page_line{"Coverage %:", 7, 50+1, 1, 1},
};

page_line_s Sparkle_S4 = page_line_s{"Use Palette Instead?:", 4, {"Yes, No"}};
page_line_s Sparkle_S5 = page_line_s{"Palette:", 5, *pal_array};

page_line Zones[1] = {
  page_line{"Speed:", 5, 20+1, 1, 100},
};
page_line_s Zones_S1 = page_line_s{"Palette:", 1, *pal_array};
page_line_s Zones_S3 = page_line_s{"Function:", 3, {"Coming Soon"}};


struct whole_page menu[] = {
  whole_page{"Static Color", Static_Color, {Static_Color_S}},
  whole_page{"Preset Gradient", 0, {Preset_Gradient_S}},
  whole_page{"Custom Gradient", Custom_Gradient},
  whole_page{"Rainbow", Rainbow},
  whole_page{"Breathing", Breathing},
  whole_page{"Pulse", Pulse},
  whole_page{"Sparkle", Sparkle, {Sparkle_S4,Sparkle_S5}},
  whole_page{"Zones", Zones, {Zones_S1, Zones_S3}}
};
