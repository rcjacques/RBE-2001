#include <Servo.h>
#include <LiquidCrystal.h>

#define ON HIGH //black
#define OFF LOW //white

//LCD pins:
LiquidCrystal lcd(12, 13, 10, 9, 8, 7);

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

void updateSensors(){
  LMState = digitalRead(lightPinLeftMiddle);
  RMState = digitalRead(lightPinRightMiddle);
  LSState = digitalRead(lightPinLeftSide);
  RSState = digitalRead(lightPinRightSide);
}

void drive(int val){ //Forward(+);Backward(-)
  left.write(leftStop-val);
  right.write(rightStop+val);
}

void goUntilDoubleBlack()
{
  updateSensors();
  while((LSState == OFF) && (RSState == OFF))
  {
  updateSensors();
  drive(driveSpeed);
  delay(10);
  }
}

void goUntilDoubleWhite()
{
  updateSensors();
  while((LSState == ON) || (RSState == ON))
  {
    updateSensors();
    drive(driveSpeed);
    delay(10);
  }
}

void stopMotors(){
  left.write(leftStop);
  right.write(rightStop);
}

void loop()
{
  int i;
  lcd.setCursor(0,0);
  for(i=0;i<3;i++)
  {
    goUntilDoubleBlack();
    goUntilDoubleWhite();
    lcd.print(i);
  }
  stopMotors();
  delay(10000000);
}

void turn(int val){ //Right(-);Left(+)
  left.write(leftStop+val);
  right.write(rightStop+val);
}

  
