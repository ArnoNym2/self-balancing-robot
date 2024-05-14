#include <Stepper.h>


#define stepsPerRevolution = 200;  // change this to fit the number of steps per revolution

Stepper myStepper(stepsPerRevolution, 25, 26, 32, 33);


int freq = 0;

void IRAM_ATTR Ext_INT1_ISR() {
  //myStepper.step(direction);
  myStepper.step(1);
}

void setup() {
  Serial.begin(9600);
  pinMode(4, INPUT_PULLDOWN);  //Interput pin
  attachInterrupt(4, Ext_INT1_ISR, CHANGE);
  pinMode(2, OUTPUT);
}

void loop() {
  tone(2, freq++);
  Serial.println(freq);
  if (freq>(50*stepsPerRevolution){
    freq = 0
  }
  delay(10);
}
