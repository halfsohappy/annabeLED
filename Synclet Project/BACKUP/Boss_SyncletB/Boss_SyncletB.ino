#include "WiFi.h"
#include "painlessMesh.h"
#include "Adafruit_seesaw.h"
#include <seesaw_neopixel.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define   MESH_PREFIX     "SyncletMesh"
#define   MESH_PASSWORD   "PrattStar"
#define   MESH_PORT       5555
painlessMesh  mesh;

//declare node IDs
#define nodeA 402915293
#define nodeB 402859677
#define nodeC 402884033
#define nodeD 402849465
#define nodeE 402864161

//pattern codes (chars) for broadcast
char patterns[] = {'s', 'f', 'r', 'o', 'c', 'm', 'u', 'X'}; 
int patP = 0;
int numPatterns = 8;//IF ADDING PATTERNS, CHANGE numPatterns!!!!!

int hsv[] = {0,127,72};
int parameter;
String numN;
char palette[] = {'T', 'D'}; 
int palP = 0;
int numPal = 2;

//messages contain a character that reoresents pattern selection, and an integer(s) to modify the pattern
void sendSyncletCommand(){
  String message = "";
  
  message += patterns[patP]; message += '-';
  message += hsv[0]*256; message += '<';
  message += hsv[1]*2; message += '>';
  message += hsv[2]*2;
  
  mesh.sendBroadcast(message);
  Serial.println(message);
}

#include "Boss_Screen.h"
#include "Boss_Menu.h"

void receivedCallback( uint32_t from, String &msg ) {
  Serial.println(msg);
}

void newConnectionCallback(uint32_t nodeId) {
Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
numN = String(sizeof(mesh.getNodeList())/4);
}

void setup() {
  Serial.begin(115200);
  delay(100);
  
  pinMode(buttonPinOne, INPUT_PULLUP);
  //pinMode(buttonPinTwo, INPUT_PULLUP);
  pinMode(14, OUTPUT); pinMode(11, OUTPUT);
  digitalWrite(14, LOW); digitalWrite(11, LOW);
  Serial.println("try start seesaw");
  ss.begin(0x36);
  sspixel.begin(0x36);
  Serial.println("seesaw started");

  sspixel.show();
   
  // use a pin for the built in encoder switch
  ss.pinMode(24, INPUT_PULLUP);

  
  pinMode(ppin0, INPUT_PULLUP);
  pinMode(ppin1, INPUT_PULLUP);
  pinMode(ppin2, INPUT_PULLUP);

  //using boss as mesh root simplifies EVERYTHING and allows node-to-node timing to work (i think???)
 // mesh.setRoot(true);
  //mesh.setContainsRoot(true);

  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onReceive(&receivedCallback);

  Serial.println(mesh.getNodeId());

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setRotation(2);
  display.display();
  delay(500);
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setFont();
  
  userScheduler.addTask( taskButtonWatch );
  userScheduler.addTask( taskUpdateEnc );
  userScheduler.addTask( taskParameterCheck );
  taskButtonWatch.enable();
  taskUpdateEnc.enable();
  taskParameterCheck.enable();
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
}
