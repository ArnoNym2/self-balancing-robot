//Here are all static settings stored

#ifndef settings_h
#define settings_h

//Pin definition

#define LED_BUILTIN 2

#define EEPROM_SIZE 256

#define offsetAddress 0  //Address where the offsets are stored in EEPROM

#define pidAddress 30  //Address where the pid coefficients are stored in EEPROM

#define stepsPerRevolution 1600  // Steps needed for one rotation: With 1/8 microstepping its 200*8==1600

#define maxNeededAngle 10  //maximum Angle where controler must work

#define ledBlinkTime 500  //How many ms should the led blink

#define maxNeededTime 330


#define True true  //I'm dumb and probably will use both variants
#define False false



#endif