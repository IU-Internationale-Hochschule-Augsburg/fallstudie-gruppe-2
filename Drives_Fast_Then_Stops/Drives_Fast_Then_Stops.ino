#include <A4990MotorShield.h>

/*
 * In this example the Robot drives up to a specific speed and then stops
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
    
    if (speed > 50050){
      motors.setSpeeds(0, 0);
    } else {
      motors.setM1Speed(speed);
      motors.setM2Speed(speed);
      stopIfFault();
    }
    
  }
  
  
}