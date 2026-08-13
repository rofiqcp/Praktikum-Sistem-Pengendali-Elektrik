#include <Arduino.h>
#include <ESCBoard.h>
using namespace ESC;
HardwareSerial Debug(PA3, PA2);

float command = 0.0f;
uint32_t pwmHz = 12000;
uint32_t deadNs = 1000;
bool enabled = false;

void apply() {
  Board::setHBridge(command);
  Board::shutdown(!enabled);
}

void setup() {
  Debug.begin(115200);
  Debug.setTimeout(30);
  Board::begin(pwmHz, deadNs);
  Board::calibrateCurrentOffsets(500);
  Board::setHBridge(0);
  Board::shutdown(true);
  Debug.println("P05 DC H-Bridge Rev.E | e enable | x shutdown | d <(-1..1)> | f <Hz>");
}

void loop() {
  if (Debug.available()) {
    char c = Debug.read();
    if (c == 'e') { enabled = true; apply(); }
    else if (c == 'x') { enabled = false; command = 0; Board::shutdown(true); Board::floatAll(); }
    else if (c == 'd') { command = constrain(Debug.parseFloat(), -1.0f, 1.0f); apply(); }
    else if (c == 'f') {
      uint32_t f = (uint32_t)Debug.parseInt();
      enabled = false; Board::shutdown(true);
      pwmHz = constrain(f, 1000UL, 30000UL);
      Board::setPwm(pwmHz, deadNs); Board::setHBridge(command);
    }
  }
  static uint32_t t = 0;
  if (millis() - t >= 500) {
    t = millis();
    auto m = Board::readMeasurements();
    Debug.print("cmd="); Debug.print(command,3);
    Debug.print(" pwm="); Debug.print(pwmHz);
    Debug.print(" Vbus="); Debug.print(m.vbus,2);
    Debug.print(" Idc="); Debug.print(m.idc,2);
    Debug.print(" SD="); Debug.println(Board::isShutdown());
  }
}
