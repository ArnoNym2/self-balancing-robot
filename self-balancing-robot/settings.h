//Here are all static settings stored

#ifndef settings_h
#define settings_h

//Pin definition

#define LED_BUILTIN 2

struct dipSwitch {
  byte debug;
  byte offset;
  byte three;
  byte four;
  byte five;
  byte six;
  byte seven;
  byte eight;
};

const dipSwitch DIP = {
  15,
  4,
  27,
  26,
  25,
  33,
  32,
  35
};

struct stepper {
  byte enablePin;
  byte directionPin;
  byte stepPin;
  int8_t direction;
};

const stepper stepperLeft = {
  13,
  12,
  14,
  1
};


const stepper stepperRight = {
  19,
  18,
  5,
  -1
};



#endif