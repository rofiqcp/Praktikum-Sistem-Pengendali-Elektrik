#include <Arduino.h>
#include <ESCBoard.h>
using namespace ESC;
HardwareSerial Debug(PA3,PA2);

// WAJIB disesuaikan dengan motor/encoder praktikum.
constexpr float ENCODER_PPR=600.0f;
constexpr float COUNTS_PER_REV=ENCODER_PPR*4.0f;
constexpr int POLE_PAIRS=7;

PIController idPI(0.8f,80.0f,3.0f),iqPI(0.8f,80.0f,3.0f);
PIController speedPI(0.015f,0.20f,2.0f);
float kpPos=80.0f;

bool aligned=false,runEnabled=false,positionMode=false;
float speedRef=0,positionRef=0,iqRef=0,idRef=0,rpmFilt=0;
uint32_t lastUs=0,lastSpeedMs=0,lastPosMs=0;

void alignRotor(){
  Board::shutdown(false);
  uint32_t start=millis();
  while(millis()-start<1200){
    float vbus=Board::readVbus();if(vbus<6){Board::shutdown(true);Debug.println("VBUS too low");return;}
    float da,db,dc;svpwm(1.0f,0.0f,vbus,da,db,dc);Board::setThreePhaseDuty(da,db,dc);delay(1);
  }
  Board::shutdown(true);Board::floatAll();delay(200);Board::zeroEncoder();aligned=true;Debug.println("ALIGN OK: encoder zero = electrical angle 0");
}

void stopAll(){runEnabled=false;Board::shutdown(true);Board::floatAll();idPI.reset();iqPI.reset();speedPI.reset();iqRef=0;}

void setup(){
 Debug.begin(115200);Debug.setTimeout(30);Board::begin(12000,1200);Board::calibrateCurrentOffsets(1000);Board::beginEncoder();Board::shutdown(true);
 Debug.print("P15 encoder FOC | PPR=");Debug.print(ENCODER_PPR);Debug.print(" pole_pairs=");Debug.println(POLE_PAIRS);
 Debug.println("a align | e enable | x stop | v speed-mode | p position-mode | s <rpm> | g <turns>");lastUs=micros();
}

void loop(){
 if(Debug.available()){
  char c=Debug.read();
  if(c=='a'){stopAll();alignRotor();}
  else if(c=='e'){if(aligned)runEnabled=true;else Debug.println("align first: a");}
  else if(c=='x')stopAll();
  else if(c=='v'){positionMode=false;}
  else if(c=='p'){positionMode=true;positionRef=(float)Board::encoderPosition()/COUNTS_PER_REV;}
  else if(c=='s'){speedRef=constrain(Debug.parseFloat(),-300.0f,300.0f);}
  else if(c=='g'){positionRef=Debug.parseFloat();positionMode=true;}
 }
 uint32_t now=micros();float dt=(now-lastUs)*1e-6f;if(dt<0.0002f)return;lastUs=now;
 int32_t delta=Board::sampleEncoder();float rpm=(float)delta/COUNTS_PER_REV/dt*60.0f;rpmFilt+=0.05f*(rpm-rpmFilt);
 float turns=(float)Board::encoderPosition()/COUNTS_PER_REV;
 if(millis()-lastPosMs>=20){lastPosMs=millis();if(positionMode){float e=positionRef-turns;speedRef=constrain(kpPos*e,-200.0f,200.0f);}}
 if(millis()-lastSpeedMs>=5){lastSpeedMs=millis();iqRef=speedPI.update(speedRef-rpmFilt,0.005f);}
 if(!runEnabled || !aligned){Board::shutdown(true);return;}
 float vbus=Board::readVbus();if(vbus<6.0f){stopAll();return;}
 auto m=Board::readMeasurements();float alpha,beta,id,iq;clarke(m.ia,m.ib,alpha,beta);
 float mechAngle=turns*2.0f*PI;float theta=wrap2pi(mechAngle*(float)POLE_PAIRS);park(alpha,beta,theta,id,iq);
 float vlim=0.25f*vbus;idPI.limit=vlim;iqPI.limit=vlim;float vd=idPI.update(idRef-id,dt),vq=iqPI.update(iqRef-iq,dt);
 float va,vb,da,db,dc;invPark(vd,vq,theta,va,vb);svpwm(va,vb,vbus,da,db,dc);Board::setThreePhaseDuty(da,db,dc);Board::shutdown(false);
 static uint32_t t=0;if(millis()-t>200){t=millis();Debug.print("turn=");Debug.print(turns,3);Debug.print(" rpm=");Debug.print(rpmFilt,1);Debug.print(" ref=");Debug.print(speedRef,1);Debug.print(" iqRef=");Debug.print(iqRef,2);Debug.print(" id=");Debug.print(id,2);Debug.print(" iq=");Debug.println(iq,2);}
}
