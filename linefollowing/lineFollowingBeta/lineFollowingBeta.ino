#include <Servo.h>
#include <LiquidCrystal.h>

/*
lineFollowingBeta
Author: Rene Jacques
Last Updated: 2/10/15

Purpose:
RBE 2001 Final Project Line Following Autonomous

Description:
Updated version of lineFollowingAlpha. This version counts the number of perpendicular lines that it
encounters, giving it a fair idea of where it is on the field. It can turn based on which line is is 
told to turn at.

Issues:
This is working as expected. Next step is adding bluetooth input and decision logic.
*/

#define ON HIGH //black
#define OFF LOW //white

//LCD pins:
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

//Motor Declaration:
Servo left;
Servo right;

//Speed Declaration:
int leftStop = 90;
int rightStop = 90;
int driveSpeed = 40;
int turnSpeed = 30;

//Pin Declaration:
const int rightMotorPin = 5;
const int leftMotorPin = 6;
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
int turnPoint[] = {3,1};
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
//Manages variable output to LCD
void manageLCDOutput(){
  lcd.setCursor(0,0);
  lcd.print(String(lineCount)+", "+String(LMState)+", "+String(RMState)+"                  ");
  lcd.setCursor(0,1);
  lcd.print(String(LSState)+", "+String(RSState)+"            ");
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
//Checks where the robot is supposed to turn, and sets booleans
//based on when that turn is reached for left or right side
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
//Keeps track of the number of perpendicular lines encountered
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
    stopMotors();
    if (LMState == RISING && RMState == RISING){
      updateLineCount();
    }
    if (searchLeft){
      while(LSState != ON && RSState != ON){//Wait until side sensors see the line, and then turn
        updateSensors();
        drive(driveSpeed);
      }
      seekLeft();
    }
    else if (searchRight){
      while(LSState != ON && RSState != ON){//Wait until side sensors see the line, and then turn
        updateSensors();
        drive(driveSpeed);
      }
      seekRight(); 
    }
    else{
      drive(-driveSpeed);//Not sure if this makes sense or is necessary
    }
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
//Searches for a new line to follow on the right
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
