#include "settings.h"
#include "Wire.h"
#include <MPU6050_light.h>
#include "dprint.h"

bool debugSetting = true;

MPU6050 mpu(Wire);

void setup() {
  Serial.begin(115200);
  initializeDipSwitch(DIP);  //set the pin mode for every dip switch
  debugSetting = !digitalRead(DIP.debug); // Read the debug setting from the DIP Switch


  //Read the offset switch
  //If activated, calculate offset, else read from memory
  if (!digitalRead(DIP.offset)) {
    dprintln("Calculating  offset, please don't move");
  } else {
    dprintln("Read offset from flash");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}
