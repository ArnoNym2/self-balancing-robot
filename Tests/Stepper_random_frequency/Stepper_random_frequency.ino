

#define stepsPerRevolution  200*4  // change this to fit the number of steps per revolution



int freq = 20;



void setup() {
  Serial.begin(115200);
  pinMode(5, OUTPUT);
  randomSeed(analogRead(34));
}

void loop() {
  tone(2, freq);
  Serial.println(freq);
  freq=random(1,30000);
  
  delay(1000);
}
