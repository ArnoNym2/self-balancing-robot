# self-balancing-robot
A self balancing robot

# Parts
Was testet with this parts:
 - SBC-NodeMCU-ESP32 (Joy-it)
 - MPU6050 (Joy-it)

# Todo
## Necessary
 - [ ] Disable driver below certain angle
 - [ ] Disable driver if angle is smaller than step angle
 - [x] Light up LED if angle is > 10 deg
   - [x] Light it up
   - [x] Hold it on for few seconds
## Helpful
 - [x] Allow monitoring the gyro and accel data
 - [x] Store Parameters in Flash
 - [x] Use DIP switch to select different modes
 - [ ] Play startup sound
 - [ ] Adjust pid values over serial
## Probably needed
 - [ ] Set an angle offset 
## Nice to have
 - [ ] Set Wrap Angle in Library
 - [ ] Twiggle Algorithm finetuning
