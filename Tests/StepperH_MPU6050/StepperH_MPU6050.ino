#include <Stepper.h>

#include "Wire.h"
#include <MPU6050_light.h>
#include "Tone.cpp"

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution

Stepper myStepper(stepsPerRevolution, 25, 26, 32, 33);
MPU6050 mpu(Wire);


volatile char direction;
double yAngle;

long lastmillies = 0;

void IRAM_ATTR Ext_INT1_ISR() {
  //myStepper.step(direction);
  myStepper.step(-1);
}

char sgn(float inputNumber) {
  return (inputNumber > 0) - (inputNumber < 0);
}

void setup() {
  Serial.begin(9600);
  pinMode(4, INPUT_PULLDOWN);  //Interput pin
  attachInterrupt(4, Ext_INT1_ISR, CHANGE);
  pinMode(2, OUTPUT);

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
}

void loop() {
  mpu.update();
  if (millis() - lastmillies > 50) {
    lastmillies = millis();
    yAngle = mpu.getAngleY();
    direction = sgn(yAngle);
    Serial.println(direction);
    Serial.println(yAngle);
    tone(2, map(abs(yAngle), 0, 90, 1, 100));
  }
}
