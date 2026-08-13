#pragma once
#include <Arduino.h>
#include <math.h>

namespace ESC {

struct Measurements {
  float ia;
  float ib;
  float ic;
  float idc;
  float vbus;
  float throttle;
  float brake;
};

class Board {
public:
  static constexpr uint32_t TIMER_CLOCK_HZ = 84000000UL;
  static constexpr float ADC_REF = 3.3f;
  static constexpr float CURRENT_V_PER_A = 0.040f;
  static constexpr float VBUS_RATIO = 21.0f;
  static constexpr float INPUT_DIV_RATIO = 22.0f / 37.0f;

  static void begin(uint32_t pwmHz = 12000, uint32_t deadtimeNs = 1000);
  static void setPwm(uint32_t pwmHz, uint32_t deadtimeNs);
  static void shutdown(bool active);
  static bool isShutdown();

  static void setThreePhaseDuty(float da, float db, float dc);
  static void setHBridge(float command);
  static void setSinglePhaseSPWM(float modulation, float electricalAngle);
  static void floatAll();
  static void sixStep(uint8_t sector, float duty);

  static void beginHall();
  static uint8_t hallState();
  static int8_t hallSector(uint8_t state);

  static void beginEncoder();
  static int32_t sampleEncoder();
  static int32_t encoderPosition();
  static void zeroEncoder();

  static void calibrateCurrentOffsets(uint16_t samples = 1000);
  static Measurements readMeasurements();
  static float readCurrentA();
  static float readCurrentB();
  static float readCurrentDC();
  static float readVbus();
  static float readThrottle();
  static float readBrake();

  static uint32_t pwmFrequency();
  static uint32_t deadtimeNanoseconds();

private:
  static uint32_t _pwmHz;
  static uint32_t _deadtimeNs;
  static float _offsetIA;
  static float _offsetIB;
  static float _offsetIDC;
  static int32_t _encoderPos;
  static uint16_t _encoderLast;

  static float clamp01(float x);
  static float adcVolts(uint32_t pin);
  static uint8_t encodeDeadtime(uint32_t ns);
  static uint32_t dutyToCCR(float d);
  static void configurePwmPins();
  static void setChannelEnable(uint8_t channel, bool mainEnable, bool nEnable);
};

float wrap2pi(float x);
void clarke(float ia, float ib, float &alpha, float &beta);
void park(float alpha, float beta, float theta, float &id, float &iq);
void invPark(float vd, float vq, float theta, float &alpha, float &beta);
void svpwm(float alpha, float beta, float vbus, float &da, float &db, float &dc);

#pragma push_macro("PI")
#undef PI
class PI {
public:
  PI(float kp = 0, float ki = 0, float limit = 1) : kp(kp), ki(ki), limit(limit) {}
  float update(float error, float dt);
  void reset(float value = 0) { integral = value; }
  float kp, ki, limit;
private:
  float integral = 0;
};
#pragma pop_macro("PI")

} // namespace ESC
