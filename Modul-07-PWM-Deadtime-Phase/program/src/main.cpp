#include <Arduino.h>
#include <ESCBoard.h>
using namespace ESC;
HardwareSerial Debug(PA3, PA2);

uint32_t pwmHz=12000, deadNs=1200;
float freqElec=5.0f, spacingDeg=120.0f, mod=0.6f, theta=0;
bool enabled=false;
uint32_t lastUs=0;

void reconfigure(){
  Board::shutdown(true); enabled=false;
  Board::setPwm(pwmHz,deadNs);
  Debug.println("reconfigured; send e to enable");
}

void setup(){
  Debug.begin(115200); Debug.setTimeout(30);
  Board::begin(pwmHz,deadNs); Board::shutdown(true);
  Debug.println("P07 | e/x | f <PWM Hz> | t <dead ns> | q <elec Hz> | p <phase deg> | m <0..0.9>");
  lastUs=micros();
}

void loop(){
  if(Debug.available()){
    char c=Debug.read();
    if(c=='e'){enabled=true; Board::shutdown(false);}
    else if(c=='x'){enabled=false; Board::shutdown(true); Board::floatAll();}
    else if(c=='f'){pwmHz=constrain((uint32_t)Debug.parseInt(),1000UL,30000UL); reconfigure();}
    else if(c=='t'){deadNs=constrain((uint32_t)Debug.parseInt(),300UL,5000UL); reconfigure();}
    else if(c=='q'){freqElec=constrain(Debug.parseFloat(),-100.0f,100.0f);}
    else if(c=='p'){spacingDeg=constrain(Debug.parseFloat(),30.0f,180.0f);}
    else if(c=='m'){mod=constrain(Debug.parseFloat(),0.0f,0.9f);}
  }
  uint32_t now=micros(); float dt=(now-lastUs)*1e-6f;
  if(dt>=0.0001f){
    lastUs=now; theta=wrap2pi(theta+2.0f*PI*freqElec*dt);
    float p=spacingDeg*PI/180.0f;
    float da=0.5f+0.5f*mod*sinf(theta);
    float db=0.5f+0.5f*mod*sinf(theta-p);
    float dc=0.5f+0.5f*mod*sinf(theta+p);
    Board::setThreePhaseDuty(da,db,dc);
    Board::shutdown(!enabled);
  }
}
