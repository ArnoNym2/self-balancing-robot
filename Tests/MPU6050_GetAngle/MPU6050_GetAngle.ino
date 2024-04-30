
#include "Wire.h"
#include <MPU6050_light.h>  //does only work if Library is in library folder

MPU6050 mpu(Wire);

unsigned long timer = 0;
uint64_t lastmicros = 0;
uint32_t duration = 0;

void setup() {
  Serial.begin(9600);

  Serial.println("Project MPU6050_GetAngle");
  Serial.print("Compiled: ");
  Serial.print(__DATE__);
  Serial.print("\t");
  Serial.println(__TIME__);

  Wire.begin();

  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while (status != 0) {}  // stop everything if could not connect to MPU6050

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

  lastmicros = micros();
  mpu.update();
  duration = micros() - lastmicros;


  if ((millis() - timer) > 1000) {  // print data every 100ms

    Serial.print("X:");
    Serial.print(mpu.getAngleX());
    Serial.print(",Y:");
    Serial.print(mpu.getAngleY());
    Serial.print(",Z:");
    Serial.print(mpu.getAngleZ());
    Serial.print(",Duration:");
    Serial.println(duration);
    timer = millis();
  }
  //delay(100);// Simulate running time with 100 ms
}
