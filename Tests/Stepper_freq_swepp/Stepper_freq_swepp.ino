#include <Stepper.h>


const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution

Stepper myStepper(stepsPerRevolution, 25, 26, 32, 33);


long freq = 0;

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
  delay(100);
}
