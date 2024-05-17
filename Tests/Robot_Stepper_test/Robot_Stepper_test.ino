#define stepsPerRevolution 200
#define microStepping 8

#define motor1Step 2
#define motor2Step 4
#define motor1Dir 22
#define motor2Dir 23

int freq = 100;

void setup() {
  pinMode(motor1Step, OUTPUT);
  pinMode(motor2Step, OUTPUT);
  pinMode(motor1Dir, OUTPUT);
  pinMode(motor2Dir, OUTPUT);
}

void loop() {
  Serial.print("RPM:");
  Serial.println((float)freq / (stepsPerRevolution * microStepping));
  digitalWrite(motor1Dir, LOW);
  digitalWrite(motor2Dir, LOW);
  tone(motor1Step, freq);
  tone(motor2Step, freq);
  delay(1000);
  digitalWrite(motor1Dir, HIGH);
  digitalWrite(motor2Dir, HIGH);
  delay(2000);
  digitalWrite(motor1Dir, LOW);
  digitalWrite(motor2Dir, LOW);
  delay(1000);
  freq += 50;
  
}
