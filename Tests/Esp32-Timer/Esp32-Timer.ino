#include "ESP32TimerInterrupt.h"

#define LED 4
#define Poti 13
#define checkInterval 1000  // Time in ms after which Counters are printed


#define TIMER0_INTERVAL_S 2  // Zeitintervall in Sekunden

volatile uint32_t Timer0Count = 0;

hw_timer_t* timer = NULL;
//hw_timer_t* watchdogTimer = NULL;
volatile SemaphoreHandle_t timerSemaphore;

void IRAM_ATTR onTimer() {
  xSemaphoreGiveFromISR(timerSemaphore, NULL);
  Timer0Count++;
  digitalWrite(LED, !digitalRead(LED));
}

void ARDUINO_ISR_ATTR resetModule() {
  ets_printf("reboot\n");
  esp_restart();
}



ESP32Timer ITimer0(0);


void setup() {
  pinMode(LED, OUTPUT);
  pinMode(Poti, INPUT);
  Serial.begin(115200);

  Serial.println("Project EPS32-Timer");
  Serial.print("Compiled: ");
  Serial.print(__DATE__);
  Serial.print("\t");
  Serial.println(__TIME__);

  timerSemaphore = xSemaphoreCreateBinary();
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000000, true);

  //watchdogTimer = timerBegin(0, 80, true);
  //timerAttachInterrupt(watchdogTimer, &resetModule, true);   //attach callback
  //timerAlarmWrite(watchdogTimer, 3000 * 1000, false);  //set time in us



  //timerAlarmEnable(watchdogTimer);  //enable interrupt
  timerAlarmEnable(timer);
}

void loop() {
  if (xSemaphoreTake(timerSemaphore, 0) == pdTRUE) {
    Serial.print("Interrupt Nr.\t");
    Serial.println(Timer0Count);
  }
  uint64_t timerValue = map(analogRead(Poti), 0, 4095, 100000, 2000000);
  timerAlarmWrite(timer, timerValue, true);
  timerAlarmEnable(timer);
  Serial.println(timerValue / 1000);

  delay(500);
}
