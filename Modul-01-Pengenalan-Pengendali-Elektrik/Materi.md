# Modul 01 — Pengenalan Sistem Pengendali Elektrik

## Praktikum Sistem Pengendali Elektrik

Pertemuan pertama menjadi fondasi seluruh praktikum. Mahasiswa belum langsung diminta menyalakan power stage. Fokus utama adalah memahami **alur energi**, **alur informasi**, jenis aktuator, jenis power converter, metode kontrol, sensor, proteksi, dan posisi PCB ESC Rev.E dalam keseluruhan mata kuliah.

---

## 1. Capaian Pembelajaran

Setelah menyelesaikan modul ini, mahasiswa diharapkan mampu:

1. Menjelaskan pengertian sistem pengendali elektrik.
2. Membedakan **power path** dan **control/information path**.
3. Menjelaskan fungsi PLC, VFD, H-bridge, inverter, gate driver, sensor, dan motor.
4. Menjelaskan perbedaan motor AC induksi, motor DC, BLDC, dan PMSM secara praktis.
5. Menjelaskan perbedaan kontrol digital, analog 4–20 mA, dan komunikasi RS485/Modbus.
6. Menggambarkan diagram blok sistem kontrol open-loop dan closed-loop.
7. Menjelaskan fungsi PWM, duty cycle, switching frequency, electrical frequency, dan dead-time.
8. Mengidentifikasi bagian utama PCB ESC Rev.E yang akan digunakan pada P5–P15.
9. Menjelaskan alasan adanya fail-safe `DRV_SD` pada PCB.
10. Menjalankan program Python sederhana untuk membantu memahami pemilihan struktur pengendali dan hubungan duty–tegangan.
11. Menjelaskan urutan keselamatan sebelum power electronics diaktifkan.

---

## 2. Gambaran Besar Mata Kuliah

Mata kuliah ini mempelajari pengendali elektrik secara bertahap dari perangkat industri sampai algoritma pengendalian motor yang dijalankan pada mikrokontroler.

Urutan besarnya adalah:

```text
PLC + VFD
   ↓
Digital control
   ↓
4–20 mA analog control
   ↓
RS485 / Modbus RTU
   ↓
STM32 + PWM
   ↓
DC motor H-bridge
   ↓
Inverter 1 phase
   ↓
BLDC six-step
   ↓
Sinusoidal PWM
   ↓
SVPWM
   ↓
Current sensing
   ↓
Clarke / Park
   ↓
FOC current
   ↓
FOC speed / position
```

Dengan urutan ini mahasiswa tidak hanya melihat algoritma, tetapi memahami hubungan antara **command**, **controller**, **power electronics**, **motor**, dan **feedback sensor**.

---

## 3. Apa yang Dimaksud Pengendali Elektrik?

Sistem pengendali elektrik adalah sistem yang mengatur energi listrik menuju beban agar suatu variabel mengikuti nilai yang diinginkan.

Contoh variabel yang dikendalikan:

- ON/OFF,
- arah putaran,
- kecepatan,
- posisi,
- torsi,
- arus,
- tegangan,
- frekuensi,
- daya.

Diagram blok umum:

```text
             ┌───────────── feedback ─────────────┐
             │                                     │
Setpoint → Controller → Power Converter → Plant/Motor → Sensor
             │
             └→ Protection / Interlock / Shutdown
```

### 3.1 Setpoint
Nilai yang diinginkan. Contoh:

- motor harus berputar 1000 rpm,
- posisi shaft harus 90°,
- arus q-axis harus 2 A,
- VFD harus menghasilkan 25 Hz.

### 3.2 Controller
Bagian yang memutuskan aksi berdasarkan perintah atau error.

Contoh:

- ladder PLC,
- logic ON/OFF,
- PI/PID,
- state machine,
- FOC controller.

### 3.3 Power converter
Bagian yang benar-benar memproses energi.

Contoh:

