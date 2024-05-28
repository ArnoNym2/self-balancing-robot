void SerialReader(void *parameter) {
  printHelp();
  pidCoefficients _value = pidValue;
  for (;;) {
    if (Serial.available()) {
      String SerialData = Serial.readString();
      SerialData.toLowerCase();
      switch (SerialData.charAt(0)) {
        case 's':         
          printPidCoefficients(_value);
          newPidValues = true;
          pidValue = _value;
          Serial.println("New values saved");
          break;
        case 'p':
          SerialData.remove(0, 2);  //Remove the first three characters
          _value.Kp = SerialData.toDouble();
          printPid(_value);
          break;
        case 'i':
          SerialData.remove(0, 2);
          _value.Ki = SerialData.toDouble();
          printPid(_value);
          break;
        case 'd':
          SerialData.remove(0, 2);
          _value.Kd = SerialData.toDouble();
          printPid(_value);
          break;
        default:
          printHelp();
          break;
      }
    }
    vTaskDelay(1);
  }
}
void printHelp() {
  Serial.println("Set the pid values like this:");
  Serial.println("p:1.2");
  Serial.println("Available coefficients: p, i, d");
  Serial.println("Save with s");
  Serial.println("Note that only 2 decimal places are saved");
}
void printPid(pidCoefficients _value) {
  Serial.print("Kp: ");
  Serial.println(_value.Kp);
  Serial.print("Ki: ");
  Serial.println(_value.Ki);
  Serial.print("Kd: ");
  Serial.println(_value.Kd);
}