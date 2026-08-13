# Jobsheet 01 — Identifikasi Sistem Pengendali Elektrik

## 1. Tujuan Praktikum

Mahasiswa melakukan orientasi laboratorium dan mengidentifikasi hubungan antara controller, power converter, motor, sensor, proteksi, dan komunikasi **tanpa langsung melakukan pengujian power-stage bertegangan tinggi**.

---

## 2. Peralatan

Minimal:

- PLC WECON yang tersedia di laboratorium,
- VFD WECON,
- motor AC induksi,
- motor DC brushed,
- motor BLDC/PMSM,
- PCB ESC Rev.E,
- multimeter,
- komputer/laptop,
- Python 3,
- VS Code atau Google Colab,
- manual/datasheet perangkat aktual.

Opsional:

- osiloskop,
- current-limited bench supply,
- encoder,
- Hall sensor,
- USB-RS485.

> Pada P1 PCB ESC hanya diidentifikasi. Jangan memberi VBUS hanya untuk memenuhi tugas identifikasi.

---

## 3. Keselamatan Sebelum Praktikum

Sebelum menyentuh hardware:

1. Identifikasi sumber AC utama dan emergency stop.
2. Pastikan VFD dalam kondisi STOP.
3. Jangan mencabut/memasang kabel motor ketika VFD aktif.
4. Jangan mengukur node high-side dengan ground clip osiloskop biasa tanpa metode pengukuran yang benar.
5. PCB Rev.E belum diberi VBUS selama identifikasi.
6. Jangan menghubungkan sensor push-pull 5 V langsung ke input Hall/Encoder PCB.
7. Pastikan setiap kelompok mengetahui cara mematikan sumber energi.

---

## 4. Praktikum A — Inventarisasi Perangkat

Catat nameplate dan model aktual.

| No | Perangkat | Merk/model | Supply | Rating | Fungsi utama |
|---:|---|---|---|---|---|
| 1 | PLC | | | | |
| 2 | VFD | | | | |
| 3 | Motor AC | | | | |
| 4 | Motor DC | | | | |
| 5 | BLDC/PMSM | | | | |
| 6 | PCB Rev.E | STM32F401 + IR2110 | | | |

### Pertanyaan

- Apakah output PLC relay atau transistor?
- Berapa tegangan input digital PLC?
- Berapa tegangan input VFD?
- Berapa daya/rating motor?
- Apakah BLDC memiliki Hall sensor?
- Apakah encoder tersedia?

---

## 5. Praktikum B — Diagram PLC → VFD → Motor AC

Gambarkan dua diagram terpisah.

### 5.1 Power path

```text
Supply AC → VFD → U/V/W → Motor AC
```

Tambahkan:

- breaker/fuse bila ada,
- grounding,
- terminal input VFD,
- terminal motor.

### 5.2 Control path

```text
Push button / logic → PLC → output PLC → DI VFD
```

Tambahkan kemungkinan alternatif:

```text
PLC AO → 4–20 mA → AI VFD
PC/PLC → RS485 → VFD
```

### Output wajib

Beri warna berbeda pada gambar laporan:

- power path,
- control path,
- feedback/status path.

---

## 6. Praktikum C — Identifikasi Motor DC dan H-Bridge

Gambar H-bridge empat saklar.

Tentukan state konseptual:

| Command | Tegangan motor ideal | Arah |
|---:|---:|---|
| -100% | -VBUS | reverse |
| -50% | sekitar -0.5 VBUS | reverse |
| 0% | 0 | stop/coast sesuai strategi |
| +50% | sekitar +0.5 VBUS | forward |
| +100% | +VBUS | forward |

Diskusikan mengapa **tegangan rata-rata ideal tidak sama dengan kecepatan aktual** pada semua kondisi.

---

## 7. Praktikum D — Penelusuran PCB ESC Rev.E

Tanpa memberi VBUS, cari dan tunjukkan:

- terminal VBUS+/GND,
- PH_A,
- PH_B,
- PH_C,
- STM32F401CCU6 Black Pill,
- tiga IR2110,
- enam MOSFET,
- shunt current,
- konektor Hall/Encoder,
- konektor CAN,
- konektor USART,
- I2C,
- throttle/brake,
- `DRV_SD`.

Cocokkan pin:

