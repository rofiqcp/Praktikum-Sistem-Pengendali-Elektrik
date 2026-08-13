#include <Arduino.h>
#include <ESCBoard.h>
using namespace ESC;
HardwareSerial Debug(PA3, PA2);

void printReport() {
  auto m = Board::readMeasurements();
  Debug.print("Vbus="); Debug.print(m.vbus,2);
  Debug.print(" IA="); Debug.print(m.ia,2);
  Debug.print(" IB="); Debug.print(m.ib,2);
  Debug.print(" IDC="); Debug.print(m.idc,2);
  Debug.print(" Thr="); Debug.print(m.throttle,2);
  Debug.print(" Brk="); Debug.print(m.brake,2);
  Debug.print(" Hall="); Debug.println(Board::hallState(), BIN);
}

void setup() {
  Debug.begin(115200); Debug.setTimeout(30);
  Board::begin(12000,1000);
  Board::beginHall();
  Board::calibrateCurrentOffsets(1000);
  Board::shutdown(true);
  Debug.println("P06 BRING-UP | r report | p prepare PWM 25/50/75% | e enable | x shutdown | c recalibrate");
}

void loop() {
  if (Debug.available()) {
    char c=Debug.read();
    if(c=='r') printReport();
    else if(c=='c') { Board::shutdown(true); Board::calibrateCurrentOffsets(1000); Debug.println("current offset calibrated"); }
    else if(c=='p') { Board::shutdown(true); Board::setThreePhaseDuty(0.25f,0.50f,0.75f); Debug.println("PWM prepared; still shutdown"); }
    else if(c=='e') { Board::shutdown(false); Debug.println("GATE ENABLED - scope test only"); }
    else if(c=='x') { Board::shutdown(true); Board::floatAll(); Debug.println("SHUTDOWN"); }
  }
  static uint32_t t=0;
  if(millis()-t>1000){t=millis(); printReport();}
}
