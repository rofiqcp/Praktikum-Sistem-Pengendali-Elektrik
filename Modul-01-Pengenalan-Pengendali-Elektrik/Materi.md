# Modul 01 — Pengenalan Sistem Pengendali Elektrik

## Capaian
Mahasiswa mampu menjelaskan fungsi controller, power converter, aktuator, sensor, setpoint, feedback, proteksi, dan komunikasi pada sistem penggerak elektrik.

## 1. Apa itu pengendali elektrik?
Pengendali elektrik adalah sistem yang mengatur aliran energi listrik menuju beban agar variabel tertentu mengikuti perintah. Variabel yang dikendalikan dapat berupa kecepatan, torsi, posisi, arus, tegangan, frekuensi, atau arah putar.

Blok umum:

`Setpoint → Controller → Power Converter → Motor/Plant → Sensor → Feedback`

Proteksi (over-current, over-voltage, emergency stop, interlock) bekerja melintang di seluruh blok dan harus dapat mematikan aktuasi secara independen dari algoritma normal.

## 2. VFD dikontrol PLC
PLC menjalankan logika mesin. VFD menangani pembangkitan tegangan/frekuensi untuk motor AC. Antarmuka umum:
- Digital input: RUN, STOP, FWD, REV, preset speed.
- Analog: 0–10 V atau 4–20 mA untuk reference speed.
- RS485/Modbus RTU: command, frequency reference, monitoring, fault.

## 3. Motor DC
Kecepatan motor DC dapat diatur dengan tegangan rata-rata melalui PWM. Untuk arah dua arah diperlukan H-bridge. Pada praktikum P5, **Phase A dan B pada PCB Rev.E** dipakai sebagai H-bridge; BTS7960 tidak digunakan.

## 4. Inverter
Inverter mengubah DC menjadi AC. Pada P9, dua half-bridge A+B membentuk full bridge 1-fasa. Pada P10–P15, tiga half-bridge A/B/C membentuk inverter 3-fasa untuk BLDC/PMSM.

## 5. BLDC dan PMSM
BLDC memerlukan komutasi elektronik. Tahapan yang dipelajari:
1. six-step + Hall,
2. sinusoidal PWM,
3. SVPWM,
4. sensing current,
5. Clarke-Park,
6. FOC current,
7. FOC speed/position.

## 6. Board project Rev.E sebagai platform praktikum
Board menggunakan STM32F401CCU6, 3×IR2110, enam MOSFET, sensing IA/IB/IDC/VBUS, Hall/Encoder, CAN, I2C dan USART. `DRV_SD` di PA12 adalah fail-safe: HIGH mematikan gate driver, LOW mengaktifkan.

## 7. Keselamatan
Power electronics tidak dimulai dengan tegangan maksimum. Semua praktikum power-stage dimulai dari 12–18 V current-limited, gate diuji tanpa motor, dead-time diverifikasi, offset current dikalibrasi, baru motor dipasang.
