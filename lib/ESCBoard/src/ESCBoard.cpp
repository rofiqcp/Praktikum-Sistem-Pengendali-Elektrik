#include "ESCBoard.h"
#include "stm32f4xx_hal.h"

namespace ESC {

constexpr uint32_t Board::TIMER_CLOCK_HZ;
constexpr float Board::ADC_REF;
constexpr float Board::CURRENT_V_PER_A;
constexpr float Board::VBUS_RATIO;
constexpr float Board::INPUT_DIV_RATIO;

uint32_t Board::_pwmHz = 12000;
uint32_t Board::_deadtimeNs = 1000;
float Board::_offsetIA = 1.65f;
float Board::_offsetIB = 1.65f;
float Board::_offsetIDC = 1.65f;
int32_t Board::_encoderPos = 0;
uint16_t Board::_encoderLast = 0;

static const uint32_t PIN_IA = PA0;
static const uint32_t PIN_IB = PA1;
static const uint32_t PIN_IDC = PA4;
static const uint32_t PIN_VBUS = PA5;
static const uint32_t PIN_THROTTLE = PA6;
static const uint32_t PIN_BRAKE = PA7;
static const uint32_t PIN_HALL_A = PB6;
static const uint32_t PIN_HALL_B = PB7;
static const uint32_t PIN_HALL_C = PB2;
static const uint32_t PIN_DRV_SD = PA12;

float Board::clamp01(float x) {
  if (x < 0.0f) return 0.0f;
  if (x > 1.0f) return 1.0f;
  return x;
}

void Board::shutdown(bool active) { digitalWrite(PIN_DRV_SD, active ? HIGH : LOW); }
bool Board::isShutdown() { return digitalRead(PIN_DRV_SD) == HIGH; }

uint8_t Board::encodeDeadtime(uint32_t ns) {
  uint32_t ticks = (uint32_t)(((uint64_t)ns * TIMER_CLOCK_HZ + 999999999ULL) / 1000000000ULL);
  if (ticks <= 127) return (uint8_t)ticks;
  if (ticks <= 254) {
    uint32_t v = (ticks + 1) / 2;
    if (v < 64) v = 64;
    if (v > 127) v = 127;
    return (uint8_t)(0x80U | (v - 64U));
  }
  if (ticks <= 504) {
    uint32_t v = (ticks + 7) / 8;
    if (v < 32) v = 32;
    if (v > 63) v = 63;
    return (uint8_t)(0xC0U | (v - 32U));
  }
  uint32_t v = (ticks + 15) / 16;
  if (v < 32) v = 32;
  if (v > 63) v = 63;
  return (uint8_t)(0xE0U | (v - 32U));
}

void Board::configurePwmPins() {
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  GPIO_InitTypeDef g = {};
  g.Mode = GPIO_MODE_AF_PP;
  g.Pull = GPIO_NOPULL;
  g.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  g.Alternate = GPIO_AF1_TIM1;
  g.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;
  HAL_GPIO_Init(GPIOA, &g);
  g.Pin = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOB, &g);
}

void Board::begin(uint32_t pwmHz, uint32_t deadtimeNs) {
  pinMode(PIN_DRV_SD, OUTPUT);
  shutdown(true);
  analogReadResolution(12);
  configurePwmPins();
  setPwm(pwmHz, deadtimeNs);
  floatAll();
}

void Board::setPwm(uint32_t pwmHz, uint32_t deadtimeNs) {
  if (pwmHz < 1000) pwmHz = 1000;
  if (pwmHz > 30000) pwmHz = 30000;
  _pwmHz = pwmHz;
  _deadtimeNs = deadtimeNs;
  __HAL_RCC_TIM1_CLK_ENABLE();
  TIM1->CR1 = 0;
  TIM1->CR2 = 0;
  TIM1->SMCR = 0;
  TIM1->PSC = 0;
  uint32_t arr = TIMER_CLOCK_HZ / (2UL * pwmHz);
  if (arr < 2) arr = 2;
  TIM1->ARR = arr - 1;
  TIM1->RCR = 0;
  TIM1->CCMR1 = TIM_CCMR1_OC1PE | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 |
                TIM_CCMR1_OC2PE | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;
  TIM1->CCMR2 = TIM_CCMR2_OC3PE | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2;
  TIM1->CCR1 = TIM1->ARR / 2;
  TIM1->CCR2 = TIM1->ARR / 2;
  TIM1->CCR3 = TIM1->ARR / 2;
  TIM1->CCER = 0;
  uint8_t dtg = encodeDeadtime(deadtimeNs);
  TIM1->BDTR = TIM_BDTR_OSSR | TIM_BDTR_OSSI | TIM_BDTR_MOE | dtg;
  TIM1->EGR = TIM_EGR_UG;
  TIM1->CR1 = TIM_CR1_ARPE | TIM_CR1_CMS_0 | TIM_CR1_CEN;
}

