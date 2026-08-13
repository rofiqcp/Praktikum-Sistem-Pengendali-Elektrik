#include <Arduino.h>
#include <ESCBoard.h>
using namespace ESC;
HardwareSerial Debug(PA3,PA2);

PIController idPI(0.8f,80.0f,3.0f), iqPI(0.8f,80.0f,3.0f);
float idRef=0.0f, iqRef=0.3f, angleOffset=0.0f;
bool enabled=false;uint32_t lastUs=0;

void setup(){
 Debug.begin(115200);Debug.setTimeout(30);Board::begin(12000,1200);Board::calibrateCurrentOffsets(1000);Board::beginHall();Board::shutdown(true);
 Debug.println("P14 Hall-FOC | e/x | q <A> | d <A> | o <angle_deg>");lastUs=micros();
}
void loop(){
 if(Debug.available()){char c=Debug.read();if(c=='e')enabled=true;else if(c=='x'){enabled=false;Board::shutdown(true);Board::floatAll();idPI.reset();iqPI.reset();}else if(c=='q')iqRef=constrain(Debug.parseFloat(),-2.0f,2.0f);else if(c=='d')idRef=constrain(Debug.parseFloat(),-1.0f,1.0f);else if(c=='o')angleOffset=Debug.parseFloat()*PI/180.0f;}
 uint32_t now=micros();float dt=(now-lastUs)*1e-6f;if(dt<0.0002f)return;lastUs=now;
 uint8_t hall=Board::hallState();int8_t sector=Board::hallSector(hall);float vbus=Board::readVbus();
 if(!enabled || sector<0 || vbus<6.0f){Board::shutdown(true);return;}
 auto m=Board::readMeasurements();float alpha,beta,id,iq;clarke(m.ia,m.ib,alpha,beta);
 float theta=wrap2pi(((float)sector+0.5f)*PI/3.0f+angleOffset);park(alpha,beta,theta,id,iq);
 float vlim=0.25f*vbus;idPI.limit=vlim;iqPI.limit=vlim;float vd=idPI.update(idRef-id,dt),vq=iqPI.update(iqRef-iq,dt);
 float va,vb,da,db,dc;invPark(vd,vq,theta,va,vb);svpwm(va,vb,vbus,da,db,dc);Board::setThreePhaseDuty(da,db,dc);Board::shutdown(false);
 static uint32_t t=0;if(millis()-t>200){t=millis();Debug.print("hall=");Debug.print(hall,BIN);Debug.print(" id=");Debug.print(id,2);Debug.print(" iq=");Debug.print(iq,2);Debug.print(" vd=");Debug.print(vd,2);Debug.print(" vq=");Debug.println(vq,2);}
}
