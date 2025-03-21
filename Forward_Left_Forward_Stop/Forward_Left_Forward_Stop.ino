//This example drives the Robot forward, turns left, again forward and then stops

#include <A4990MotorShield.h>

A4990MotorShield motors;

void setup() {

  motors.setSpeeds(0,0);
  //forwards
  for(int i = 0; i<=50000; i++){
    motors.setM1Speed(1000);
    motors.setM2Speed(1000);
  }
  //turn left
  for(int i = 0; i<=10000; i++){
    motors.setM1Speed(1000);
    motors.setM2Speed(0);
  }
  //forwards
  for(int i = 0; i<=50000; i++){
    motors.setM1Speed(1000);
    motors.setM2Speed(1000);
  }
  motors.setSpeeds(0,0);
}

void loop() {

}

