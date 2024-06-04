#include "settings.h"       //Static settings
#include "Wire.h"           //I2C communication
#include <MPU6050_light.h>  //Sensor reading and angle calculation
#include "dprint.h"         //Custom print functions
#include <EEPROM.h>         //Saving and reading to flash
#include <PID_v1.h>         //PID controller
#include "esp_task_wdt.h"   //Creating esp tasks


bool debugSetting = true;  //Disable serial printing
bool plotData = true;      //Print values over serial to be plottet

// double is here the same as float
double Setpoint, Input, Output = 0;  //Desired setpoint, read input angöe, pid's output
int lastmicros = 0;                  //Used to calculate the loop time

//Global variables used in loop
//This should help reduce the loop time as the variables are created at startup and not dynamically in heap
uint32_t duration;                   //Time in microseconds the loop did take
uint32_t stepsNeeded;                //Number of steps needed  to reach the desired output
uint64_t freq;                       //The needed frequency to reach the angle till next loop
int8_t direction;                    //In which direction do we need to turn
                                     //This is a signed variable which allows it to be mulltiplicated with the steppers installation direction
bool overNeededAngle = false;        //True if the angle is greater than the needed Angle
int overNeededAngleMillis;           //Tracking time to let the led light up for a longer time if we are in the correct range again
bool overMaximumAngle = false;       //True if we are over 90 degree
volatile bool newPidValues = false;  // Used to mark if new pid values came in

//uint32_t jokeTime = 0;
//bool doJoke = false;


struct pidCoefficients {  //struct for storing pid coefficients
  volatile double Kp;
  volatile double Ki;
  volatile double Kd;
};

//Values used at startup
pidCoefficients pidValue = {  // Adapt this according to the vehicle
  1,
  0,
  0
};


struct dipSwitch {  //Struct to store the settings pins in
  byte debug;       //Disable serial printing
  byte offset;      //Calculate mpu offsets
  byte setPid;      //Set pid coefficients over serial
  byte plotData;    //Disable plottable serial printing (reduces spam)
  byte tune;        //WIP Autotune pid coefficients
  byte load;        //Switch pid coefficients based on load
  byte seven;
  byte joke;
};

struct mpuOffset {  //struct to store mpu offsets in

  //Gyroscope sensor offset
  float xGyro;
  float yGyro;
  float zGyro;
  //Acceleometer offsets
  float xAccel;
  float yAccel;
  float zAccel;
};

