#include <Stepper.h>



const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution

Stepper myStepper(stepsPerRevolution, 25, 26, 32, 33);

enum sound { bip,
             duDi,
             diDu };


long freq = 0;

void IRAM_ATTR Ext_INT1_ISR() {
  //myStepper.step(direction);
  myStepper.step(1);
}

void playSound(sound melody) {

  switch (melody) {
    case duDi:
      tone(2, 293 *2, 125);
      tone(2, 392 *2, 125);
      delay(250);
      break;
    case diDu:
      tone(2, 392 *2, 125);
      tone(2, 293 *2, 125);
      delay(250);

      break;
    case bip:
      tone(2, 392 *2, 125);
      delay(125);
 
      break;
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(4, INPUT_PULLDOWN);  //Interput pin
  attachInterrupt(4, Ext_INT1_ISR, CHANGE);
  pinMode(2, OUTPUT);
}

void loop() {
  playSound(duDi);
  delay(1000);
  playSound(diDu);
  delay(1000);
  playSound(bip);
  delay(1000);
}
