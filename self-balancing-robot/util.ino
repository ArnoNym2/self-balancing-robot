//utilities that are used for various reasons

void initializeDipSwitch(struct dipSwitch _dipSwitch) {
  pinMode(_dipSwitch.debug, INPUT_PULLUP);
  pinMode(_dipSwitch.offset, INPUT_PULLUP);
  pinMode(_dipSwitch.setPid, INPUT_PULLUP);
  pinMode(_dipSwitch.plotData, INPUT_PULLUP);
}

void initializeStepper(struct stepper _stepper) {
  pinMode(_stepper.directionPin, OUTPUT);
  pinMode(_stepper.stepPin, OUTPUT);
  pinMode(_stepper.enablePin, OUTPUT);
}

void savePidCoefficients(pidCoefficients _pidValue) {
  dprint("Saving pid to Address ");
  dprintln(pidAddress);
  EEPROM.put(pidAddress, _pidValue);
  dprint("size: ");
  dprintln(sizeof(_pidValue));

  EEPROM.commit();  //Needs to be called
}

pidCoefficients readPidCoefficients() {
  pidCoefficients result;
  dprint("Reading pid from Address");
  dprintln(pidAddress);
  EEPROM.get(pidAddress, result);
  dprint("size: ");
  dprintln(sizeof(result));

  return result;
}

void printPidCoefficients(pidCoefficients _pidValue) {
  dprint("Kp:\t");
  dprintln(_pidValue.Kp, 5);
  dprint("Ki:\t");
  dprintln(_pidValue.Ki, 5);
  dprint("Kd:\t");
  dprintln(_pidValue.Kd, 5);
}

void saveOffset(mpuOffset _offset) {
  dprint("Saving offset to Address ");
  dprintln(offsetAddress);
  EEPROM.put(offsetAddress, _offset);
  dprint("size: ");
  dprintln(sizeof(_offset));

  EEPROM.commit();  //Needs to be called
}

mpuOffset readOffset() {
  mpuOffset result;
  dprint("Reading offset from Address");
  dprintln(offsetAddress);
  EEPROM.get(offsetAddress, result);
  dprint("size: ");
  dprintln(sizeof(result));

  return result;
}
void printOffset(mpuOffset _offset) {
  dprint("Gyro x:\t");
  dprintln(_offset.xGyro);
  dprint("Gyro y:\t");
  dprintln(_offset.yGyro);
  dprint("Gyro z:\t");
  dprintln(_offset.zGyro);

  dprint("Accel x:\t");
  dprintln(_offset.xAccel);
  dprint("Accel y:\t");
  dprintln(_offset.yAccel);
  dprint("Accel z:\t");
  dprintln(_offset.zAccel);
}

void errorSound() {
  tone(stepperLeft.stepPin, 392 * 2, 125);
  tone(stepperLeft.stepPin, 293 * 2, 125);
  delay(250);
}

void printGraphs() {  //Prints the values in a way that allows to use Serial Plotter
  if (plotData == true) {

    //dprint("Angle:");
    dprint(Input);
    dprint("\t");
    //dprint("Speed:");
    // dprint(mpu.getGyroY());
    // dprint("\t");
    //dprint("\tAccel:");
    //dprint(mpu.getAccY());
    // dprint("\t");
    //dprint("\tpidAngle:");
    dprint(Output);
    dprint("\t");
    //dprint("\taccelAngle:");
    //dprint(mpu.getAccAngleY());
    //dprint("\t");
    //dprint("\tdAngle:");
    //dprint(dAngle);
    //dprint("\t");
    //dprint("\tduration:");
    // dprint(duration);
    // dprint("\t");
    //dprint("\tmotorRpm:");
    dprint(60. * freq / stepsPerRevolution);
    dprint("\t");
    //dprint("\tstepsNeeded:");
    dprint(stepsNeeded);
    dprint("\t");
    dprint(freq);
    dprint("\t");

    //dprint("\tdirection:");
    dprint(direction);
    dprintln("");
  }
}