struct stepper {      //Struct to store stepper dependent settings in
  byte enablePin;     //Driver enable pin
  byte directionPin;  //Driver direction pin
  byte stepPin;       //Driver step pin
  int8_t direction;   //Direction bias (1 or -1) based on the install direction
  bool disable;       // Here can be set wether the enable pin is low or high active (driver dependant)
                      // Should be disabled with digitalWrite(stepper.enablePin, stepper.disable)
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



mpuOffset offset = {  //Initialize offset with zeros
  0,
  0,
  0,
  0,
  0,
  0
};

const stepper stepperLeft = {  //Left stepper
  13,
  12,
  14,
  -1,
  HIGH
};


const stepper stepperRight = {  //Right stepper
  19,
  18,
  5,
  1,
  HIGH
};

//Function declarations
void initializeDipSwitch(struct dipSwitch);  //Initialize dips switch pins
void initializeStepper(struct stepper);      //Initialize stepper pins
void savePidCoefficients(pidCoefficients);   //Save pid coefficients in flash
pidCoefficients readPidCoefficients();       //Read pid coefficients from flash
void printPidCoefficients(pidCoefficients);  //Print pid coefficients over serial
void saveOffset(mpuOffset);                  //Save mpu offsets in flash
mpuOffset readOffset();                      //Read mpu offsets from flash
void printOffset(mpuOffset);                 //Print mpu offsets over serial
void errorSound();                           //Play the error sound with the stepper motor
void printGraphs();                          //Print values formatted to be plotted

void SerialReader(void *parameter);  //Function that runs the parallel task for setting pid coefficients over serial connection
void printHelp();                    //Helper function for the above function


//Constructors for librarys
MPU6050 mpu(Wire);                                                                   //Constructor for mpu sensor
PID pid(&Input, &Output, &Setpoint, pidValue.Kp, pidValue.Ki, pidValue.Kd, DIRECT);  //Constructor for pid controller
//Takes pointers to the measured angle, the pid output and setpoint as well as the coefficients


void setup() {
  Serial.begin(115200);  //Initialize the serial connection
  dprintln("Project self balancing robot");
  dprint("Compiled: ");
  dprint(__DATE__);
  dprint("\t");
  dprintln(__TIME__);

  initializeDipSwitch(DIP);      //Set the pin mode for every dip switch
  pinMode(LED_BUILTIN, OUTPUT);  //Set the pin mode for the led
  initializeStepper(stepperLeft);
  initializeStepper(stepperRight);

  EEPROM.begin(EEPROM_SIZE);  //Initialize the flash storage with a predefined size

  debugSetting = digitalRead(DIP.debug);  //Read the dip switch for debug setting

  plotData = digitalRead(DIP.plotData);  //Read the dip  switch for plotting setting
  dprintln(plotData == true ? "Plotting enabled" : "Plotting disabled");


  Wire.begin();               //Start I2C communication
  byte status = mpu.begin();  //Start the mpu sensor
  if (status != 0) {          //An error occured
    dprint("Error initialising mpu: ");
    dprintln(status);
    tone(LED_BUILTIN, 2);  //Let the led blink
    errorSound();          //Play the error sound
    delay(3000);
    ESP.restart();  //Restart the esp hoping the error will magically fix itself
  }

  //Read the offset switch
  //If activated (logical low), calculate offset, else read from memory
  if (!digitalRead(DIP.offset)) {
    dprintln("Calculating  offset, please don't move");
    delay(1000);  // Wait a sec to let the sensor settle a moment

    mpu.calcOffsets();  //Calculate the offset
    offset.xAccel = mpu.getAccXoffset();
    offset.yAccel = mpu.getAccYoffset();
    offset.zAccel = mpu.getAccZoffset();
    offset.xGyro = mpu.getGyroXoffset();
    offset.yGyro = mpu.getGyroYoffset();
    offset.zGyro = mpu.getGyroZoffset();

    saveOffset(offset);                  //Save the offset to flash
    printOffset(offset);                 //Print the offset to serial
    while (!digitalRead(DIP.offset)) {}  //Wait till the switch is set to off
    dprint("Restarting ESP");
    ESP.restart();  //Restart the ESP as we only want to calculate the offset
  } else {
    dprintln("Read offset from flash");
    offset = readOffset();                                         //Read the offset from flash
    mpu.setGyroOffsets(offset.xGyro, offset.yGyro, offset.zGyro);  //Set the offsets
    mpu.setAccOffsets(offset.xAccel, offset.yAccel, offset.zAccel);
    printOffset(offset);  //print the offset
  }

  /* if (!digitalRead(DIP.setPid)) {  // We want to set the pid values
    pidValue = readPidCoefficients();
    if (isnan(pidValue.Kp) || isnan(pidValue.Ki) || isnan(pidValue.Kd)) {  //manuel set the values if they are not valid
      dprintln("pid values are nan, setting manually");
      pidValue = { 1,
                   0,
                   0 };
      savePidCoefficients(pidValue);
    }
    printPidCoefficients(pidValue);
    xTaskCreatePinnedToCore(
      SerialReader,   // Task function. 
      "SerialReader", // Name of task. 
      10000,          // Stack size of task 
      NULL,           // Parameter of the task 
      2,              // Priority of the task 
      NULL,         // Task handle to keep track of created task 
      0);

  } else {  // We just read them
    dprintln("Reading pid coefficients from flash");
    pidValue = readPidCoefficients();
    if (isnan(pidValue.Kp) || isnan(pidValue.Ki) || isnan(pidValue.Kd)) {  //manuel set the values if they are not valid
      dprintln("pid values are nan, setting manually");
      pidValue = { 1,
                   0,
                   0 };
      savePidCoefficients(pidValue);
    }
    printPidCoefficients(pidValue);
  }*/


  /*if (!digitalRead(DIP.joke)) {  //Do the joke
    doJoke = true;
  }*/


  pid.SetOutputLimits(-50, 50);  //Set output limit to +/- 50 deg
                                 //This is a guesstimate to stay in the steppers higher torque range
  pid.SetSampleTime(50);         //Let the pid calculate every 50 ms
  pid.SetMode(AUTOMATIC);        //Begin the controller
}


void loop() {
  mpu.update();                                                  //Pull new values from sensor and calculate angle
  Input = mpu.getAngleY();                                       //Copy the new angle
  if (abs(Input) > 90) {                                         // The vehicle is laying
    digitalWrite(stepperRight.enablePin, stepperRight.disable);  //Disable the motor driver
    digitalWrite(stepperLeft.enablePin, stepperLeft.disable);
    // pid.SetMode(MANUAL);
    //jokeTime = 0;
    overMaximumAngle = true;
  } else if (abs(Input) < 90 && overMaximumAngle == true) { //We leave the laying position
      overMaximumAngle = false;
      digitalWrite(stepperRight.enablePin, !stepperRight.disable);  //Enable driver again
      digitalWrite(stepperLeft.enablePin, !stepperLeft.disable);
      //pid.SetMode(AUTOMATIC);
    }


  if (pid.Compute()) {  //50ms have passed and the pid controller calculated new values

    duration = micros() - lastmicros;  // Calculate how long the last loop did take, we assume that this loop has the same duration
    lastmicros = micros();

    stepsNeeded = abs((Output / 360) * stepsPerRevolution);  //The needed steps to reach pid's angle
    freq = 1000000 * stepsNeeded / duration;                 // The frequency needed to reach the given angle in Hz
    //This probably needs to be set to a fix value if it changes to much

    direction = Output > 0 ? 1 : -1;  //Check the outputs sign and set direction accordingly


    if (abs(Output) > 360. / stepsPerRevolution) { //Only set the new frequency if we need to move more than one step
      //Left stepper
      digitalWrite(stepperLeft.directionPin, direction * stepperLeft.direction == 1 ? HIGH : LOW);  //set the direction by multiplying the direction with the direction bias
                                                                                                    //then write the pin if it is HIGH, else (it should be -1) set it to LOW
      tone(stepperLeft.stepPin, freq);                                                              //Set a frequency with 50% duty cycle on the step pin

      //Right stepper
      digitalWrite(stepperRight.directionPin, direction * stepperRight.direction == 1 ? HIGH : LOW);
      tone(stepperRight.stepPin, freq);
    }


    //Turn on LED if angle is greater than the maximal needed Angle

    if (abs(Input) > maxNeededAngle && overNeededAngle == false) {  // If its first measurement above the angle then turn on led on
      digitalWrite(LED_BUILTIN, HIGH);
      overNeededAngleMillis = millis();
      overNeededAngle = true;
    } else if (abs(Input) > maxNeededAngle && overNeededAngle == true) {  //If we ares still over the angle just update the timer
      overNeededAngleMillis = millis();
    } else if (abs(Input) < maxNeededAngle && overNeededAngle == true && millis() - overNeededAngleMillis > ledBlinkTime) {  //We are in the right range again
      digitalWrite(LED_BUILTIN, LOW);
      overNeededAngle = false;
    }

    if (newPidValues) {  //We have new values set
      pid.SetTunings(pidValue.Kp, pidValue.Ki, pidValue.Kd);
      printPidCoefficients(pidValue);
      newPidValues = false;
    }

    printGraphs();  //Print the values over serial

    /* if (doJoke == true) {
      if (jokeTime > maxNeededTime * 1000) {
        pid.SetTunings(pid.GetKp() += 0.01, pid.GetKi(), pid.GetKd());
      }
      jokeTime += 50;
    }*/
  }
}
