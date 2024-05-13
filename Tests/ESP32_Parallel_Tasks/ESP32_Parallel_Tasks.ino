#include <Arduino.h>


#include "Wire.h"
#include <MPU6050_light.h>

uint32_t lastmicros = 0;
volatile uint32_t duration = 0;

MPU6050 mpu(Wire);


void Controller(void *parameter) {
  int lastmillis = 0;
  for (;;) {

    lastmicros = micros();
    mpu.update();
    duration = micros() - lastmicros;

    //Serial.print("Controller:");
    Serial.println(duration);  //Added for comparison
  }
}


void SerialStuff(void *parameter) {
  int lastmillis = 0;

  for (;;) {
    //Serial.print("Serial:");
    Serial.println(duration);
  }
}


void setup() {
  Serial.begin(115200);

  Wire.begin();

  byte status = mpu.begin();

  xTaskCreatePinnedToCore(
    Controller,   /* Task function. */
    "Controller", /* Name of task. */
    10000,        /* Stack size of task */
    NULL,         /* Parameter of the task */
    2,            /* Priority of the task */
    NULL,         /* Task handle to keep track of created task */
    0);


  /*xTaskCreatePinnedToCore(
    SerialStuff,
    "SerialStuff",
    10000,
    NULL,
    1,
    NULL,
    0);*/
}

void loop() {
  // put your main code here, to run repeatedly:
}
