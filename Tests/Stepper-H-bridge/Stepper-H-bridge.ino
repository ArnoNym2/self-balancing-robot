
/*
 Stepper Motor Control - one step at a time

 This program drives a unipolar or bipolar stepper motor.
 The motor is attached to digital pins 8 - 11 of the Arduino.

 The motor will step one step at a time, very slowly.  You can use this to
 test that you've got the four wires of your stepper wired to the correct
 pins. If wired correctly, all steps should be in the same direction.

 Use this also to count the number of steps per revolution of your motor,
 if you don't know it.  Then plug that number into the oneRevolution
 example to see if you got it right.

 Created 30 Nov. 2009
 by Tom Igoe

 */

#include <Stepper.h>

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 25, 26, 32, 33);

volatile int stepCount = 0;  // number of steps the motor has taken

void IRAM_ATTR Ext_INT1_ISR() {
  myStepper.step(1);
  
  stepCount++;
}



void setup() {
  // initialize the serial port:
  Serial.begin(9600);
  pinMode(4, INPUT_PULLDOWN);  //Interput pin
  attachInterrupt(4, Ext_INT1_ISR, CHANGE);

  pinMode(2, OUTPUT);
  tone(2, 100);

  pinMode(12, INPUT);
}

void loop() {
  // step one step:

  Serial.print("steps:");
  Serial.println(stepCount);
  tone(2, map(analogRead(12), 0, 4095, 1, 100));

  delay(100);

}
