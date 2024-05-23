#include <A4990MotorShield.h>

/*
 * In this example the Robot accelerates
 */

A4990MotorShield motors;


void stopIfFault()
{
  if (motors.getFault())
  {
    motors.setSpeeds(0,0);
    while(1);
  }
}

void setup()
{


}

void loop()
{
  
  for (int speed = 0; speed >= -1; speed++)
  {
    
    motors.setM1Speed(speed);
    motors.setM2Speed(speed);
    
  }
  
  
}