- contactor,
- VFD,
- DC chopper,
- H-bridge,
- inverter 1-fasa,
- inverter 3-fasa.

### 3.4 Plant / motor
Objek fisik yang dikendalikan.

Contoh:

- motor induksi,
- motor DC brushed,
- BLDC,
- PMSM.

### 3.5 Sensor
Mengubah kondisi fisik menjadi sinyal yang dapat dibaca controller.

Contoh:

- Hall sensor,
- encoder AB/ABI,
- current shunt,
- voltage divider,
- tachometer,
- resolver.

---

## 4. Power Path vs Control Path

Salah satu kesalahan awal yang sering terjadi adalah menganggap kabel kontrol dan kabel daya memiliki fungsi yang sama.

### Power path
Membawa energi utama menuju motor.

Contoh sistem VFD:

```text
PLN / Supply AC
     ↓
    VFD
     ↓ U/V/W
Motor AC
```

Contoh PCB ESC:

```text
Battery / DC Supply
      ↓ VBUS
DC-Link Capacitor
      ↓
6 MOSFET
      ↓ PH_A / PH_B / PH_C
BLDC / PMSM
```

### Control path
Membawa perintah atau data.

```text
Push button → PLC → Digital Output → DI VFD
```

atau

```text
PC → USB-RS485 → VFD
```

atau

```text
Hall/Encoder → STM32 → Algorithm → PWM → IR2110 → MOSFET
```

Arus pada jalur kontrol umumnya jauh lebih kecil dibanding jalur daya. Akan tetapi kesalahan wiring pada control path tetap dapat menyebabkan motor bergerak tidak terduga.

---

## 5. Open-Loop dan Closed-Loop

### 5.1 Open-loop
Controller memberikan command tanpa memeriksa hasil aktual.

```text
Command duty 40% → H-bridge → motor
```

Kecepatan aktual dapat berubah karena beban, tegangan supply, temperatur, dan gesekan.

### 5.2 Closed-loop
Controller membandingkan setpoint dengan feedback.

```text
RPM_ref → [error] → PI speed → PWM → Motor → Encoder → RPM_actual
             ↑__________________________________________|
```

Error:

```text
e(t) = setpoint(t) - feedback(t)
```

Pada P15 struktur menjadi bertingkat:

```text
Position loop → Speed loop → Current/Torque loop → SVPWM → Motor
```

Loop yang lebih dalam biasanya harus bekerja lebih cepat daripada loop yang berada di luar.

---

## 6. VFD dan Motor AC

VFD atau Variable Frequency Drive adalah power converter untuk mengatur motor AC dengan mengatur tegangan/frekuensi keluaran sesuai metode kontrol drive.

Secara sederhana:

```text
AC input
  ↓
Rectifier
  ↓
DC link
  ↓
3-phase inverter
  ↓
U / V / W
  ↓
Motor AC
```

PLC tidak memberikan arus daya langsung ke motor. PLC memberikan command kepada VFD.

Metode command yang dipelajari:

| Metode | Pertemuan | Contoh |
|---|---:|---|
| Digital terminal | P2 | RUN, FWD, REV, multi-speed |
| Analog current | P3 | 4–20 mA frequency reference |
| Serial communication | P4 | RS485 Modbus RTU |

Keuntungan komunikasi serial adalah banyak informasi dapat melewati sepasang kabel komunikasi: command, reference, status, monitor, dan fault.

---

## 7. Motor DC Brushed

Motor DC brushed relatif mudah dipahami karena arah torsi dapat diubah dengan membalik polaritas tegangan armature.

Model listrik sederhana:

```text
V = R·i + L·di/dt + Ke·ω
```

Torsi elektromagnetik:

```text
T = Kt·i
```

Artinya secara intuitif:

- arus sangat berhubungan dengan torsi,
- tegangan rata-rata berpengaruh pada kecepatan,
- beban yang bertambah membutuhkan arus lebih besar.