uint32_t Board::dutyToCCR(float d) {
  d = clamp01(d);
  return (uint32_t)(d * (float)TIM1->ARR);
}

void Board::setChannelEnable(uint8_t ch, bool mainEnable, bool nEnable) {
  uint32_t me = 0, ne = 0;
  if (ch == 1) { me = TIM_CCER_CC1E; ne = TIM_CCER_CC1NE; }
  else if (ch == 2) { me = TIM_CCER_CC2E; ne = TIM_CCER_CC2NE; }
  else { me = TIM_CCER_CC3E; ne = TIM_CCER_CC3NE; }
  TIM1->CCER &= ~(me | ne);
  if (mainEnable) TIM1->CCER |= me;
  if (nEnable) TIM1->CCER |= ne;
}

void Board::floatAll() {
  TIM1->CCER &= ~(TIM_CCER_CC1E | TIM_CCER_CC1NE |
                  TIM_CCER_CC2E | TIM_CCER_CC2NE |
                  TIM_CCER_CC3E | TIM_CCER_CC3NE);
}

void Board::setThreePhaseDuty(float da, float db, float dc) {
  TIM1->CCR1 = dutyToCCR(da);
  TIM1->CCR2 = dutyToCCR(db);
  TIM1->CCR3 = dutyToCCR(dc);
  setChannelEnable(1, true, true);
  setChannelEnable(2, true, true);
  setChannelEnable(3, true, true);
}

void Board::setHBridge(float command) {
  if (command > 1) command = 1;
  if (command < -1) command = -1;
  float da = 0.5f + 0.45f * command;
  float db = 0.5f - 0.45f * command;
  TIM1->CCR1 = dutyToCCR(da);
  TIM1->CCR2 = dutyToCCR(db);
  setChannelEnable(1, true, true);
  setChannelEnable(2, true, true);
  setChannelEnable(3, false, false);
}

void Board::setSinglePhaseSPWM(float modulation, float angle) {
  if (modulation > 0.95f) modulation = 0.95f;
  if (modulation < 0.0f) modulation = 0.0f;
  float s = sinf(angle);
  float da = 0.5f + 0.5f * modulation * s;
  float db = 0.5f - 0.5f * modulation * s;
  TIM1->CCR1 = dutyToCCR(da);
  TIM1->CCR2 = dutyToCCR(db);
  setChannelEnable(1, true, true);
  setChannelEnable(2, true, true);
  setChannelEnable(3, false, false);
}

void Board::sixStep(uint8_t sector, float duty) {
  duty = clamp01(duty);
  if (duty > 0.95f) duty = 0.95f;
  floatAll();
  delayMicroseconds(2);
  auto source = [&](uint8_t ch) {
    if (ch == 1) TIM1->CCR1 = dutyToCCR(duty);
    if (ch == 2) TIM1->CCR2 = dutyToCCR(duty);
    if (ch == 3) TIM1->CCR3 = dutyToCCR(duty);
    setChannelEnable(ch, true, false);
  };
  auto sink = [&](uint8_t ch) {
    if (ch == 1) TIM1->CCR1 = 0;
    if (ch == 2) TIM1->CCR2 = 0;
    if (ch == 3) TIM1->CCR3 = 0;
    setChannelEnable(ch, false, true);
  };
  switch (sector % 6) {
    case 0: source(1); sink(2); break;
    case 1: source(1); sink(3); break;
    case 2: source(2); sink(3); break;
    case 3: source(2); sink(1); break;
    case 4: source(3); sink(1); break;
    case 5: source(3); sink(2); break;
  }
}

void Board::beginHall() {
  pinMode(PIN_HALL_A, INPUT_PULLUP);
  pinMode(PIN_HALL_B, INPUT_PULLUP);
  pinMode(PIN_HALL_C, INPUT_PULLUP);
}

uint8_t Board::hallState() {
  return (digitalRead(PIN_HALL_A) ? 4 : 0) |
         (digitalRead(PIN_HALL_B) ? 2 : 0) |
         (digitalRead(PIN_HALL_C) ? 1 : 0);
}

int8_t Board::hallSector(uint8_t state) {
  static const int8_t map[8] = {-1, 0, 4, 5, 2, 1, 3, -1};
  return map[state & 7];
}

