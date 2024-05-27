
#include "painlessMesh.h"
//mesh setup
#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555
painlessMesh  mesh;

//declare node IDs
#define nodeA 402915293
#define nodeB 402859677
#define nodeC 402884033
#define nodeD 402849465
#define nodeE 402864161

#define buttonOne 8
#define buttonTwo 7
#define buttonThree 6

//everything needed for scheduling button watching and message sending
bool lastSwitchOneStatus = HIGH;
bool lastSwitchTwoStatus = HIGH;
bool lastSwitchThreeStatus = HIGH;
bool switchPushed = false;

void checkInput();
Scheduler userScheduler;
Task taskCheckInput(50, -1, &checkInput);
int colorVal;

//pattern codes (chars) for broadcast
char patterns[] = {'s', 'f', 'r', 'o', 'c'}; //IF ADDING PATTERNS, CHANGE numOfPatterns!!!!!
//s is "color set", change all hues to receivedNum
//f is "fade", leds fade and brighten on last colorVal
//r is "rainbow", static rainbow fills the strip
//o if "off", fills strip with darkness
//c is "cycle", cycles hues of leds throughout strip
int patternPlace = 0;
int numOfPatterns = 5;

//messages contain a character that reoresents pattern selection, and an integer(s) to modify the pattern
void sendSyncletCommand(int pPlace, int myVal){
  String message = "";
  message += patterns[pPlace];
  message += " ";
  message += myVal;
  
  mesh.sendBroadcast(message);
  
  Serial.println(message);
}

//what do buttons do?
void buttonOnePress() {
      Serial.println("buttonOne Press");
      
      colorVal += 1024;
      
      sendSyncletCommand(patternPlace, colorVal);
}

void buttonTwoPress(){
      Serial.println("buttonTwo Press");
      
      colorVal -= 1024;
      
      sendSyncletCommand(patternPlace, colorVal);
}

void buttonThreePress(){
      Serial.println("buttonThree Press");
      
      patternPlace++;
      if (patternPlace >= numOfPatterns){patternPlace = 0;}
      
      sendSyncletCommand(patternPlace, colorVal);
}

//button watching
void checkInput() {
  byte switchStatus;
  //1st switch
  switchStatus = digitalRead(buttonOne);
  if (lastSwitchOneStatus != switchStatus)//was there a change in switch state?
  {
    lastSwitchOneStatus = switchStatus;//update to the new state
    if (switchStatus == switchPushed)//did the switch get pushed?
    {
      buttonOnePress();
    }
  }
  //2nd switch
  switchStatus = digitalRead(buttonTwo);
  if (lastSwitchTwoStatus != switchStatus)//was there a change in switch state?
  {
   lastSwitchTwoStatus = switchStatus;//update to the new state
   if (switchStatus == switchPushed)//did the switch get pushed?
    {
        buttonTwoPress();
    } 
  }
  //3rd switch
  switchStatus = digitalRead(buttonThree);
  if (lastSwitchThreeStatus != switchStatus)//was there a change in switch state?
  {
   lastSwitchThreeStatus = switchStatus;//update to the new state
   if (switchStatus == switchPushed)//did the switch get pushed?
    {
        buttonThreePress();
    } 
  }
} 

void receivedCallback( uint32_t from, String &msg ) {
  Serial.println(msg);
}

void newConnectionCallback(uint32_t nodeId) {
Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void setup() {
  Serial.begin(115200);
  
  pinMode(buttonOne, INPUT_PULLUP);
  pinMode(buttonTwo, INPUT_PULLUP);
  pinMode(buttonThree, INPUT_PULLUP);

  //using boss as mesh root simplifies EVERYTHING and allows node-to-node timing to work
  mesh.setRoot(true);
  mesh.setContainsRoot(true);

  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onReceive(&receivedCallback);

  Serial.println(mesh.getNodeId());
  
  userScheduler.addTask( taskCheckInput );
  taskCheckInput.enable();

}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
}