### 7.1 H-bridge
Untuk membalik arah diperlukan empat saklar daya.

```text
        +VBUS
      Q1     Q2
       |     |
       +--M--+
       |     |
      Q3     Q4
        GND
```

Contoh state ideal:

| Kondisi | Leg A | Leg B | Hasil |
|---|---|---|---|
| Forward | HIGH/PWM | LOW | motor + |
| Reverse | LOW | HIGH/PWM | motor - |
| Coast | OFF | OFF | free-run |
| Brake* | kondisi tertentu | kondisi tertentu | dynamic braking |

`*` Strategi brake harus mengikuti desain hardware dan firmware yang dipakai.

Pada P5, H-bridge dibuat menggunakan **Phase A dan Phase B PCB Rev.E**, bukan BTS7960.

---

## 8. PWM, Duty Cycle, dan Frekuensi

PWM mengatur rata-rata energi dengan mengubah perbandingan waktu ON dan OFF.

```text
Duty = Ton / T × 100%
```

Untuk penyederhanaan ideal:

```text
Vavg ≈ Duty × Vbus
```

Contoh VBUS 12 V:

| Duty | Vavg ideal |
|---:|---:|
| 0% | 0 V |
| 25% | 3 V |
| 50% | 6 V |
| 75% | 9 V |
| 100% | 12 V |

### PWM carrier frequency bukan motor electrical frequency

- **PWM/carrier frequency**: frekuensi switching MOSFET.
- **Electrical frequency**: frekuensi medan listrik yang dibentuk untuk motor AC/BLDC.
- **Mechanical frequency**: frekuensi putaran rotor.

Ketiganya tidak boleh dicampur.

---

## 9. Dead-Time

Satu half-bridge memiliki MOSFET high-side dan low-side. Keduanya tidak boleh ON bersamaan karena akan membuat jalur langsung VBUS ke ground yang disebut **shoot-through**.

Dead-time adalah jeda singkat saat kedua transistor OFF ketika berganti konduksi.

```text
High:  ───────┐      ┌──────
              └──────┘
                 ↑ dead-time
Low :  _________┌────┐_______
```

Dead-time terlalu kecil berisiko shoot-through. Dead-time terlalu besar menambah distorsi dan mengurangi kualitas modulasi. P7 mempelajari pengaruhnya secara langsung menggunakan osiloskop.

---

## 10. Inverter 1-Fasa

Dua half-bridge dapat dibuat menjadi full-bridge 1-fasa.

Pada P9:

```text
DC 12 V → Phase A+B PCB → AC low voltage → Transformer 12/220 → AC high voltage
```

Frekuensi output yang diinginkan, misalnya 50 Hz, tidak sama dengan carrier PWM yang dapat berada pada orde kHz.

PWM sinusoidal/SPWM digunakan agar komponen fundamental mendekati bentuk sinus.

---

## 11. BLDC dan PMSM

BLDC/PMSM tidak memakai komutator mekanik seperti motor DC brushed. Komutasi dilakukan oleh inverter dan controller.

### 11.1 Six-step
P10 menggunakan sensor Hall untuk menentukan sektor rotor. Setiap sektor mengaktifkan kombinasi dua fase dan membiarkan satu fase floating sesuai strategi komutasi.

### 11.2 Sinusoidal PWM
P11 membentuk tiga referensi sinus yang berbeda 120°.

```text
Va = sin(θ)
Vb = sin(θ - 120°)
Vc = sin(θ + 120°)
```

### 11.3 SVPWM
P12 menggunakan space-vector modulation untuk menentukan duty tiga phase berdasarkan vektor tegangan di bidang αβ.

### 11.4 FOC
P14–P15 mengubah arus tiga phase ke kerangka koordinat yang berputar bersama rotor.

Urutan konseptual:

```text
Ia, Ib, Ic
   ↓ Clarke
Iα, Iβ
   ↓ Park(θe)
Id, Iq
   ↓ PI current controller
Vd, Vq
   ↓ inverse Park
Vα, Vβ
   ↓ SVPWM
Duty A/B/C
```

