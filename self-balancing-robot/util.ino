//utilities that are used for various reasons

void initializeDipSwitch(struct dipSwitch _dipSwitch){
  pinMode(_dipSwitch.debug, INPUT_PULLUP);
  pinMode(_dipSwitch.offset, INPUT_PULLUP);

}