# Praktikum Sistem Pengendali Elektrik — Branch v1

Repository praktikum 16 pertemuan dengan satu hardware utama: **PCB ESC Rev.E STM32F401CCU6 + 3× IR2110 + 6 MOSFET + LM358** sesuai panduan `Panduan_Wiring_ESC_F401_IR2110_LM358_RevE_FINAL.docx`.

## Target pembelajaran

Mahasiswa bergerak dari pengendali elektrik industri (PLC/VFD) menuju embedded motor drive: PWM H-bridge, inverter 1-fasa, six-step BLDC, sinusoidal PWM, SVPWM, pengukuran arus/tegangan, Clarke-Park, FOC, kontrol kecepatan, dan kontrol posisi.

## Peta 16 pertemuan

| P | Materi utama | Praktikum / program |
|---|---|---|
| 1 | Pengantar pengendali elektrik, VFD-PLC, DC motor, inverter, BLDC | Identifikasi blok sistem dan keselamatan |
| 2 | WECON PLC + WECON VFD digital | RUN/STOP, FWD/REV, multi-speed digital |
| 3 | VFD analog 4–20 mA | Scaling 4–20 mA dan karakterisasi frekuensi |
| 4 | VFD RS485 Modbus RTU | Read/write register setelah alamat diverifikasi dari manual tipe VFD lab |
| 5 | STM32F401 + PCB Rev.E: motor DC | H-bridge memakai Phase A+B, variasi duty dan PWM |
| 6 | **Project hardware PCB** | Assembly, continuity test, power-rail test, gate test, ADC test, low-voltage motor test |
| 7 | Complementary PWM | Variasi dead-time, carrier frequency, phase/duty |
| 8 | Responsi P1–P7 | Tanya jawab + demonstrasi board |
| 9 | Inverter 1-fasa 12 V → trafo 12/220 V | SPWM full bridge Phase A+B, variasi frekuensi |
| 10 | BLDC six-step + Hall | Hall decoding dan 6-step commutation |
| 11 | BLDC sine wave | 3-phase sinusoidal PWM, frekuensi elektrik dan dead-time |
| 12 | BLDC SVPWM | αβ vector, sector, duty U/V/W |
| 13 | Sensor arus/tegangan | Offset calibration IA/IB/IDC, VBUS, throttle, brake |
| 14 | Clarke-Park & current FOC | Ia/Ib/Ic → Iα/Iβ → Id/Iq + PI current loop |
| 15 | FOC speed & position | Encoder AB, cascaded position-speed-current control |
| 16 | Responsi final | Tanya jawab dan demo end-to-end |

## Board Rev.E yang dipakai P5–P15

- MCU: STM32F401CCU6 Black Pill.
- Gate driver: 3× IR2110.
- PWM: PA8/PB13 (A), PA9/PB14 (B), PA10/PB15 (C).
- Driver shutdown: PA12, **LOW = enable, HIGH = shutdown**.
- ADC: PA0 IA, PA1 IB, PA4 IDC, PA5 VBUS, PA6 throttle, PA7 brake.
- Hall/Encoder: PB6 A, PB7 B, PB2 C/Index.
- CAN: MCP2515 + MCP2562.
- Power target PCB: 42 V; **bring-up wajib dimulai 12–18 V dengan current limit**.

## Menjalankan program PlatformIO

Setiap modul embedded memiliki folder `program/` sebagai project PlatformIO mandiri.

```bash
cd Modul-05-Motor-DC-HBridge/program
pio run
pio run -t upload
pio device monitor -b 115200
```

Board PlatformIO:

```ini
board = blackpill_f401cc
framework = arduino
```

Library board bersama berada di `lib/ESCBoard` dan direferensikan oleh setiap project menggunakan `lib_extra_dirs = ../../lib`.

## Catatan penting

1. P5 pada rancangan awal memakai BTS7960. Pada branch `v1` **diganti memakai power stage PCB Rev.E sendiri**, sehingga Phase A+B menjadi full H-bridge motor DC.
2. P9 memakai Phase A+B sebagai full-bridge inverter. Untuk praktikum gunakan VBUS 12 V dan trafo 12→220 V. Sisi 220 V tetap berbahaya dan hanya diuji di meja terlindung.
3. FOC pada board ini bersifat edukasional/prototipe. LM358 memiliki bandwidth terbatas, sehingga frekuensi PWM dan bandwidth current-loop dibuat konservatif.
4. Parameter WECON yang berupa nomor parameter/register tidak di-hard-code tanpa manual model VFD yang benar. Isi tabel register pada P2–P4 berdasarkan unit aktual lab.