Secara umum:

- `Id` berhubungan dengan flux axis,
- `Iq` berhubungan dengan torque axis.

---

## 12. PCB ESC Rev.E sebagai Hardware Utama P5–P15

PCB project menggunakan:

- STM32F401CCU6 Black Pill,
- 3 × IR2110 gate driver,
- 6 × N-channel MOSFET,
- phase A/B/C,
- current sensing IA dan IB,
- DC/bus current sensing IDC,
- VBUS sensing,
- throttle dan brake analog input,
- Hall/Encoder A-B-C(I),
- CAN MCP2515 + MCP2562,
- I2C,
- USART,
- empat push button,
- hardware shutdown `DRV_SD`.

### 12.1 Pin penting

| Fungsi | STM32F401 |
|---|---|
| Phase A high | PA8 |
| Phase A low | PB13 |
| Phase B high | PA9 |
| Phase B low | PB14 |
| Phase C high | PA10 |
| Phase C low | PB15 |
| DRV_SD | PA12 |
| IA | PA0 |
| IB | PA1 |
| IDC | PA4 |
| VBUS | PA5 |
| throttle | PA6 |
| brake | PA7 |
| Hall/Encoder A | PB6 |
| Hall/Encoder B | PB7 |
| Hall C / Encoder I/Z | PB2 |

`DRV_SD` bersifat fail-safe:

```text
PA12 HIGH → shutdown IR2110
PA12 LOW  → gate driver enable
```

Saat MCU reset, pull-up hardware menjaga driver shutdown.

---

## 13. Sensor Arus dan Tegangan pada PCB

PCB menggunakan shunt 2 mΩ dan LM358 untuk sensing arus. Gain nominal amplifier sekitar 20 sehingga sensitivitas ideal kira-kira:

```text
2 mΩ × 20 = 40 mV/A
```

Zero-current diletakkan sekitar VREF 1.65 V agar arus positif dan negatif dapat direpresentasikan dalam rentang ADC.

PCB juga memiliki VBUS divider nominal sekitar 21:1.

Nilai nominal ini **tidak menggantikan kalibrasi**. Pada P13 firmware akan melakukan zero-offset calibration ketika gate driver shutdown.

---

## 14. Proteksi dan Fail-Safe

Proteksi tidak boleh hanya bergantung pada algoritma normal.

Contoh lapisan keselamatan:

1. fuse,
2. current-limited supply saat bring-up,
3. gate-driver shutdown,
4. interlock software,
5. current limit,
6. voltage limit,
7. timeout komunikasi,
8. emergency stop,
9. enclosure/guard,
10. prosedur kerja.

Prinsip penting:

```text
Fault → actuator OFF lebih penting daripada mempertahankan command.
```

---

## 15. Urutan Bring-Up yang Akan Dipakai

Urutan umum P5–P15:

1. VBUS belum dipasang: continuity test.
2. Periksa supply logic.
3. Pastikan firmware boot dengan `DRV_SD = HIGH`.
4. Gunakan bus rendah 12–18 V dengan current limit.
5. Tanpa motor, cek high/low gate menggunakan osiloskop yang sesuai.
6. Ukur dead-time.
7. Kalibrasi offset sensor arus.
8. Baru pasang motor kecil/beban rendah.
9. Naikkan command perlahan.
10. Selalu sediakan cara STOP/shutdown.

---

## 16. Perbandingan Sistem yang Dipelajari

| Sistem | Power stage | Command utama | Feedback | Topik utama |
|---|---|---|---|---|
| PLC + VFD | rectifier + inverter internal VFD | DI / analog / RS485 | status VFD/sensor | industrial drive |
| DC motor | H-bridge | duty + direction | optional encoder/current | PWM |
| inverter 1-phase | full bridge | sine reference | voltage/current | SPWM |
| BLDC six-step | 3 half-bridge | Hall sector + duty | Hall | commutation |
| BLDC sinus | 3 half-bridge | electrical angle | Hall/encoder | sine PWM |
| SVPWM | 3 half-bridge | Vα/Vβ | angle/current | vector modulation |
| FOC | 3 half-bridge | Id/Iq | current + rotor angle | vector current control |

