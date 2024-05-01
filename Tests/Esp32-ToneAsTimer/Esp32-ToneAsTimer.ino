#define LED 4
#define Poti 13

unsigned int setNewFreq(uint8_t pin, unsigned int freq) {
  noTone(pin);
  tone(pin, freq);
  return freq;
}


void setup() {
  pinMode(LED, OUTPUT);
  pinMode(Poti, INPUT);
  Serial.begin(115200);
  Serial.println("Project EPS32-ToneAsTimer");
  Serial.print("Compiled: ");
  Serial.print(__DATE__);
  Serial.print("\t");
  Serial.println(__TIME__);
}

void loop() {
  
 Serial.println(setNewFreq(LED, map(analogRead(Poti), 0, 4095, 1, 15)));
 delay(2000);




}
