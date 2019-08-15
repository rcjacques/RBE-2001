#include <Servo.h>
#include <LiquidCrystal.h>

/*
finalAlpha
Authors: Rene Jacques, Dom Bozzuto
Last Updated: 2/16/15

Purpose:
RBE 2001 Final Project Line Following Autonomous

Description:
Combination of Dom's state machine and the lineFollowingEpsilon navigation code.

Issues:
None currently.
*/

#define ON HIGH //black
#define OFF LOW //white
#define FWD 1
#define BCK -1

//LCD pins:
LiquidCrystal lcd(3, 12, 10, 9, 8, 7);

//Motor Declaration:
Servo left;
Servo right;
Servo fbmotor;
Servo gripmotor;
Servo screwmotor;

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

//Interrupt Variables:
volatile boolean goCodeFlag = false;

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
const int buttonPin = 3;

enum robotState {
  waitToBegin,
  getRodLocations,
  moveToR1,
  getSpentRod1,
  moveSRod1ToSpent,
  getNewRod1,
  deliverRod1ToR1,
  moveToR2,
  getSpentRod2,
  moveSRod2ToSpent,
  getNewRod2,
  deliverRod2ToR2,
  celebrate
};

//////////////////////////////////////////////////////////////
void setup(){
  Serial.begin(115200);
  lcd.begin(16,2);
  
  left.attach(leftMotorPin,1000,2000);
  right.attach(rightMotorPin,1000,2000);
  gripmotor.attach(9);
  screwmotor.attach(8, 1000, 2000);
  fbmotor.attach(7);
  left.write(leftStop);
  right.write(rightStop);
  
  pinMode(lightPinLeftMiddle,INPUT_PULLUP);
  pinMode(lightPinRightMiddle,INPUT_PULLUP);
  pinMode(lightPinLeftSide,INPUT_PULLUP);
  pinMode(lightPinRightSide,INPUT_PULLUP);
  
  pinMode(limitPin,INPUT_PULLUP);
  
  pinMode(buttonPin,INPUT_PULLUP);
  attachInterrupt(1,buttonISR,FALLING);
  
  pinMode(LEDLeft,OUTPUT);
  pinMode(LEDRight,OUTPUT);
}
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
void buttonISR(void){
  goCodeFlag = true;
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
void stopMotors(void){
  left.write(leftStop);
  right.write(rightStop);
}
//////////////////////////////////////////////////////////////

/////////////////////////////////////////
/////////////////////
void updateSensors(){
  LMState = digitalRead(lightPinLeftMiddle);
  RMState = digitalRead(lightPinRightMiddle);
  LSState = digitalRead(lightPinLeftSide);
  RSState = digitalRead(lightPinRightSide);
}
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
void basicLineFollow(int dir){
  updateSensors();
  if (LMState == OFF && RMState == OFF){ //no sensors active
    drive(dir*driveSpeed);
  }
  else if (LMState == ON && RMState == OFF){ //left sensor active
    turn(dir*turnSpeed);
  }
  else if (LMState == OFF && RMState == ON){ //right sensor active
    turn(dir*(-turnSpeed));
  }
  else if (LMState == ON && RMState == ON){ //both sensors active
    drive(dir*driveSpeed);
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
//    manageLCDOutput();
    updateSensors();
    turn(turnSpeed);
  }
  while (RMState == OFF){
//    manageLCDOutput();
    updateSensors();
    turn(turnSpeed);
  }
}
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
//Searches for a new line to follow on the right
void seekRight(){
  while (RMState == OFF){
//    manageLCDOutput();
    updateSensors();
    turn(-turnSpeed);
  }
  while (LMState == OFF){
//    manageLCDOutput();
    updateSensors();
    turn(-turnSpeed);
  }
}
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
void goUntilDoubleBlack()
{
  updateSensors();
//  manageLCDOutput();
  while((LSState == OFF) && (RSState == OFF))
  {
    basicLineFollow(FWD);
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
//    manageLCDOutput();
    drive(driveSpeed);
    delay(10);
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
robotState robotMode;
boolean goOnFlag = false;
boolean reactorContact = false;
boolean firstTime = true;
void loop()
{
  reactorContact = digitalRead(limitPin);
  switch (robotMode)
  {
    //wait for blueTooth Communications to be established via pushbutton
    case waitToBegin:
    
    if(goCodeFlag)
    {
      robotMode = getRodLocations;
      goOnFlag = true;
    }
    break;
    
    //determine rod locations and remember them
    case getRodLocations:
    
    if(goOnFlag)
    {
      robotMode = moveToR1;
      goOnFlag = false;
    }
    break;
    
    //go to the first reactor
    case moveToR1:
    if(reactorContact == HIGH)
    {
      basicLineFollow(FWD);
    }
    else
    {
      //this is spooky
      drive(0);
      goOnFlag = true;
    }
    
    if(goOnFlag)
    {
      robotMode = getSpentRod1;
      goOnFlag = true;
    }
    break;
    
    //get the rod from the reactor
    case getSpentRod1:
    gripmotor.write(0);
    fbmotor.write(173);
    screwmotor.write(180);//up
    delay(1000);
    screwmotor.write(90);
    gripmotor.write(130);
    delay(1000);
    screwmotor.write(0);
    delay(1000);
    fbmotor.write(90);
    screwmotor.write(90);
    delay(1000);
    goOnFlag = true;
    
    if(goOnFlag)
    {
      robotMode = moveSRod1ToSpent;
      goOnFlag = false;
    }
    break;
    
    //bring the spent rod to an open storage tube
    case moveSRod1ToSpent:
    
    if(firstTime){
      drive(-driveSpeed);
      delay(1500);
      seekLeft();
      drive(0);
      delay(500);
      firstTime = false;
    }
    else{
      for(int i=0;i<turnPoint[0];i++)
      {
        goUntilDoubleBlack();
        goUntilDoubleWhite();
//        manageLCDOutput();
        lineCount++;                                                                          
      }
      
      stopMotors();
      turnManager();
      delay(1000);
      drive(-driveSpeed);
      delay(500);
//      manageLCDOutput();
      if (searchLeft){
        seekLeft();
        updateSensors();
        int time = 0;
        while(time < 500){
          time++;
//          manageLCDOutput();
          basicLineFollow(FWD);
        }
      }
      else if (searchRight){
        seekRight();
        updateSensors();
        int time = 0;
        while(time < 500){
          time++;
//          manageLCDOutput();
          basicLineFollow(FWD);
        }
      }
    }
    
    if(goOnFlag)
    {
      robotMode = getNewRod1;
      goOnFlag = false;
    }
    break;
    
    //move from spent rod depot to get a new rod
    case getNewRod1:
    
    if(goOnFlag)
    {
      robotMode = deliverRod1ToR1;
      goOnFlag = false;
    }
    break;
    
    //bring the fresh rod to reactor 1
    case deliverRod1ToR1:
    
    if(goOnFlag)
    {
      robotMode = moveToR2;
      goOnFlag = false;
    }
    break;
    
    //go to the second reactor
    case moveToR2:
    
    if(goOnFlag)
    {
      robotMode = getSpentRod2;
      goOnFlag = false;
    }
    break;
    
    //get the rod from the second reactor
    case getSpentRod2:
    
    if(goOnFlag)
    {
      robotMode = moveSRod2ToSpent;
      goOnFlag = false;
    }
    break;
    
    //bring the spent rod to an open storage tube
    case moveSRod2ToSpent:
    
    if(goOnFlag)
    {
      robotMode = getNewRod2;
      goOnFlag = false;
    }
    break;
    
    //move from spent rod depot to get a new rod
    case getNewRod2:
    
    if(goOnFlag)
    {
      robotMode = deliverRod2ToR2;
      goOnFlag = false;
    }
    break;
    
    //bring the fresh rod to reactor 2
    case deliverRod2ToR2:
    
    if(goOnFlag)
    {
      robotMode = celebrate;
      goOnFlag = false;
    }
    break;
    
    case celebrate:
    
    /*
                            ___
                        |  ~~--.
                        |%=@%%/
                        |o%%%/
                     __ |%%o/
               _,--~~ | |(_/ ._
            ,/'  m%%%%| |o/ /  `\.
           /' m%%o(_)%| |/ /o%%m `\
         /' %%@=%o%%%o|   /(_)o%%% `\
        /  %o%%%%%=@%%|  /%%o%%@=%%  \
       |  (_)%(_)%%o%%| /%%%=@(_)%%%  |
       | %%o%%%%o%%%(_|/%o%%o%%%%o%%% |
       | %%o%(_)%%%%%o%(_)%%%o%%o%o%% |
       |  (_)%%=@%(_)%o%o%%(_)%o(_)%  |
        \ ~%%o%%%%%o%o%=@%%o%%@%%o%~ /
         \. ~o%%(_)%%%o%(_)%%(_)o~ ,/
           \_ ~o%=@%(_)%o%%(_)%~ _/
             `\_~~o%%%o%%%%%~~_/'
    */
    while(1)
    {}
    break;
  }
  Serial.println(robotMode);
  delay(10);
}
//////////////////////////////////////////////////////////////


    
