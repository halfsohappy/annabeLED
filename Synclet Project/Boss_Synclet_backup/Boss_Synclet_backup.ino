
#include "painlessMesh.h"
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
char patterns[] = {'s', 'f', 'r', 'o', 'c', 'm', 'u'}; 
int patternPlace = 0;
int numOfPatterns = 7;//IF ADDING PATTERNS, CHANGE numOfPatterns!!!!!



//messages contain a character that reoresents pattern selection, and an integer(s) to modify the pattern
void sendSyncletCommand(int pPlace, uint32_t mycVal, int mybVal){
  String message = "";
  message += patterns[pPlace];
  message += mycVal;
  mesh.sendBroadcast(message);
  
  Serial.println(message);
}

#include "Boss_Encoder.h"
#include "Boss_Screen.h"
#include "Boss_Menu.h"

void receivedCallback( uint32_t from, String &msg ) {
  Serial.println(msg);
}

void newConnectionCallback(uint32_t nodeId) {
Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void setup() {
  Serial.begin(115200);
  
  pinMode(buttonPinOne, INPUT_PULLUP);
  pinMode(buttonPinTwo, INPUT_PULLUP);
  pinMode(buttonPinThree, INPUT_PULLUP);

  enableInterrupts();
  setupEnc();

  //using boss as mesh root simplifies EVERYTHING and allows node-to-node timing to work (i think???)
  mesh.setRoot(true);
  mesh.setContainsRoot(true);

  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onReceive(&receivedCallback);

  Serial.println(mesh.getNodeId());

  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setFont();
  
  userScheduler.addTask( taskButtonWatch );
  userScheduler.addTask( taskPotWatch );
  userScheduler.addTask( taskUpdateEnc );
  taskButtonWatch.enable();
  taskPotWatch.enable();
  taskUpdateEnc.enable();

}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
}
