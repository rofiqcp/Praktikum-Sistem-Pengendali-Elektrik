# Pinout PCB ESC Rev.E

Sumber pinout: panduan wiring PCB Rev.E.

| STM32F401 | Fungsi |
|---|---|
| PA8 | TIM1_CH1 — PWM Phase A high / IR2110 A HIN |
| PB13 | TIM1_CH1N — PWM Phase A low / IR2110 A LIN |
| PA9 | TIM1_CH2 — PWM Phase B high |
| PB14 | TIM1_CH2N — PWM Phase B low |
| PA10 | TIM1_CH3 — PWM Phase C high |
| PB15 | TIM1_CH3N — PWM Phase C low |
| PA12 | DRV_SD, LOW enable / HIGH shutdown |
| PA0 | ADC IA |
| PA1 | ADC IB |
| PA4 | ADC IDC |
| PA5 | ADC VBUS |
| PA6 | ADC throttle |
| PA7 | ADC brake |
| PB6 | Hall A / Encoder A (TIM4_CH1) |
| PB7 | Hall B / Encoder B (TIM4_CH2) |
| PB2 | Hall C / Encoder Index |
| PB3/PB4/PB5 | SPI3 SCK/MISO/MOSI MCP2515 |
| PB12 | MCP2515 CS |
| PA15 | MCP2515 INT |
| PB8/PB9 | I2C1 SCL/SDA |
| PA2/PA3 | USART2 TX/RX |
| PB0/PB1/PB10/PA11 | Button 1..4 |

## Konstanta analog PCB

- Shunt IA/IB/IDC = 2 mΩ.
- Gain current amplifier ≈20 → sensitivitas ideal ≈40 mV/A.
- Zero-current sekitar VREF = 1.65 V; firmware wajib melakukan zero-offset calibration aktual.
- VBUS divider = (100k + 100k) : 10k → rasio ideal 21:1.
- Throttle/brake divider = 15k seri dan 22k ke GND → `Vadc = Vin × 22/(15+22)`.

## Aturan sensor Hall/Encoder

Tidak ada level shifter. Sensor push-pull wajib 3.3 V. Sensor 5 V hanya boleh bila output open-collector/open-drain dan ditarik ke 3.3 V oleh pull-up PCB.
