#include <Arduino.h>
#include <ESCBoard.h>
using namespace ESC;
HardwareSerial Debug(PA3,PA2);
float fElec=2.0f,mod=0.25f,theta=0;bool enabled=false;uint32_t lastUs=0;
void setup(){Debug.begin(115200);Debug.setTimeout(30);Board::begin(12000,1200);Board::calibrateCurrentOffsets(500);Board::shutdown(true);Debug.println("P12 SVPWM | e/x | f <elec Hz> | m <0..0.9>");lastUs=micros();}
void loop(){
 if(Debug.available()){char c=Debug.read();if(c=='e')enabled=true;else if(c=='x'){enabled=false;Board::shutdown(true);Board::floatAll();}else if(c=='f')fElec=constrain(Debug.parseFloat(),-100.0f,100.0f);else if(c=='m')mod=constrain(Debug.parseFloat(),0.0f,0.9f);}
 uint32_t n=micros();float dt=(n-lastUs)*1e-6f;if(dt>=0.0001f){lastUs=n;theta=wrap2pi(theta+2*PI*fElec*dt);float vbus=Board::readVbus();if(vbus<6)vbus=12;float vref=mod*0.55f*vbus;float alpha=vref*cosf(theta),beta=vref*sinf(theta),da,db,dc;svpwm(alpha,beta,vbus,da,db,dc);Board::setThreePhaseDuty(da,db,dc);Board::shutdown(!enabled);}
 static uint32_t t=0;if(millis()-t>500){t=millis();Debug.print("Vbus=");Debug.print(Board::readVbus(),1);Debug.print(" f=");Debug.print(fElec);Debug.print(" M=");Debug.println(mod,2);}
}
