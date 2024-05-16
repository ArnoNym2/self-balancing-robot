#include "settings.h"
void setup() {
  initializeDipSwitch(DIP);  //set the pin mode for every dip switch
  if (!digitalRead(DIP.debug)) {
    Serial.begin(115200);  //Start the Serial connection
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
}
