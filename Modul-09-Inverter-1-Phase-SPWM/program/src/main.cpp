#include <Arduino.h>
#include <ESCBoard.h>
using namespace ESC;
HardwareSerial Debug(PA3,PA2);

float fOut=50.0f, modulation=0.30f, theta=0;
uint32_t carrier=16000, deadNs=1200, lastUs=0;
bool enabled=false;

void reconfig(){ Board::shutdown(true); enabled=false; Board::setPwm(carrier,deadNs); }

void setup(){
  Debug.begin(115200); Debug.setTimeout(30);
  Board::begin(carrier,deadNs); Board::calibrateCurrentOffsets(500); Board::shutdown(true);
  Debug.println("P09 SPWM | e/x | f <Hz> | m <0..0.9> | c <carrier> | t <dead ns>");
  lastUs=micros();
}

void loop(){
  if(Debug.available()){
    char c=Debug.read();
    if(c=='e'){enabled=true;Board::shutdown(false);}
    else if(c=='x'){enabled=false;Board::shutdown(true);Board::floatAll();}
    else if(c=='f'){fOut=constrain(Debug.parseFloat(),1.0f,100.0f);}
    else if(c=='m'){modulation=constrain(Debug.parseFloat(),0.0f,0.9f);}
    else if(c=='c'){carrier=constrain((uint32_t)Debug.parseInt(),4000UL,25000UL);reconfig();}
    else if(c=='t'){deadNs=constrain((uint32_t)Debug.parseInt(),500UL,5000UL);reconfig();}
  }
  uint32_t now=micros(); float dt=(now-lastUs)*1e-6f;
  if(dt>=0.0001f){
    lastUs=now; theta=wrap2pi(theta+2.0f*PI*fOut*dt);
    Board::setSinglePhaseSPWM(modulation,theta);
    Board::shutdown(!enabled);
  }
  static uint32_t t=0;
  if(millis()-t>500){t=millis();auto m=Board::readMeasurements();Debug.print("f=");Debug.print(fOut);Debug.print(" M=");Debug.print(modulation,2);Debug.print(" Vbus=");Debug.print(m.vbus,1);Debug.print(" Idc=");Debug.println(m.idc,2);}
}
