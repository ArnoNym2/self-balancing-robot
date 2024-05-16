

#define stepsPerRevolution  200*4  // change this to fit the number of steps per revolution



int freq = 80000;



void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);
}

void loop() {
  tone(2, freq);
  freq -= 10;
  Serial.println(freq);
  if (freq<1){
    freq = 20000;
  }
  delay(10);
}
