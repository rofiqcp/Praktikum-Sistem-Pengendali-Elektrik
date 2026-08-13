# Modul 03 — VFD WECON dengan Reference Analog 4–20 mA

## Praktikum Sistem Pengendali Elektrik

Pertemuan ini memindahkan frequency reference VFD dari pilihan digital/preset pada P2 menjadi **sinyal analog arus 4–20 mA**. Fokus utama bukan hanya membuat motor berubah kecepatan, tetapi memahami loop arus, scaling, kalibrasi, error, wiring, dan diagnosis kondisi sinyal abnormal.

> Terminal analog, jumper current/voltage, parameter source, range input, serta modul analog output PLC berbeda antar model WECON. Semua setting harus diverifikasi dari manual unit laboratorium.

---

## 1. Capaian Pembelajaran

Mahasiswa mampu:

1. Menjelaskan alasan penggunaan 4–20 mA di industri.
2. Menjelaskan konsep `live zero` pada 4 mA.
3. Membedakan current output aktif, loop-powered/passive device, dan input analog VFD.
4. Mengidentifikasi terminal analog input dan analog common VFD.
5. Mengidentifikasi modul/kanal analog output PLC yang mendukung arus.
6. Melakukan scaling 4–20 mA menjadi frekuensi.
7. Melakukan inverse scaling dari frekuensi menjadi current command.
8. Menghubungkan raw digital value PLC dengan engineering unit mA dan Hz.
9. Mengukur arus loop secara benar dengan multimeter.
10. Menguji lima titik 4, 8, 12, 16, dan 20 mA.
11. Menghitung slope, offset, error, dan linearity sederhana.
12. Mengidentifikasi kondisi below-range/loop abnormal.
13. Menjalankan script Python untuk membuat template data dan menganalisis hasil pengamatan.

---

## 2. Mengapa 4–20 mA?

Sinyal analog dapat berupa tegangan atau arus. Pada lingkungan industri, current loop banyak digunakan karena:

- lebih tahan terhadap pengaruh resistansi kabel dalam batas compliance sumber,
- relatif robust untuk jarak lebih panjang,
- mudah diukur secara seri,
- memiliki `live zero`.

### Live zero

Jika nilai minimum proses dipetakan ke 4 mA, maka:

```text
4 mA  = nilai minimum yang VALID
0 mA  = bukan nilai minimum normal
```

Dengan demikian kehilangan sinyal atau loop terbuka dapat dibedakan dari command minimum, tergantung konfigurasi perangkat.

---

## 3. Arsitektur P3

```text
Setpoint Hz
   ↓ scaling
PLC / Analog Output
   ↓ 4–20 mA
Analog Input VFD
   ↓ internal scaling
Frequency Reference
   ↓
VFD inverter
   ↓ U/V/W
Motor AC
```

RUN/STOP dapat tetap menggunakan digital terminal dari P2. Dengan cara ini mahasiswa melihat bahwa:

- **RUN source** dapat berasal dari terminal digital,
- **frequency source** dapat berasal dari analog current.

Keduanya tidak harus berasal dari sumber yang sama.

---

## 4. Periksa Kemampuan Hardware

Sebelum wiring, tentukan:

### PLC / analog module

- apakah memiliki analog output?
- apakah output mendukung current?
- range 0–20 mA atau 4–20 mA?
- berapa raw digital minimum/maksimum?
- apakah output active source atau memerlukan loop supply eksternal?
- berapa load/burden yang diizinkan?

### VFD

- terminal analog input mana yang mendukung current?
- apakah perlu jumper untuk memilih current/voltage?
- range input 0–20 mA atau 4–20 mA?
- analog common terminal?
- parameter frequency source?
- parameter lower/upper analog scaling?

Beberapa seri WECON mendukung input analog yang dapat dipilih sebagai voltage/current, tetapi terminal dan parameternya harus mengikuti seri aktual.

---

## 5. Mengukur Current Loop dengan Multimeter

Untuk mengukur arus, multimeter dipasang **seri** pada loop.

```text
AO+ ──→ Multimeter A ──→ AI VFD
AO- / COM ─────────────→ Analog COM VFD
```

Jangan memperlakukan current measurement seperti voltage measurement.

Kesalahan umum:

- probe meter masih di jack voltage,
- meter pada mode voltage tetapi diharapkan membaca mA,
- meter current dipasang paralel terhadap supply,
- common analog salah,
- input VFD masih mode voltage,
- AO PLC belum diset current mode.

