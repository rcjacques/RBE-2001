#include <BluetoothClient.h>
#include <BluetoothMaster.h>
#include <ReactorProtocol.h>
#include <TimerOne.h>
#include <LiquidCrystal.h>

#define ROBOT 1

#define storageLED1 46
#define storageLED2 48
#define storageLED4 50
#define storageLED8 52

#define supplyLED1 47
#define supplyLED2 49
#define supplyLED4 51
#define supplyLED8 53

ReactorProtocol pcol(byte(ROBOT));
BluetoothClient btClient;
BluetoothMaster btMaster;

volatile unsigned char ticks;
volatile unsigned long elapsedTime;
volatile unsigned char ticksSinceHB;
volatile boolean pulseHB;
volatile boolean start;

volatile unsigned char radAlertTicks = 0;
volatile boolean radAlert = false;
boolean carryingSpentRod = false;
boolean carryingNewRod = false;

volatile boolean sendStatus = false;

byte storage;
byte supply;

LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

int stLED1 = 0;
int stLED2 = 0;
int stLED4 = 0;
int stLED8 = 0;
int spLED1 = 0;
int spLED2 = 0;
int spLED4 = 0;
int spLED8 = 0;

byte pkt[10];

const int buttonPin = 3;
const int startLED = 2;

void setup(){
  Serial.begin(115200);//console comm port
  Serial1.begin(115200);//bluetooth comm port
  
  lcd.begin(16,2);
  
  start = false;//starts as true for the moment
  ticks = 0;
  ticksSinceHB = 0;
  elapsedTime = 0;
  pulseHB = false;
  
  pinMode(startLED,OUTPUT);
  digitalWrite(startLED,LOW);
  
  pinMode(storageLED1,OUTPUT);
  digitalWrite(storageLED1,LOW);
  pinMode(storageLED2,OUTPUT);
  digitalWrite(storageLED2,LOW);
  pinMode(storageLED4,OUTPUT);
  digitalWrite(storageLED4,LOW);
  pinMode(storageLED8,OUTPUT);
  digitalWrite(storageLED8,LOW);
  pinMode(supplyLED1,OUTPUT);
  digitalWrite(supplyLED1,LOW);
  pinMode(supplyLED2,OUTPUT);
  digitalWrite(supplyLED2,LOW);
  pinMode(supplyLED4,OUTPUT);
  digitalWrite(supplyLED4,LOW);
  pinMode(supplyLED8,OUTPUT);
  digitalWrite(supplyLED8,LOW);
  
  pinMode(buttonPin,INPUT_PULLUP);
  attachInterrupt(1,buttonISR,FALLING);
  
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(timerISR);
}

void manageLCDOutput(){
  lcd.setCursor(0,0);
  lcd.print(String(storage));
  lcd.setCursor(0,1);
  lcd.print(String(supply));
}

void buttonISR(void){
  start = true;
  digitalWrite(startLED,HIGH);
}

void timerISR(void){
  ticks++;
  ticksSinceHB++;
  radAlertTicks++;
  if (ticks == 10){
    ticks = 0;
    elapsedTime++;
  }
  if (ticksSinceHB == 20){
    ticksSinceHB = 0;
    pulseHB = true;
    sendStatus = true;
  }
  if (radAlertTicks == 20){
    radAlertTicks = 0;
    radAlert = true;
  }
}

void sendMessage(byte type){
  int sz;
  byte data[3];
  
  pcol.setDst(0x00);//broadcast message
  sz = pcol.createPkt(type,data,pkt);//packet
  btMaster.sendPkt(pkt,sz);//send to field computer
}

void determineLEDs(byte type,int LED){
  if (storage && type){
    LED = 1;
  }
}

void loop(){
  byte data[3];
  byte type;
  
  manageLCDOutput();
  
  //Heartbeat
  if (pulseHB && start){
    pulseHB = false;
    sendMessage(0x07);//heartbeat
  }
  
  //Spent Rod
  if (carryingSpentRod && radAlert && start){
    radAlert = false;
    sendMessage(0x2C);//spend fuel rod
  }
  
  //New Rod
  if (carryingNewRod && radAlert && start){
    radAlert = false;
    sendMessage(0xFF);//new fuel rod
  }
  
  if (sendStatus && start){
    sendStatus = false;
    sendMessage(0x06);//status
  }
 
  if (btMaster.readPacket(pkt)){
    if (pcol.getData(pkt,data,type)){
      switch (type){
        case 0x01:
          storage = data[0];
          break;
        case 0x02:
          supply = data[0];
          break;
        default:
          break;
      }
      
      digitalWrite(storageLED1, storage & 0x01);
      digitalWrite(storageLED2, storage & 0x02);
      digitalWrite(storageLED4, storage & 0x04);
      digitalWrite(storageLED8, storage & 0x08);
      
      digitalWrite(supplyLED1, supply & 0x01);
      digitalWrite(supplyLED2, supply & 0x02);
      digitalWrite(supplyLED4, supply & 0x04);
      digitalWrite(supplyLED8, supply & 0x08);
      
    }
  }
}

