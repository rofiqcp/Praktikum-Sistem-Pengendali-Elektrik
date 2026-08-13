# Project 06 — PCB Universal Motor Control

## Target
Membuat dan membuktikan satu PCB Rev.E yang siap dipakai untuk:
1. motor DC antara PH_A–PH_B,
2. inverter 1-fasa full bridge A+B,
3. BLDC 3-fasa A/B/C,
4. Hall atau encoder AB/ABI,
5. current/voltage sensing,
6. komunikasi USART dan CAN.

## Deliverable
- schematic EasyEDA/KiCad,
- PCB layout,
- BOM aktual beserta part number,
- Gerber,
- foto PCB kosong dan assembled,
- hasil continuity test,
- tabel rail 12V/5V/3V3/VREF,
- screenshot osiloskop 6 gate PWM dan dead-time,
- hasil kalibrasi IA/IB/IDC/VBUS,
- log firmware diagnostic,
- video bring-up,
- daftar revisi bila ada.

## Acceptance test
Board hanya lulus bila boot default shutdown, tidak ada shoot-through, sensor ADC masuk rentang aman, Hall/Encoder aman 3.3 V, dan low-voltage motor test berhasil.
