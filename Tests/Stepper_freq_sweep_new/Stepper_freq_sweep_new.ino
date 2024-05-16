// Example sketch to control a stepper motor with A4988 stepper motor driver
// and Arduino without a library.
// More info: https://www.makerguides.com

// Define stepper motor connections and steps per revolution:
#define dirPin 5
#define stepPin 2
#define stepsPerRevolution 200
int duration = 500;
long timer;

void setup() {
  // Declare pins as output:
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  digitalWrite(dirPin, HIGH);
  Serial.begin(9600);
  
}

void loop() {
  // Set the spinning direction clockwise:

  // Spin the stepper motor 1 revolution slowly:

    // These four lines result in 1 step:
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(duration);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(duration);
  

  if (millis() - timer>100){
    timer = millis();
    duration --;
    //digitalWrite(dirPin, !digitalRead(dirPin));
    Serial.println(duration);
    if(duration == 0){
      duration = 500;
    }
  }
}