| Fungsi | Pin |
|---|---|
| Phase A H/L | PA8 / PB13 |
| Phase B H/L | PA9 / PB14 |
| Phase C H/L | PA10 / PB15 |
| DRV_SD | PA12 |
| IA / IB | PA0 / PA1 |
| IDC / VBUS | PA4 / PA5 |
| Hall/Encoder A/B/C(I) | PB6 / PB7 / PB2 |

### Pertanyaan

1. Mengapa terdapat 6 output PWM untuk 3 phase?
2. Mengapa high-side dan low-side perlu gate driver?
3. Mengapa `DRV_SD` menggunakan pull-up hardware?
4. Mengapa current shunt berada di power path tetapi sinyalnya masuk ke ADC?

---

## 8. Praktikum E — Jalankan Program Python

Masuk ke folder:

```bash
cd Modul-01-Pengenalan-Pengendali-Elektrik/program
```

### 8.1 Control selector

```bash
python3 control_selector.py
python3 control_selector.py --system vfd
python3 control_selector.py --system foc
```

Catat untuk setiap sistem:

- plant,
- converter,
- command,
- feedback,
- proteksi.

### 8.2 Power flow demo

```bash
python3 power_flow_demo.py --system vfd
python3 power_flow_demo.py --system dc
python3 power_flow_demo.py --system bldc
```

Tulis ulang output menjadi diagram blok.

### 8.3 PWM demo

```bash
python3 dc_motor_pwm_demo.py --vbus 12 --duty 25
python3 dc_motor_pwm_demo.py --vbus 12 --duty 50
python3 dc_motor_pwm_demo.py --vbus 12 --duty -50
```

Bandingkan hasil perhitungan.

---

## 9. Praktikum F — Risk Assessment Sederhana

Setiap kelompok mengisi minimal 8 baris.

| Aktivitas | Hazard | Akibat | Pengendalian | Residual risk |
|---|---|---|---|---|
| wiring VFD | salah terminal | kerusakan/kejutan listrik | power OFF + verifikasi | |
| power PCB | short VBUS | MOSFET rusak | current limit + fuse | |
| arah motor | command salah | gerakan mendadak | interlock + area aman | |
| | | | | |

Minimal bahas:

- listrik AC,
- VBUS DC,
- motor bergerak,
- rotating shaft,
- salah polaritas,
- shoot-through,
- sensor 5 V,
- kehilangan komunikasi.

---

## 10. Tabel Perbandingan Akhir

Isi sendiri setelah observasi.

| Aspek | VFD + AC | DC H-bridge | Inverter 1φ | BLDC six-step | FOC |
|---|---|---|---|---|---|
| Motor/plant | | | | | |
| Power converter | | | | | |
| Command | | | | | |
| Sensor | | | | | |
| Variabel kontrol | | | | | |
| Kelebihan | | | | | |
| Kekurangan | | | | | |
| Risiko utama | | | | | |

---

## 11. Pertanyaan Analisis

1. Mengapa PLC tidak langsung memberikan U/V/W ke motor?
2. Apa perbedaan power path, control path, dan feedback path?
3. Apa beda command speed dengan actual speed?
4. Mengapa duty 50% tidak menjamin motor selalu berputar pada 50% kecepatan maksimum?
5. Apa fungsi dead-time?
6. Mengapa BLDC memerlukan komutasi elektronik?
7. Apa beda six-step dengan FOC?
8. Mengapa sensing current menjadi penting pada FOC?
9. Apa akibat jika high-side dan low-side satu leg ON bersamaan?
10. Mengapa power-stage diuji dengan current-limited supply pada tahap awal?
11. Mengapa `DRV_SD` hardware tetap diperlukan walaupun firmware dapat mengatur PWM 0%?
12. Apa keuntungan RS485 dibanding banyak kabel digital?

---

## 12. Deliverable

Dikumpulkan:

1. diagram power/control/feedback,
2. tabel inventaris perangkat,
3. tabel perbandingan sistem,
4. screenshot hasil tiga program Python,
5. risk assessment,
6. jawaban pertanyaan analisis,
7. foto identifikasi PCB tanpa VBUS.

---

## 13. Kriteria Kelulusan

Mahasiswa dinyatakan lulus sesi bila:

- mampu membedakan jalur energi dan informasi,
- dapat menunjuk komponen utama PCB Rev.E,
- memahami fail-safe shutdown,
- dapat menjelaskan hubungan P1 dengan P2–P16,
- seluruh program Python berhasil dijalankan,
- tidak melakukan pengujian berbahaya untuk sekadar memenuhi dokumentasi.