---

## 17. Program yang Harus Dijalankan

Folder `program/` berisi:

### A. `control_selector.py`
Membandingkan struktur beberapa sistem pengendali.

```bash
python3 control_selector.py
python3 control_selector.py --system foc
```

### B. `power_flow_demo.py`
Menampilkan alur power dan control berdasarkan sistem yang dipilih.

```bash
python3 power_flow_demo.py --system vfd
python3 power_flow_demo.py --system bldc
```

### C. `dc_motor_pwm_demo.py`
Menghitung duty, tegangan rata-rata ideal, dan arah motor DC.

```bash
python3 dc_motor_pwm_demo.py --vbus 12 --duty 25
python3 dc_motor_pwm_demo.py --vbus 12 --duty -60
```

Program dapat dijalankan di VS Code, terminal, atau Google Colab.

---

## 18. Pertanyaan Pemahaman

1. Mengapa PLC tidak boleh langsung menggerakkan motor 3-fasa?
2. Apa perbedaan power path dan control path?
3. Apa perbedaan open-loop dan closed-loop?
4. Mengapa kecepatan motor DC tidak selalu proporsional sempurna dengan duty?
5. Apa fungsi H-bridge?
6. Mengapa FWD dan REV perlu interlock?
7. Apa perbedaan PWM carrier frequency dan output electrical frequency?
8. Apa fungsi dead-time?
9. Mengapa BLDC memerlukan informasi posisi rotor?
10. Mengapa FOC memerlukan current sensing?
11. Mengapa `DRV_SD` harus aktif ketika MCU reset?
12. Mengapa bring-up dimulai dengan tegangan rendah dan current limit?
13. Apa keuntungan 4–20 mA dibanding command tegangan pada lingkungan industri?
14. Apa keuntungan RS485 dibanding banyak kabel digital jika data yang dibutuhkan banyak?
15. Mengapa nilai nominal rangkaian sensor tetap harus dikalibrasi?

---

## 19. Glosarium Singkat

| Istilah | Arti praktis |
|---|---|
| Actuator | perangkat yang menghasilkan aksi fisik |
| Controller | pembuat keputusan kontrol |
| Plant | objek yang dikendalikan |
| Setpoint | nilai target |
| Feedback | nilai aktual yang diukur |
| PWM | modulasi lebar pulsa |
| Duty cycle | rasio waktu ON |
| VFD | variable frequency drive |
| H-bridge | empat saklar untuk kontrol bipolar/bidirectional |
| Inverter | konverter DC ke AC |
| Hall sensor | sensor posisi magnetik diskret |
| Encoder | sensor posisi/kecepatan berbasis pulsa |
| Dead-time | jeda antarsaklar complementary |
| SVPWM | space vector PWM |
| FOC | field-oriented control |
| VBUS | bus DC power stage |
| Gate driver | penggerak gate MOSFET |
| Interlock | logika pencegah kombinasi command berbahaya |

---

## 20. Referensi Praktikum

1. Panduan hardware: `Panduan_Wiring_ESC_F401_IR2110_LM358_RevE_FINAL.docx`.
2. Dokumentasi resmi WECON PLC sesuai seri unit laboratorium.
3. Dokumentasi resmi WECON VFD sesuai seri unit laboratorium.
4. Datasheet STM32F401CCU6 dan manual timer STM32F4.
5. Datasheet IR2110 dan komponen power stage yang benar-benar dipasang pada PCB.

> Nomor parameter VFD, register komunikasi, dan alamat I/O PLC harus selalu disesuaikan dengan **model aktual** yang digunakan di laboratorium.