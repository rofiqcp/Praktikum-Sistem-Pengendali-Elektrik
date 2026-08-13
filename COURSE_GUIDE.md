# Course Guide — Praktikum Sistem Pengendali Elektrik v1

## Filosofi praktikum

Satu semester dibangun sebagai satu jalur kompetensi. P1–P4 mengenalkan pengendali industri dengan PLC/VFD. P5–P15 kemudian memakai **PCB ESC Rev.E yang sama** sehingga mahasiswa tidak berpindah-pindah power stage ketika konsep meningkat dari DC motor hingga FOC.

## Hardware minimum per kelompok

- PCB ESC Rev.E assembled + STM32F401CCU6 Black Pill.
- ST-Link.
- PSU DC current-limited 0–18 V untuk bring-up; supply lebih tinggi hanya setelah board lulus acceptance test.
- Motor DC kecil.
- BLDC/PMSM dengan Hall.
- Encoder AB/ABI yang 3.3 V-compatible atau open-collector.
- VFD WECON + PLC WECON untuk P1–P4.
- USB-RS485.
- Multimeter, oscilloscope, current meter/probe bila tersedia.
- Trafo 12→220 V terisolasi untuk P9 di meja terlindung.

## Software

- VS Code + PlatformIO.
- Python 3 untuk P1/P3/P4 helper.
- Software pemrograman PLC WECON sesuai model aktual.
- Tool konfigurasi VFD/manual model yang digunakan.

## Urutan kelulusan hardware

`Continuity → rail 12/5/3.3/VREF → reset shutdown → gate PWM → dead-time → ADC calibration → Hall/Encoder → DC motor → inverter → BLDC → FOC`

Mahasiswa tidak boleh melompati tahap hanya karena firmware berhasil di-upload.

## Konvensi laporan

Setiap jobsheet minimal menghasilkan:
- tujuan,
- diagram/wiring,
- source program + versi commit,
- parameter eksperimen,
- data mentah,
- grafik/tabel,
- analisis,
- fault/abnormal case,
- kesimpulan.

## Struktur tugas

P1–P5, P7, P9–P15 memiliki `TugasVideo.md`. P6 adalah project assembly/bring-up. P8 adalah responsi dan checkpoint PCB. P16 adalah responsi final penuh.

## Aturan source code

1. Jangan mengubah pin Rev.E tanpa memperbarui `docs/PINOUT_BOARD_REV_E.md`.
2. Boot harus shutdown.
3. Reconfigure TIM1 hanya saat shutdown.
4. Current offset calibration dilakukan tanpa arus motor.
5. Limit duty, current, speed, dan VBUS dibuat konservatif pada tahap awal.
6. Hall map, encoder PPR dan pole-pair wajib dianggap parameter motor, bukan konstanta universal.