> Lakukan wiring saat sistem dalam kondisi yang ditentukan aman oleh prosedur laboratorium.

---

## 6. Persamaan Scaling 4–20 mA → Frequency

Untuk range:

```text
Imin = 4 mA
Imax = 20 mA
Fmin = frequency minimum
Fmax = frequency maximum
```

Normalized command:

```text
x = (I - 4) / (20 - 4)
  = (I - 4) / 16
```

Frequency:

```text
F = Fmin + x × (Fmax - Fmin)
```

Sehingga:

```text
F = Fmin + (I - 4)/16 × (Fmax - Fmin)
```

### Contoh 0–50 Hz

Jika:

```text
4 mA  → 0 Hz
20 mA → 50 Hz
```

maka:

| Current | Normalized | Frequency |
|---:|---:|---:|
| 4 mA | 0.00 | 0 Hz |
| 8 mA | 0.25 | 12.5 Hz |
| 12 mA | 0.50 | 25 Hz |
| 16 mA | 0.75 | 37.5 Hz |
| 20 mA | 1.00 | 50 Hz |

---

## 7. Inverse Scaling Frequency → Current

Jika PLC menerima setpoint dalam Hz dan harus menghasilkan current command:

```text
x = (F - Fmin) / (Fmax - Fmin)
I = 4 + 16 × x
```

atau:

```text
I = 4 + 16 × (F - Fmin)/(Fmax - Fmin)
```

Gunakan clamp agar setpoint tidak menghasilkan command di luar range yang direncanakan.

---

## 8. Raw Digital Value → mA

Modul analog biasanya menerima nilai digital tertentu. Jangan menebak range raw.

Misalkan manual menyatakan:

```text
RAW_MIN → 4 mA
RAW_MAX → 20 mA
```

Maka:

```text
I = 4 + (RAW - RAW_MIN)/(RAW_MAX - RAW_MIN) × 16
```

Sebaliknya:

```text
RAW = RAW_MIN + (I - 4)/16 × (RAW_MAX - RAW_MIN)
```

### Prinsip penting

Ada tiga domain yang harus dibedakan:

```text
Engineering command: Hz
       ↓
PLC internal value: raw/count
       ↓
Physical signal: mA
       ↓
VFD engineering value: Hz
```

Kesalahan scaling sering terjadi karena dua domain dianggap sama.

---

## 9. Clamp dan Validasi

Untuk command normal:

```text
Icmd = clamp(Icmd, 4, 20)
Fcmd = clamp(Fcmd, Fmin, Fmax)
```

Untuk measurement, jangan langsung clamp sebelum diagnosis. Nilai di bawah 4 mA dapat menjadi informasi kondisi below-range atau loop abnormal.

Contoh klasifikasi edukasional:

```text
I < 4 mA   → BELOW_RANGE
4..20 mA   → VALID_RANGE
I > 20 mA  → ABOVE_RANGE
```

Batas fault final harus mengikuti manual/perangkat dan kebijakan sistem aktual.

---

## 10. Scaling pada VFD

VFD dapat memiliki parameter seperti:

- analog input type,
- frequency source,
- lower input,
- upper input,
- lower frequency,
- upper frequency,
- filter time,
- loss-of-signal behavior.

Nama dan nomor parameter berbeda antar seri.

Tabel yang harus diisi:

| Function | Parameter actual | Setting P3 |
|---|---|---|
| frequency source | | analog current |
| analog input type | | current |
| current lower point | | 4 mA |
| current upper point | | 20 mA |
| lower frequency | | sesuai praktikum |
| upper frequency | | sesuai praktikum |
| input filter | | catat |
| signal-loss behavior | | catat |

---

## 11. Kalibrasi 5 Titik

Gunakan titik:

```text
4, 8, 12, 16, 20 mA
```

Untuk setiap titik catat:

- current command,
- current measured,
- theoretical Hz,
- VFD reference display,
- VFD output Hz,
- error.

Error terhadap theoretical reference:

```text
Error_Hz = F_measured - F_theory
```

Absolute error:

```text
AbsError = |Error_Hz|
```

Percentage of span:

```text
Error_span_% = Error_Hz/(Fmax-Fmin) × 100%
```

---

## 12. Slope dan Offset

Hubungan ideal:

```text
F = mI + b
```

Untuk 4–20 mA → 0–50 Hz:

```text
m = (50 - 0)/(20 - 4)
  = 3.125 Hz/mA

b = Fmin - m×Imin
  = -12.5 Hz
```

