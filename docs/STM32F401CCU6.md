# STM32F401CCU6 untuk Praktikum Sistem Pengendali Elektrik

Board utama praktikum adalah WeAct BlackPill V2.0 berbasis STM32F401CCU6.

## PlatformIO

```ini
[env:blackpill_f401cc]
platform = ststm32
board = blackpill_f401cc
framework = arduino
upload_protocol = stlink
monitor_speed = 115200
```

Spesifikasi utama: Cortex-M4F 84 MHz, Flash 256 KB, SRAM 64 KB, ADC 12-bit, DMA, advanced timer TIM1 untuk PWM motor-control, serta timer umum yang dapat dipakai sebagai encoder interface.

## Pin praktikum yang disarankan

| Fungsi | Pin | Catatan |
|---|---|---|
| TIM1_CH1 | PA8 | PWM fase U / PWM BTS7960 |
| TIM1_CH2 | PA9 | PWM fase V |
| TIM1_CH3 | PA10 | PWM fase W |
| TIM1_CH1N | PB13 | PWM low-side U |
| TIM1_CH2N | PB14 | PWM low-side V |
| TIM1_CH3N | PB15 | PWM low-side W |
| Hall A/B/C | PB6/PB7/PB8 | Input digital; sesuaikan PCB |
| ADC arus U/V | PA0/PA1 | Maksimum 3.3 V |
| ADC Vbus | PA2 | Wajib divider tegangan |
| UART debug | PA2/PA3 atau USB serial | Hindari konflik ADC Vbus |

> Pin harus disesuaikan dengan PCB final. Jangan menghubungkan tegangan gate-driver, tegangan motor, atau 12/220 VAC langsung ke GPIO STM32.

## Aturan keselamatan

1. Uji algoritma dengan supply rendah dan current limit sebelum menghubungkan motor besar.
2. Gunakan gate driver terisolasi/tepat level untuk inverter.
3. Selalu sediakan dead-time, hardware shutdown, fuse, emergency stop, dan pre-charge bila diperlukan.
4. Praktikum 220 VAC hanya dilakukan pada meja uji terlindung dengan pengawasan dosen/laboran.
