#include <LiquidCrystal.h>
#include <TimerOne.h>

/*
LCD Display Timer
Author: Rene Jacques
Last Update: 2/6/15

I/O Ports:
2 - button
3,4,5,6,11,12 - LCD Display

This program creates and manages a timer that is displayed on the LCD display.
Interrupts are used to keep track of the counter that is used to defined the timer,
and to reset it. The interrupts are used in order to avoid any unintentional 
interrupts to the timer that might throw off the timing.
*/

//LCD pins:
LiquidCrystal lcd(12, 11, 6, 5, 4, 3);

//Timing variable:
unsigned long count = 0; //timer variables are derived from count
//Timer Variables:
unsigned long seconds = 0;
unsigned long minutes = 0;
unsigned long hours = 0;
//Display Strings:
String dispS = "";
String dispM = "";
String dispH = "";
//Pin variable:
const int buttonPin = 2;

/*
Setup initializes the LCD, the button, Timer1, 
and attaches the interrupts
*/
void setup() {
  //LCD setup:
  lcd.begin(16, 2);
  lcd.print("Elapsed Time:");
  
  //button setup:
  pinMode(buttonPin,INPUT_PULLUP);
  attachInterrupt(0,buttonISR,FALLING);
  
  //timer setup:
  Timer1.initialize(100000);
  Timer1.attachInterrupt(timerISR);
}
/*
Loop conatains all timing calculations and display functions
*/
void loop() {
  lcd.setCursor(0, 1);//sets cursor on second line of LCD display
  
  //timer calculations:
  seconds = count/10;//converts count value into seconds
  minutes = (seconds/60)%60;//converts seconds value into minutes and controls minutes reset to 0 on 60
  seconds = seconds%60;//controls seconds reset to 0 on 60 (cannot be done before minutes declaration or timer breaks)
  hours = minutes/60;//converts minutes into hours
  
  //if seconds < 10, then the seconds value needs to be 0 padded
  if (seconds < 10){
    dispS = "0" + String(seconds);
  }
  else{
    dispS = String(seconds);
  }
  
  //if minutes < 10, then the minutes value needs to be 0 padded
  if (minutes < 10){
    dispM = "0" + String(minutes);
  }
  else{
    dispM = String(minutes);
  }
  
  //if hours < 10, then the hours value needs to be 0 padded
  if (hours < 10){
    dispH = "0" + String(hours);
  }
  else{
    dispH = String(hours);
  }
  
  lcd.print(dispH+":"+dispM+":"+dispS);//displays hours, minutes, and seconds
}

/*
Timer interrupt function
*/
void timerISR(){
  count++;
}

/*
Button interrupt function
(resets timer to 0)
*/
void buttonISR(){
  count = 0;
}
