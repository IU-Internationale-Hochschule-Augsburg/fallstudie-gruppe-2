//This example drives the Robot forward, turns left, again forward and then stops

#include <A4990MotorShield.h>

A4990MotorShield motors;

String command;

void setup() {

  Serial.begin(9600);
  Serial.println("Type Command:");
  motors.setSpeeds(0,0);
  
}

void loop() {
if (Serial.available()) {
    command = Serial.readStringUntil('\n');
    command.trim();
    if (command.equals("forward")) {
      for(int i = 0; i<=50000; i++){
    motors.setM1Speed(1000);
    motors.setM2Speed(1000);
  }
    }
}
}

