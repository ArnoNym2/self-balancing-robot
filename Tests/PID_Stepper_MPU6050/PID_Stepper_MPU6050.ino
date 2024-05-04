#include "esp_log.h"

#include <Stepper.h>

#include <MPU6050_light.h>

#include <AutoPID.h>

#include "Wire.h"
//#include "Tone.cpp"

#define Stepper1 25
#define Stepper2 26
#define Stepper3 32
#define Stepper4 33
#define stepsPerRevolution 200

#define interruptIn 4
#define interruptOut 2

//PID Settings
#define OutputMin -90
#define OutputMax 90
#define KP 1
#define KI 0
#define KD 0



double yAngle;
double yAngleTarget;
double setPoint = 0;

AutoPID pidController(&yAngle, &setPoint, &yAngleTarget, OutputMin, OutputMax, KP, KI, KD);

MPU6050 mpu(Wire);

Stepper myStepper(stepsPerRevolution, Stepper1, Stepper2 , Stepper3, Stepper4);


long lastMillies = 0;
long lastMicros = 0;

volatile char direction = 1;

void IRAM_ATTR Ext_INT1_ISR() {
  myStepper.step(abs(direction));
}

void setup() {
  esp_log_level_set("*", ESP_LOG_NONE);
  Serial.begin(9600);
  Wire.begin();
  mpu.begin();

  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  // mpu.upsideDownMounting = true; // uncomment this line if the MPU6050 is mounted upside-down
  mpu.calcOffsets();  // gyro and accelero
  Serial.println("Calculated Offsets:");
  Serial.print("Accel [1]:\nX:\t");
  Serial.print(mpu.getAccXoffset());
  Serial.print("\tY:\t");
  Serial.print(mpu.getAccYoffset());
  Serial.print("\tZ:\t");
  Serial.print(mpu.getAccZoffset());
  Serial.print("\nGyro [deg/s]:\nX:\t");
  Serial.print(mpu.getGyroXoffset());
  Serial.print("\tY:\t");
  Serial.print(mpu.getGyroYoffset());
  Serial.print("\tZ:\t");
  Serial.print(mpu.getGyroZoffset());
  Serial.println();




  pidController.setTimeStep(0);

  attachInterrupt(interruptIn, Ext_INT1_ISR, CHANGE);
  pinMode(interruptOut, OUTPUT);
}

void loop() {


  mpu.update();
  yAngle = mpu.getAngleY();
  pidController.run();
  float dAngle = yAngleTarget - yAngle;
  int stepsNeeded = 0;
  uint64_t freq = 0;
  uint32_t dT = 0;
  dT = micros() - lastMicros;
  lastMicros = micros();
  if (!(dAngle < 360 / stepsPerRevolution)) {
    stepsNeeded = (int)stepsPerRevolution / dAngle;  // Benötigte Schritte, um den Winkel zu erreichen

    freq = 1000000 * stepsNeeded / dT;

    direction = dAngle > 0 ? 1 : -1;
    tone(interruptOut, map(freq,0, 60000, 1, 90));
  }
  else{
    noTone(interruptOut);
  }
  if (millis() - lastMillies > 100) {
    lastMillies = millis();
    Serial.print("Measured:");
    Serial.print(yAngle);
    Serial.print(",Set:");
    Serial.print(yAngleTarget);
    Serial.print(",dAngle:");
    Serial.print(dAngle);
    Serial.print(",stepsNeeded:");
    Serial.print(stepsNeeded);
    Serial.print(",int(dAngle:");
    Serial.print((int)dAngle);
    Serial.print(",dT:");
    Serial.print(dT);
    Serial.print(",freq:");
    Serial.println(freq);
  }
  delay(30);
}