Pengukuran nyata dapat menghasilkan slope dan intercept sedikit berbeda. P3 menggunakan Python untuk menghitung fit linear sederhana.

---

## 13. Sumber Error

### Dari PLC/AO

- DAC resolution,
- current output calibration,
- output load,
- supply/compliance.

### Dari wiring

- common salah,
- loose terminal,
- noise,
- shield/grounding tidak baik.

### Dari measurement

- multimeter accuracy,
- meter burden,
- probe/setup.

### Dari VFD

- analog input calibration,
- filter,
- scaling parameter,
- display resolution.

### Dari dynamic response

Jika pengambilan data terlalu cepat, output frequency mungkin masih dalam acceleration ramp. Tunggu steady-state untuk pengujian static scaling.

---

## 14. Dynamic Test

Setelah static 5-point test berhasil, lakukan step:

```text
8 mA → 16 mA
```

Catat:

- waktu command berubah,
- reference display,
- output frequency,
- waktu mencapai steady-state.

Perubahan yang terlihat dipengaruhi oleh:

- analog filtering,
- acceleration/deceleration setting,
- motor dynamics.

P3 belum melakukan identifikasi model dinamik secara formal; fokus tetap pada reference chain.

---

## 15. Software Flow

```text
User enters F_target
      ↓ clamp
Frequency-to-current scaling
      ↓
Current-to-raw scaling
      ↓
Analog output module
      ↓ physical 4–20 mA
VFD input scaling
      ↓
Frequency reference
```

Jika feedback analog output aktual tersedia, sistem dapat menambahkan diagnostic check.

---

## 16. Structured Text Reference

File `wecon_4_20mA_scaling.st` menyediakan template:

- clamp frequency,
- normalize,
- generate mA command,
- convert to raw AO value,
- classify measured current.

`RAW_MIN` dan `RAW_MAX` harus diisi dari manual module AO aktual.

---

## 17. Program Python yang Harus Dijalankan

### A. Scaling calculator

```bash
python3 scaling_4_20mA.py --current 12 --fmin 0 --fmax 50
python3 scaling_4_20mA.py --frequency 25 --fmin 0 --fmax 50
```

### B. Generate template

```bash
python3 generate_characterization.py --fmin 0 --fmax 50 --output hasil_pengamatan.csv
```

### C. Analyze data

Setelah kolom pengukuran diisi:

```bash
python3 analyze_characterization.py hasil_pengamatan.csv
```

Untuk plot:

```bash
pip install -r requirements.txt
python3 analyze_characterization.py hasil_pengamatan.csv --plot
```

---

## 18. Data Integrity

Jangan mengubah data agar terlihat linear. Jika satu titik salah:

1. catat data original,
2. cari penyebab,
3. ulangi pengukuran,
4. simpan nilai repeat,
5. jelaskan mengapa data pertama ditolak/diterima.

Praktikum pengendalian harus melatih traceability data.

---

## 19. Troubleshooting

### 0 mA terbaca

- AO belum aktif,
- loop putus,
- common salah,
- mode output salah,
- meter setup salah.

### 4–20 mA benar tetapi VFD Hz salah

- analog input type,
- frequency source,
- lower/upper scaling,
- VFD analog calibration.

### Nilai noisy

- routing signal dekat power cable,
- grounding/shield,
- loose terminal,
- filter setting.

### 20 mA tidak tercapai

- range raw salah,
- output load/compliance,
- AO configuration,
- calibration.

---

## 20. Pertanyaan Evaluasi

1. Mengapa 4 mA disebut live zero?
2. Mengapa current meter dipasang seri?
3. Apa beda Hz command, raw AO, mA, dan Hz display VFD?
4. Bagaimana rumus 4–20 mA → 0–50 Hz?
5. Bagaimana inverse scaling 25 Hz → mA?
6. Mengapa raw range tidak boleh ditebak?
7. Mengapa static test harus menunggu ramp selesai?
8. Sebutkan empat sumber error.
9. Apa fungsi clamp pada command?
10. Mengapa measurement below-range tidak selalu boleh langsung di-clamp?

---

## 21. Referensi

- Manual analog output PLC/module WECON aktual.
- Manual analog input VFD WECON aktual.
- Dokumentasi resmi WECON untuk seri VFD yang digunakan.

> Model VFD dan PLC harus ditulis pada laporan agar hasil dapat direplikasi.