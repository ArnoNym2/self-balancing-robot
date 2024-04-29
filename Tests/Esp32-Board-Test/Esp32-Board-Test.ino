void setup() {
  Serial.begin(115200);
  Serial.println("Project EPS32-Board-Test");
  Serial.print("Compiled: ");
  Serial.print(__DATE__);
  Serial.print("\t");
  Serial.println(__TIME__);

  uint32_t chipId = 0;
  for (int i = 0; i < 17; i = i + 8) {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }

  Serial.printf("ESP32 Chip model = %s Rev %d\n", ESP.getChipModel(), ESP.getChipRevision());
  Serial.printf("This chip has %d cores\n", ESP.getChipCores());
  Serial.print("Chip ID: ");
  Serial.println(chipId);
}

void loop() {
  // put your main code here, to run repeatedly:
}
