#include <Arduino.h>
#include <ESCBoard.h>
using namespace ESC;
HardwareSerial Debug(PA3,PA2);

float duty=0.15f;
int direction=1;
bool enabled=false;
uint8_t lastHall=0xFF;

void updateDrive(){
  uint8_t h=Board::hallState();
  int8_t s=Board::hallSector(h);
  if(s<0){Board::shutdown(true);Board::floatAll();return;}
  uint8_t drive=(direction>0)?(uint8_t)s:(uint8_t)((s+3)%6);
  Board::sixStep(drive,duty);
  Board::shutdown(!enabled);
  if(h!=lastHall){lastHall=h;Debug.print("hall=");Debug.print(h,BIN);Debug.print(" sector=");Debug.print(s);Debug.print(" drive=");Debug.println(drive);}
}

void setup(){
  Debug.begin(115200);Debug.setTimeout(30);
  Board::begin(12000,1200);Board::beginHall();Board::calibrateCurrentOffsets(500);Board::shutdown(true);
  Debug.println("P10 SixStep | e/x | d <0..0.8> | r 1 forward / r -1 reverse");
}
void loop(){
  if(Debug.available()){
    char c=Debug.read();
    if(c=='e'){enabled=true;}
    else if(c=='x'){enabled=false;Board::shutdown(true);Board::floatAll();}
    else if(c=='d'){duty=constrain(Debug.parseFloat(),0.0f,0.80f);}
    else if(c=='r'){direction=(Debug.parseInt()<0)?-1:1;}
  }
  updateDrive();
  static uint32_t t=0;if(millis()-t>500){t=millis();auto m=Board::readMeasurements();Debug.print(" Vbus=");Debug.print(m.vbus,1);Debug.print(" Idc=");Debug.println(m.idc,2);}
}
