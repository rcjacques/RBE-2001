#include <Servo.h>
#include <LiquidCrystal.h>

/*
lineFollowingEpsilon
Author: Rene Jacques
Last Updated: 2/13/15

Purpose:
RBE 2001 Final Project Line Following Autonomous

Description:
Updated version of lineFollowingDelta. Dom's code works fairly well for determining number of lines crossed.
It might need to be revisited later due to some issues with the side sensors timing. In this code I am going 
to try and implement the turning functions that I had on the first chassis.

Note: I just had the thought that maybe both of the side sensors don't have to be active at the same time
to sense the cross lines. Something to think about.

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
int turnPoint[] = {2,1};
int robotDirection = 1;
int stage = 0;
const int limitPin = 26; //normal HIGH

//////////////////////////////////////////////////////////////
void setup(){
  Serial.begin(115200);
  lcd.begin(16,2);
  
  left.attach(leftMotorPin,1000,2000);
  right.attach(rightMotorPin,1000,2000);
  left.write(leftStop);
  right.write(rightStop);
  
  pinMode(lightPinLeftMiddle,INPUT_PULLUP);
  pinMode(lightPinRightMiddle,INPUT_PULLUP);
  pinMode(lightPinLeftSide,INPUT_PULLUP);
  pinMode(lightPinRightSide,INPUT_PULLUP);
  
  pinMode(limitPin,INPUT_PULLUP);
  
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
  lcd.print(String(lineCount)+", "+String(searchLeft)+", "+String(searchRight));
  lcd.setCursor(0,1);
  lcd.print(String(stage)+", "+String(LSState)+", "+String(RSState));
}
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
void goUntilDoubleBlack()
{
  updateSensors();
  manageLCDOutput();
  while((LSState == OFF) && (RSState == OFF))
  {
    basicLineFollow();
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
void basicLineFollow(){
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
  }
  else{ //if something goes wrong
    stopMotors();
  }
}
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
//Searches for a new line to follow on the left
void seekLeft(){
  while (LMState == OFF){
    manageLCDOutput();
    updateSensors();
    turn(turnSpeed);
  }
  while (RMState == OFF){
    manageLCDOutput();
    updateSensors();
    turn(turnSpeed);
  }
}
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
//Searches for a new line to follow on the right
void seekRight(){
  while (RMState == OFF){
    manageLCDOutput();
    updateSensors();
    turn(-turnSpeed);
  }
  while (LMState == OFF){
    manageLCDOutput();
    updateSensors();
    turn(-turnSpeed);
  }
}
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
//Checks where the robot is supposed to turn, and sets booleans
//based on when that turn is reached for left or right side
void turnManager(){
  if (lineCount == turnPoint[0]){
    if (robotDirection == 1){
      if (turnPoint[1] == 1){
        searchLeft = true;
      }
      else{
        searchRight = true;
      }
    }
    else{
      if (turnPoint[1] == 1){
        searchRight = true;
      }
      else{
        searchLeft = true;
      }
    }
  }
}
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
void loop()
{
//  lcd.setCursor(0,0);
  stage = 1;
  for(int i=0;i<turnPoint[0];i++)
  {
    goUntilDoubleBlack();
    goUntilDoubleWhite();
    manageLCDOutput();
    lineCount++;                                                                          
//    lcd.print(i);
  }
  stopMotors();
  turnManager();
  delay(1000);
  drive(-driveSpeed);
  delay(500);
  stage = 2;
  manageLCDOutput();
  if (searchLeft){
    seekLeft();
    updateSensors();
    stage = 3;
    int time = 0;
    while(time < 500){
      manageLCDOutput();
      basicLineFollow();
    }
  }
  else if (searchRight){
    seekRight();
    updateSensors();
    stage = 4;
    int time = 0;
    while(time < 500){
      time++;
      manageLCDOutput();
      basicLineFollow();
    }
  }
  stage = 5;
  manageLCDOutput();
  stopMotors();
  delay(1000000);
}
//////////////////////////////////////////////////////////////
