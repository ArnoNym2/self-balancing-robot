#include "settings.h"
#include "Wire.h"
#include <MPU6050_light.h>
#include "dprint.h"
#include <EEPROM.h>
#include <PID_v1.h>


bool debugSetting = true;



double Setpoint, Input, Output = 0;  // double is here the same as float
int lastmicros = 0;

//Variables used in loop
//This should help reduce the loop time as the variables are created at startup and not dynamically
uint32_t duration;
float dAngle;
uint32_t stepsNeeded;
uint64_t freq;
int8_t direction;

struct pidCoefficients {
  double Kp;
  double Ki;
  double Kd;
};

pidCoefficients pidValue = {  // Adapt this according to the vehicle
  2,
  0,
  0
};


struct dipSwitch {
  byte debug;
  byte offset;
  byte three;
  byte four;
  byte five;
  byte six;
  byte seven;
  byte eight;
};

struct mpuOffset {
  float xGyro;
  float yGyro;
  float zGyro;
  float xAccel;
  float yAccel;
  float zAccel;
};

struct stepper {
  byte enablePin;
  byte directionPin;
  byte stepPin;
  int8_t direction;
};

const dipSwitch DIP = {
  15,
  4,
  27,
  26,
  25,
  33,
  32,
  35
};



mpuOffset offset = {
  0,
  0,
  0,
  0,
  0,
  0
};

const stepper stepperLeft = {
  13,
  12,
  14,
  1
};


const stepper stepperRight = {
  19,
  18,
  5,
  -1
};


MPU6050 mpu(Wire);
PID pid(&Input, &Output, &Setpoint, pidValue.Kp, pidValue.Ki, pidValue.Kd, DIRECT);


void setup() {
  Serial.begin(115200);
  initializeDipSwitch(DIP);  //set the pin mode for every dip switch
  initializeStepper(stepperLeft);
  initializeStepper(stepperRight);

  EEPROM.begin(EEPROM_SIZE);
  debugSetting = !digitalRead(DIP.debug);  // Read the debug setting from the DIP Switch


  dprintln("Project self balancing robot");
  dprint("Compiled: ");
  dprint(__DATE__);
  dprint("\t");
  dprintln(__TIME__);


  Wire.begin();
  mpu.begin();
  //Read the offset switch
  //If activated, calculate offset, else read from memory
  if (!digitalRead(DIP.offset)) {
    dprintln("Calculating  offset, please don't move");
    delay(1000);  // Wait a sec to let the sensor settle a moment

    mpu.calcOffsets();                   //Calculate the offset
    saveOffset(offset);                  // save the offset
    printOffset(offset);                 //print the offset
    while (!digitalRead(DIP.offset)) {}  //Wait till the switch is set to off
    dprint("Restarting ESP");
    ESP.restart();  // Restart the ESP as we only want to calculate the offset
  } else {
    dprintln("Read offset from flash");
    offset = readOffset();                                         // read the offset from flash
    mpu.setGyroOffsets(offset.xGyro, offset.yGyro, offset.zGyro);  //set the offsets
    mpu.setAccOffsets(offset.xAccel, offset.yAccel, offset.zAccel);
    printOffset(offset);  //print the offset
  }
  pid.SetOutputLimits(-90, 90);  //set the output limit to +/- 90 deg
  pid.SetSampleTime(50);         // let the pid calculate every 50 ms
  pid.SetMode(AUTOMATIC);
}

void loop() {
  mpu.update();
  Input = mpu.getAngleY();

  if (pid.Compute()) {                 // pid calculated new output
    duration = micros() - lastmicros;  // Calculate how long the last loop did take, we assume that the next loop has the same duration
    lastmicros = micros();
    dAngle = Setpoint - Input;

    stepsNeeded = (unsigned int)stepsPerRevolution / dAngle;  // Calculate how many steps are needed to reach pid's target Angle
    freq = 1000000 * stepsNeeded / duration;                  // The frequency needed to reach the given angle in Hz

    direction = dAngle > 0 ? 1 : -1;

    //set the direction by multiplying the direction with the direction bias
    //then write the pin if it is HIGH, else (it should be -1) set it to LOW
    digitalWrite(stepperLeft.directionPin, direction * stepperLeft.direction == 1 ? HIGH : LOW);
    tone(stepperLeft.stepPin, freq);
    digitalWrite(stepperRight.directionPin, direction * stepperRight.direction == 1 ? HIGH : LOW);
    tone(stepperRight.stepPin, freq);

    printGraphs();
  }
}
