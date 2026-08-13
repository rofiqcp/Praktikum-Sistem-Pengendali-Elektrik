#include <Arduino.h>
#include <ESCBoard.h>
using namespace ESC;
HardwareSerial Debug(PA3,PA2);
float fElec=2.0f,mod=0.20f,theta=0;uint32_t carrier=12000,deadNs=1200,lastUs=0;bool enabled=false;
void reconfig(){Board::shutdown(true);enabled=false;Board::setPwm(carrier,deadNs);}
void setup(){Debug.begin(115200);Debug.setTimeout(30);Board::begin(carrier,deadNs);Board::shutdown(true);Debug.println("P11 sine | e/x | f <elec Hz> | m <0..0.8> | c <carrier> | t <dead ns>");lastUs=micros();}
void loop(){
 if(Debug.available()){char c=Debug.read();if(c=='e'){enabled=true;}else if(c=='x'){enabled=false;Board::shutdown(true);Board::floatAll();}else if(c=='f'){fElec=constrain(Debug.parseFloat(),-100.0f,100.0f);}else if(c=='m'){mod=constrain(Debug.parseFloat(),0.0f,0.8f);}else if(c=='c'){carrier=constrain((uint32_t)Debug.parseInt(),4000UL,25000UL);reconfig();}else if(c=='t'){deadNs=constrain((uint32_t)Debug.parseInt(),500UL,5000UL);reconfig();}}
 uint32_t n=micros();float dt=(n-lastUs)*1e-6f;if(dt>=0.0001f){lastUs=n;theta=wrap2pi(theta+2*PI*fElec*dt);float da=.5f+.5f*mod*sinf(theta);float db=.5f+.5f*mod*sinf(theta-2*PI/3);float dc=.5f+.5f*mod*sinf(theta+2*PI/3);Board::setThreePhaseDuty(da,db,dc);Board::shutdown(!enabled);}
}
