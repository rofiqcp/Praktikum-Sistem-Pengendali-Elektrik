#include <Arduino.h>
#include <ESCBoard.h>
using namespace ESC;
HardwareSerial Debug(PA3,PA2);

void setup(){
  Debug.begin(115200);
  Board::begin(12000,1200); Board::beginHall(); Board::calibrateCurrentOffsets(500); Board::shutdown(true);
  Debug.println("RESPONSI P08 diagnostic: default SHUTDOWN. e=test PWM 50%; x=shutdown");
}
void loop(){
  if(Debug.available()){
    char c=Debug.read();
    if(c=='e'){Board::setThreePhaseDuty(.5,.5,.5); Board::shutdown(false);}
    if(c=='x'){Board::shutdown(true); Board::floatAll();}
  }
  static uint32_t t=0;
  if(millis()-t>500){t=millis(); auto m=Board::readMeasurements();
    Debug.print("SD=");Debug.print(Board::isShutdown()); Debug.print(" Hall=");Debug.print(Board::hallState(),BIN);
    Debug.print(" Vbus=");Debug.print(m.vbus,2);Debug.print(" IA=");Debug.print(m.ia,2);Debug.print(" IB=");Debug.print(m.ib,2);Debug.print(" IDC=");Debug.println(m.idc,2);
  }
}
