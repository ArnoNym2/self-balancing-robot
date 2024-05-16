

#define stepsPerRevolution  200*4  // change this to fit the number of steps per revolution



int freq = 20;
int lastMillis = 0;


void setup() {
  Serial.begin(115200);
  pinMode(5, OUTPUT);
  lastMillis = millis();
  do {
   tone(2, freq);
  freq += 438;
  Serial.println(freq);
  delay(1);
  }while (freq<50000);
  int time = millis()-lastMillis;
  Serial.print("Time to accelerate:\t");
  Serial.println(time);
}

void loop() {
 

}
