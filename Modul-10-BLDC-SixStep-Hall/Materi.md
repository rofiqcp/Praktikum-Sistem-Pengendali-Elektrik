# Modul 10 — BLDC Six-Step dengan Hall Sensor

## Tujuan
Membaca Hall A/B/C dari PB6/PB7/PB2 dan memilih pasangan MOSFET untuk enam sektor komutasi.

## Hall default
Firmware memakai bit `ABC` dan urutan umum:

| Hall | Rotor sector | Drive forward |
|---|---:|---|
| 001 | 0 | A+ B- |
| 101 | 1 | A+ C- |
| 100 | 2 | B+ C- |
| 110 | 3 | B+ A- |
| 010 | 4 | C+ A- |
| 011 | 5 | C+ B- |

000 dan 111 dianggap invalid. **Urutan Hall dapat berbeda karena wiring/motor**, sehingga tabel wajib diverifikasi dengan memutar rotor manual sebelum gate di-enable.

Untuk reverse, pasangan source/sink dibalik; pada helper board dilakukan dengan sektor `+3 mod 6`.

## PWM
High-side source dipWM, low-side sink diaktifkan, phase ketiga floating. Duty dibatasi di bawah 100% untuk menyediakan margin bootstrap IR2110.
