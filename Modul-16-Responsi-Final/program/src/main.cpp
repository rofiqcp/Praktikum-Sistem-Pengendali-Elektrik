#include <Arduino.h>
#include <ESCBoard.h>
using namespace ESC;
HardwareSerial Debug(PA3,PA2);
const uint32_t buttons[4]={PB0,PB1,PB10,PA11};
void report(){auto m=Board::readMeasurements();Debug.print("SD=");Debug.print(Board::isShutdown());Debug.print(" Hall=");Debug.print(Board::hallState(),BIN);Debug.print(" Vbus=");Debug.print(m.vbus,2);Debug.print(" IA=");Debug.print(m.ia,2);Debug.print(" IB=");Debug.print(m.ib,2);Debug.print(" IDC=");Debug.print(m.idc,2);Debug.print(" BTN=");for(auto p:buttons)Debug.print(digitalRead(p)==LOW?'1':'0');Debug.println();}
void setup(){Debug.begin(115200);for(auto p:buttons)pinMode(p,INPUT_PULLUP);Board::begin(12000,1200);Board::beginHall();Board::calibrateCurrentOffsets(1000);Board::shutdown(true);Debug.println("P16 FINAL SELFTEST | r report | e 50% PWM no-load | x shutdown");}
void loop(){if(Debug.available()){char c=Debug.read();if(c=='r')report();else if(c=='e'){Board::setThreePhaseDuty(.5,.5,.5);Board::shutdown(false);}else if(c=='x'){Board::shutdown(true);Board::floatAll();}}static uint32_t t=0;if(millis()-t>1000){t=millis();report();}}
