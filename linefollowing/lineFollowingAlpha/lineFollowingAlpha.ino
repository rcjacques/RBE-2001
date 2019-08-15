#include <Servo.h>

#define ON HIGH
#define OFF LOW

Servo left;
Servo right;
int leftStop = 90;
int rightStop = 90;
const int rightMotorPin = 6;
const int leftMotorPin = 7;
const int lightPinLeftMiddle = 22;
const int lightPinRightMiddle = 23;
const int lightPinLeftSide = 24;
const int lightPinRightSide = 25;
const int LEDLeft = 26;
const int LEDRight = 27;
int LMState = 0;
int RMState = 0;
int LSState = 0;
int RSState = 0;
int driveSpeed = 40;
int turnSpeed = 20;

void setup(){
  left.attach(leftMotorPin,1000,2000);
  right.attach(rightMotorPin,1000,2000);
  left.write(leftStop);
  right.write(rightStop);
  
  pinMode(lightPinLeftMiddle,INPUT);
  pinMode(lightPinRightMiddle,INPUT);
  pinMode(lightPinLeftSide,INPUT);
  pinMode(lightPinRightSide,INPUT);
  
  pinMode(LEDLeft,OUTPUT);
  pinMode(LEDRight,OUTPUT);
}

void loop(){
  LMState = digitalRead(lightPinLeftMiddle);
  RMState = digitalRead(lightPinRightMiddle);
  LSState = digitalRead(lightPinLeftSide);
  RSState = digitalRead(lightPinRightSide);
  
  //assuming HIGH is off and LOW is on
  if (LMState == OFF && RMState == OFF){ //no sensors active
    left.write(leftStop-driveSpeed);
    right.write(rightStop+driveSpeed);
    digitalWrite(LEDLeft,HIGH);
    digitalWrite(LEDRight,HIGH);
  }
  else if (LMState == ON && RMState == OFF){ //left sensor active
    left.write(leftStop+turnSpeed);
    right.write(rightStop+turnSpeed);
    digitalWrite(LEDLeft,LOW);
    digitalWrite(LEDRight,HIGH);
  }
  else if (LMState == OFF && RMState == ON){ //right sensor active
    left.write(leftStop-turnSpeed);
    right.write(rightStop-turnSpeed);
    digitalWrite(LEDLeft,HIGH);
    digitalWrite(LEDRight,LOW);
  }
  else if (LMState == ON && RMState == ON){ //both sensors active
    left.write(leftStop);
    right.write(rightStop);
    digitalWrite(LEDLeft,LOW);
    digitalWrite(LEDRight,LOW);
  }
  else{ //if something goes wrong
    left.write(leftStop);
    right.write(rightStop);
    digitalWrite(LEDLeft,HIGH);
    digitalWrite(LEDRight,HIGH);
  }
}