void Board::beginEncoder() {
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_TIM4_CLK_ENABLE();
  GPIO_InitTypeDef g = {};
  g.Pin = GPIO_PIN_6 | GPIO_PIN_7;
  g.Mode = GPIO_MODE_AF_PP;
  g.Pull = GPIO_PULLUP;
  g.Speed = GPIO_SPEED_FREQ_HIGH;
  g.Alternate = GPIO_AF2_TIM4;
  HAL_GPIO_Init(GPIOB, &g);
  TIM4->CR1 = 0;
  TIM4->PSC = 0;
  TIM4->ARR = 0xFFFF;
  TIM4->CCMR1 = TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0;
  TIM4->CCER = 0;
  TIM4->SMCR = TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;
  TIM4->CNT = 0;
  TIM4->EGR = TIM_EGR_UG;
  TIM4->CR1 = TIM_CR1_CEN;
  _encoderLast = 0;
  _encoderPos = 0;
}

int32_t Board::sampleEncoder() {
  uint16_t now = (uint16_t)TIM4->CNT;
  int16_t delta = (int16_t)(now - _encoderLast);
  _encoderLast = now;
  _encoderPos += delta;
  return delta;
}
int32_t Board::encoderPosition() { return _encoderPos; }
void Board::zeroEncoder() { TIM4->CNT = 0; _encoderLast = 0; _encoderPos = 0; }

float Board::adcVolts(uint32_t pin) { return (float)analogRead(pin) * ADC_REF / 4095.0f; }

void Board::calibrateCurrentOffsets(uint16_t samples) {
  shutdown(true);
  if (samples < 64) samples = 64;
  double a = 0, b = 0, d = 0;
  for (uint16_t i = 0; i < samples; ++i) {
    a += adcVolts(PIN_IA);
    b += adcVolts(PIN_IB);
    d += adcVolts(PIN_IDC);
    delayMicroseconds(100);
  }
  _offsetIA = a / samples;
  _offsetIB = b / samples;
  _offsetIDC = d / samples;
}

float Board::readCurrentA() { return (adcVolts(PIN_IA) - _offsetIA) / CURRENT_V_PER_A; }
float Board::readCurrentB() { return (adcVolts(PIN_IB) - _offsetIB) / CURRENT_V_PER_A; }
float Board::readCurrentDC() { return (adcVolts(PIN_IDC) - _offsetIDC) / CURRENT_V_PER_A; }
float Board::readVbus() { return adcVolts(PIN_VBUS) * VBUS_RATIO; }
float Board::readThrottle() { return adcVolts(PIN_THROTTLE) / INPUT_DIV_RATIO; }
float Board::readBrake() { return adcVolts(PIN_BRAKE) / INPUT_DIV_RATIO; }

Measurements Board::readMeasurements() {
  Measurements m;
  m.ia = readCurrentA();
  m.ib = readCurrentB();
  m.ic = -m.ia - m.ib;
  m.idc = readCurrentDC();
  m.vbus = readVbus();
  m.throttle = readThrottle();
  m.brake = readBrake();
  return m;
}

uint32_t Board::pwmFrequency() { return _pwmHz; }
uint32_t Board::deadtimeNanoseconds() { return _deadtimeNs; }

float wrap2pi(float x) {
  const float twoPi = 2.0f * PI;
  while (x >= twoPi) x -= twoPi;
  while (x < 0) x += twoPi;
  return x;
}

void clarke(float ia, float ib, float &alpha, float &beta) {
  alpha = ia;
  beta = (ia + 2.0f * ib) * 0.57735026919f;
}

void park(float alpha, float beta, float theta, float &id, float &iq) {
  float c = cosf(theta), s = sinf(theta);
  id = alpha * c + beta * s;
  iq = -alpha * s + beta * c;
}

void invPark(float vd, float vq, float theta, float &alpha, float &beta) {
  float c = cosf(theta), s = sinf(theta);
  alpha = vd * c - vq * s;
  beta = vd * s + vq * c;
}

void svpwm(float alpha, float beta, float vbus, float &da, float &db, float &dc) {
  if (vbus < 1.0f) { da = db = dc = 0.5f; return; }
  float va = alpha;
  float vb = -0.5f * alpha + 0.86602540378f * beta;
  float vc = -0.5f * alpha - 0.86602540378f * beta;
  float vmax = fmaxf(va, fmaxf(vb, vc));
  float vmin = fminf(va, fminf(vb, vc));
  float voffset = -0.5f * (vmax + vmin);
  da = 0.5f + (va + voffset) / vbus;
  db = 0.5f + (vb + voffset) / vbus;
  dc = 0.5f + (vc + voffset) / vbus;
  da = fminf(0.95f, fmaxf(0.05f, da));
  db = fminf(0.95f, fmaxf(0.05f, db));
  dc = fminf(0.95f, fmaxf(0.05f, dc));
}

float PIController::update(float error, float dt) {
  float p = kp * error;
  integral += ki * error * dt;
  if (integral > limit) integral = limit;
  if (integral < -limit) integral = -limit;
  float y = p + integral;
  if (y > limit) y = limit;
  if (y < -limit) y = -limit;
  return y;
}

} // namespace ESC
