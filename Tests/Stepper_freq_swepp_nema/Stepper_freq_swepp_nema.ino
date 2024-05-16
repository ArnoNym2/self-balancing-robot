

#define stepsPerRevolution  200*4  // change this to fit the number of steps per revolution



int freq = 20;



void setup() {
  Serial.begin(115200);
  pinMode(5, OUTPUT);
}

void loop() {
  tone(2, freq);
  //freq ++;
  freq += 3;
  Serial.println(freq);
  if (freq>(50000)){
    freq = 50000;
  }
  delay(10);
}
