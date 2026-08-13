#include <Arduino.h>
#include <ESCBoard.h>
using namespace ESC;
HardwareSerial Debug(PA3,PA2);
void setup(){Debug.begin(115200);Board::begin(12000,1200);Board::shutdown(true);Board::calibrateCurrentOffsets(1000);Debug.println("time_ms,ia,ib,ic,idc,vbus,throttle,brake");}
void loop(){
 if(Debug.available() && Debug.read()=='c'){Board::shutdown(true);Board::calibrateCurrentOffsets(1000);}
 static uint32_t t=0;if(millis()-t>=100){t=millis();auto m=Board::readMeasurements();Debug.print(t);Debug.print(',');Debug.print(m.ia,4);Debug.print(',');Debug.print(m.ib,4);Debug.print(',');Debug.print(m.ic,4);Debug.print(',');Debug.print(m.idc,4);Debug.print(',');Debug.print(m.vbus,4);Debug.print(',');Debug.print(m.throttle,4);Debug.print(',');Debug.println(m.brake,4);}
}
