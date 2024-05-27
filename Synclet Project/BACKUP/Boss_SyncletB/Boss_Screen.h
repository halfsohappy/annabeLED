
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

String globalPat = "";
String globalVal1 = "";
String globalVal2 = "";
String globalVal3 = "";

void redraw(){
  display.clearDisplay();
  
  display.setCursor(0, 0);
  display.write("Pattern:");
  display.write(globalPat.c_str());
  
  display.setCursor(0, 9); 
  display.write("H:");
  display.write(globalVal1.c_str());

  display.setCursor(48, 9); 
  display.write("S:");
  display.write(globalVal2.c_str());

  display.setCursor(96, 9); 
  display.write("V:");
  display.write(globalVal3.c_str());
  
  display.setCursor(0, 18);
  display.write("# of Nodes:");

  display.write(numN.c_str());
  
  display.display();
}

void tellPattern(char pattern){
  String pString;
  switch (pattern){
      case 's': //s is color set, change all leds hue to colorVal
        pString = "StaticClone";
      break;
 
      case 'f': //f is fade, leds fade and brighten on last colorVal
        pString = "Fade";
      break;

      case 'r': //r is rainbow, static rainbow fills the strip
        pString = "StaticRainbow";
      break;

      case 'o': //o is off, fills strip with darkness
        pString = "Darkness";
      break;

      case 'c': //c is cycle, cycles hues of leds throughout strip
        pString = "CycleRainbow";
      break;

      case 'm': //m is "my", changes synclet to defined color
        pString = "UserPalette";
      break;
      
      case 'u': //u is run, like a chase of a single color led
        pString = "Chase";
      break;

      case 'X': //u is run, like a chase of a single color led
        pString = "DEEP_SLEEP";
      break;
  }
  globalPat = String(pString);
  redraw();
  
}

void tellUserPalette(){
  
}

void tellColor(int v1, int v2, int v3){
  globalVal1 = String(v1);
  globalVal2 = String(v2);
  globalVal3 = String(v3);
  
  redraw();
}
