
#include "painlessMesh.h"

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555
#define _TASK_EXTERNAL_TIME //makes task manager uses nodeTime
painlessMesh  mesh;
uint32_t _task_millis() {return mesh.getNodeTime()/1000;}
uint32_t _task_micros() {return mesh.getNodeTime()/1000;}

uint32_t timeToChange;
bool switchQ = false;

#include <Adafruit_NeoPixel.h>
#define ledNum 4
#define ledPin 20
Adafruit_NeoPixel strip(ledNum, ledPin, NEO_GRB + NEO_KHZ800);
char pattern;


Scheduler userScheduler; // to control your personal task
    
void patternSet();
Task taskPatternSet(100, -1, &patternSet);

void patternFade();
Task taskPatternFade(25, -1, &patternFade);

void patternRainbow();
Task taskPatternRainbow(100, -1, &patternRainbow);

void patternOff();
Task taskPatternOff(100, -1, &patternOff);

void patternCycle();
Task taskPatternCycle(100, -1, &patternCycle);

#include "Synclet_Patterns.h"

void receivedCallback( uint32_t from, String &msg ) {
  mesh.sendSingle(4199520084,"got it!");
  disablePattern();
  
  //update variables to new message
  pattern = msg.charAt(0);
  colorVal = (msg.substring(2)).toInt();

  timeToChange = mesh.getNodeTime() / 1000000;
  timeToChange += 1;

  switchQ = true;
  
  //Serial.println(msg);
  //Serial.println(pattern);
  //Serial.println(colorVal);
}

void newConnectionCallback(uint32_t nodeId) {
//Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void setup() {
  
  //Serial.begin(115200);

  colorVal = 0;
  brightVal = 128;
  strip.begin();
  
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);

  mesh.setContainsRoot(true);

  userScheduler.addTask( taskPatternSet );
  userScheduler.addTask( taskPatternFade );
  userScheduler.addTask( taskPatternRainbow );
  userScheduler.addTask( taskPatternOff );
  userScheduler.addTask( taskPatternCycle );
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
  if((switchQ) && (mesh.getNodeTime() >= (timeToChange * 1000000))) {
      enablePattern();
      switchQ = false;
     }
}
