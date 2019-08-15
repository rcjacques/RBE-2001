#include <Servo.h>
#include <LiquidCrystal.h>

/*
lineFollowingDelta
Author: Rene Jacques
Last Updated: 2/13/15

Purpose:
RBE 2001 Final Project Line Following Autonomous

Description:
Updated version of lineFollowingGamma. Attempting to fix perpendicular line detection on new chassis.
It is unclear why the old line follower worked on the old chassis, but we know that it doesn't work 
anymore. Dom has come up with new way to track lins. Attempting to implement that into the old basic
line following code.

Issues:
None currently.
*/

#define ON HIGH //black
#define OFF LOW //white

//LCD pins:
LiquidCrystal lcd(3, 12, 10, 9, 8, 7);

//Motor Declaration:
Servo left;
Servo right;

//Speed Declaration:
int leftStop = 90;
int rightStop = 90;
int driveSpeed = 25;
int turnSpeed = 20;

//Pin Declaration:
const int rightMotorPin = 6;
const int leftMotorPin = 5;
const int lightPinLeftMiddle = 24;
const int lightPinRightMiddle = 25;
const int lightPinLeftSide = 22;
const int lightPinRightSide = 23;
const int LEDLeft = 26;
const int LEDRight = 27;

//Light Sensors Variables:
int LMState = 0;
int RMState = 0;
int LSState = 0;
int RSState = 0;

//Other Variables:
int lineCount = 0;
boolean searchLeft = false;
boolean searchRight = false;
int turnPoint[] = {4,1};
int robotDirection = 1;


//////////////////////////////////////////////////////////////
void setup(){
  lcd.begin(16,2);
  
  left.attach(leftMotorPin,1000,2000);
  right.attach(rightMotorPin,1000,2000);
  left.write(leftStop);
  right.write(rightStop);
  
  pinMode(lightPinLeftMiddle,INPUT_PULLUP);
  pinMode(lightPinRightMiddle,INPUT_PULLUP);
  pinMode(lightPinLeftSide,INPUT_PULLUP);
  pinMode(lightPinRightSide,INPUT_PULLUP);
  
  pinMode(LEDLeft,OUTPUT);
  pinMode(LEDRight,OUTPUT);
}
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
void drive(int val){ //Forward(+);Backward(-)
  left.write(leftStop-val);
  right.write(rightStop+val);
}
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
void turn(int val){ //Right(-);Left(+)
  left.write(leftStop+val);
  right.write(rightStop+val);
}
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
void stopMotors(){
  left.write(leftStop);
  right.write(rightStop);
}
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
void updateSensors(){
  LMState = digitalRead(lightPinLeftMiddle);
  RMState = digitalRead(lightPinRightMiddle);
  LSState = digitalRead(lightPinLeftSide);
  RSState = digitalRead(lightPinRightSide);
}
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
//Manages variable output to LCD
void manageLCDOutput(){
  lcd.setCursor(0,0);
  lcd.print(String(lineCount)+", "+String(LMState)+", "+String(RMState));
  lcd.setCursor(0,1);
  lcd.print(String(LSState)+", "+String(RSState));
}
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
void goUntilDoubleBlack()
{
  updateSensors();
  manageLCDOutput();
  while((LSState == OFF) && (RSState == OFF))
  {
    updateSensors();
    if (LMState == OFF && RMState == OFF){ //no sensors active
      drive(driveSpeed);
    }
    else if (LMState == ON && RMState == OFF){ //left sensor active
      turn(turnSpeed);
    }
    else if (LMState == OFF && RMState == ON){ //right sensor active
      turn(-turnSpeed);
    }
    else if (LMState == ON && RMState == ON){ //both sensors active
      drive(driveSpeed);
    }//end if
    else{ //if something goes wrong
      stopMotors();
    }
  }
}
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
void goUntilDoubleWhite()
{
  updateSensors();
  while((LSState == ON) || (RSState == ON))
  {
    updateSensors();
    manageLCDOutput();
    drive(driveSpeed);
    delay(10);
  }
}
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
void loop()
{
//  lcd.setCursor(0,0);
  for(int i=0;i<=turnPoint[0];i++)
  {
    goUntilDoubleBlack();
    goUntilDoubleWhite();
    manageLCDOutput();
//    lcd.print(i);
  }
  stopMotors();
  delay(10000000);
}
//////////////////////////////////////////////////////////////
