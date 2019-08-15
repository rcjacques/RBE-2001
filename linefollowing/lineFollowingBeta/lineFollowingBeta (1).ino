#include <Servo.h>
#include <LiquidCrystal.h>

/*
lineFollowingBeta
Author: Rene Jacques
Last Updated: 2/8/15

Purpose:
RBE 2001 Final Project Line Following Autonomous

Description:
Updated version of lineFollowingAlpha. I am attempting to count the number of lines that this robot passes
(lines perpendicular to the master line that the robot is following) as well as get the robot to turn at the 
proper line.

Issues:
I am not sure why this is not working right now. I am going to come back tommorrow with an LCD and use that to
see variables in real time. Hopefully that will shed some light on the matter.
*/

#define ON HIGH //black
#define OFF LOW //white

//LCD pins:
LiquidCrystal lcd(12, 11, 6, 5, 4, 3);

//Motor Declaration:
Servo left;
Servo right;

//Speed Declaration:
int leftStop = 90;
int rightStop = 90;
int driveSpeed = 25;
int turnSpeed = 20;

//Pin Declaration:
const int rightMotorPin = 8;
const int leftMotorPin = 7;
const int lightPinLeftMiddle = 22;
const int lightPinRightMiddle = 23;
const int lightPinLeftSide = 24;
const int lightPinRightSide = 25;
const int LEDLeft = 26;
const int LEDRight = 27;

//Light Sensors Variables:
int LMState = 0;
int RMState = 0;
int LSState = 0;
int RSState = 0;

//Other Variables:
int lineCount = 0;
boolean searchLeft = true;
boolean searchRight = false;
int turnPoint[] = {2,1};
int robotDirection = 1;


//////////////////////////////////////////////////////////////
void setup(){
  lcd.begin(16,2);
  
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
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
void manageLCDOutput(){
  lcd.setCursor(0,0);
  lcd.print(String(lineCount)+", "+String(LMState)+", "+String(RMState));
  lcd.setCursor(0,1);
  lcd.print(String(LSState)+", "+String(RSState));
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
void turnManager(){
  if (lineCount == turnPoint[0]){
    if (robotDirection == 1){
      if (turnPoint[1] == 1){
        searchRight = true;
      }
      else{
        searchLeft = true;
      }
    }
    else{
      if (turnPoint[1] == 1){
        searchLeft = true;
      }
      else{
        searchRight = true;
      }
    }
  }
}
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
void updateLineCount(){
  if (robotDirection == 1){
    lineCount++;
  }
  if (robotDirection == -1){
    lineCount--;
  }
}
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
void loop(){
  turnManager();
  updateSensors();
  manageLCDOutput();
  //assuming HIGH is off and LOW is on
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
    if (LMState == RISING && RMState == RISING){
      updateLineCount();
    }
    if (searchLeft){
      while(LSState != ON && RSState != ON){
        updateSensors();
        drive(driveSpeed);
      }
      seekLeft();
    }
    else if (searchRight){
      while(LSState != ON && RSState != ON){
        updateSensors();
        drive(driveSpeed);,.
      }
      seekRight(); 
    }
    else{
      drive(-driveSpeed);
    }
  }
  else{ //if something goes wrong
    stopMotors();
  }
}
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
void seekLeft(){
  while (LMState == OFF){
    updateSensors();
    turn(turnSpeed);
  }
  while (RMState == OFF){
    updateSensors();
    turn(turnSpeed);
  }
}
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
void seekRight(){
  while (RMState == OFF){
    updateSensors();
    turn(-turnSpeed);
  }
  while (LMState == OFF){
    updateSensors();
    turn(-turnSpeed);
  }
}
//////////////////////////////////////////////////////////////
