# STM32F401CCU6 pada PCB ESC Rev.E

MCU praktikum adalah STM32F401CCU6 Black Pill yang dipasang sebagai module plug-in pada PCB ESC Rev.E.

## PlatformIO

```ini
[env:blackpill_f401cc]
platform = ststm32
board = blackpill_f401cc
framework = arduino
upload_protocol = stlink
monitor_speed = 115200
```

## Pin yang benar untuk PCB Rev.E

| Fungsi | Pin STM32F401 | Peripheral |
|---|---|---|
| Phase A high | PA8 | TIM1_CH1 |
| Phase A low | PB13 | TIM1_CH1N |
| Phase B high | PA9 | TIM1_CH2 |
| Phase B low | PB14 | TIM1_CH2N |
| Phase C high | PA10 | TIM1_CH3 |
| Phase C low | PB15 | TIM1_CH3N |
| Gate-driver shutdown | PA12 | GPIO, HIGH=shutdown |
| Current IA | PA0 | ADC |
| Current IB | PA1 | ADC |
| Current IDC | PA4 | ADC |
| VBUS | PA5 | ADC |
| Throttle | PA6 | ADC |
| Brake | PA7 | ADC |
| Hall A / Encoder A | PB6 | GPIO / TIM4_CH1 |
| Hall B / Encoder B | PB7 | GPIO / TIM4_CH2 |
| Hall C / Encoder Index | PB2 | GPIO/EXTI |
| USART debug TX/RX | PA2/PA3 | USART2 |
| CAN MCP2515 SPI | PB3/PB4/PB5 | SPI3 |
| CAN CS | PB12 | GPIO |
| CAN INT | PA15 | GPIO/EXTI |
| I2C | PB8/PB9 | I2C1 |

> PA9/PA10 dipakai power-stage PWM dan PA12 dipakai `DRV_SD`, sehingga contoh praktikum tidak memakai UART/USB yang berpotensi bentrok dengan pin tersebut. Debug diarahkan ke USART2 PA2/PA3.

## Timer motor-control

Library `ESCBoard` mengkonfigurasi TIM1 center-aligned dengan tiga pasangan complementary output. Dead-time dimasukkan melalui register BDTR. Firmware selalu menjaga `DRV_SD` HIGH selama konfigurasi ulang timer.

## Analog nominal Rev.E

- Shunt: 2 mΩ.
- Gain current amplifier LM358: ≈20.
- Sensitivitas ideal current: ≈40 mV/A.
- VBUS divider: ≈21:1.
- Zero current: dikalibrasi saat startup; jangan hard-code 1.65 V sebagai hasil akhir.
