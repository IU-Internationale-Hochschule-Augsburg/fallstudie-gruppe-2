//This example drives the Robot backwards and then stops

#include <A4990MotorShield.h>

A4990MotorShield motors;

void stopIfFault()
{
  if (motors.getFault())
  {
    motors.setSpeeds(0,0);
    Serial.println("Fault");
    while(1);
  }
}

void setup() {

  motors.setSpeeds(0,0);
  
  for(int i = 0; i<=50000; i++){
    motors.setM1Speed(-1000);
    motors.setM2Speed(-1000);
  }
  motors.setSpeeds(0,0);
}

void loop() {

}
