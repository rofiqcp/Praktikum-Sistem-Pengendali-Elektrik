# Jobsheet 03 — Karakterisasi Reference VFD 4–20 mA

## 1. Tujuan

Menguji end-to-end:

```text
Frequency command → PLC AO → 4–20 mA → VFD analog input → output frequency
```

serta menghitung scaling dan error.

---

## 2. Peralatan

- PLC WECON + analog output current yang sesuai, atau current calibrator yang disetujui,
- VFD WECON,
- motor AC,
- multimeter yang mampu membaca mA,
- laptop,
- Python 3,
- manual PLC/AO dan VFD.

---

## 3. Pre-Lab

Isi sebelum wiring:

| Item | Hasil |
|---|---|
| PLC/model AO | |
| AO channel | |
| AO range | |
| RAW_MIN | |
| RAW_MAX | |
| active/passive output | |
| VFD model | |
| current input terminal | |
| analog COM | |
| current/voltage selector | |
| frequency source parameter | |
| lower scaling parameter | |
| upper scaling parameter | |

---

## 4. Verifikasi Rumus dengan Python

```bash
cd Modul-03-VFD-4-20mA/program
python3 scaling_4_20mA.py --current 4 --fmin 0 --fmax 50
python3 scaling_4_20mA.py --current 12 --fmin 0 --fmax 50
python3 scaling_4_20mA.py --current 20 --fmin 0 --fmax 50
python3 scaling_4_20mA.py --frequency 37.5 --fmin 0 --fmax 50
```

Catat hasil.

---

## 5. Generate Data Sheet

```bash
python3 generate_characterization.py --fmin 0 --fmax 50 --output hasil_pengamatan.csv
```

Buka CSV di spreadsheet.

---

## 6. Wiring dan Measurement

### Current measurement

Current meter dipasang seri:

```text
PLC AO+ → meter mA → VFD current input
PLC AO-/COM ───────→ VFD analog COM
```

Periksa manual untuk wiring aktual.

### Sebelum power/run

- meter pada jack dan range yang benar,
- VFD input diset current,
- frequency source = analog,
- RUN masih OFF.

---

## 7. Test A — AO tanpa Motor RUN

Command bertahap:

| Point | Command mA | Measured mA | Deviation mA |
|---:|---:|---:|---:|
| 1 | 4 | | |
| 2 | 8 | | |
| 3 | 12 | | |
| 4 | 16 | | |
| 5 | 20 | | |

Jika current output tidak benar, jangan lanjut ke motor.

---

## 8. Test B — Static Frequency Characterization

Setelah AO benar, jalankan motor sesuai prosedur P2.

Untuk setiap titik:

1. set current,
2. tunggu steady-state,
3. baca current meter,
4. baca VFD reference,
5. baca VFD output frequency,
6. catat fault/status.

| I cmd | I measured | F theory | F ref display | F output | Error output |
|---:|---:|---:|---:|---:|---:|
| 4 | | | | | |
| 8 | | | | | |
| 12 | | | | | |
| 16 | | | | | |
| 20 | | | | | |

---

## 9. Test C — Intermediate Points

Tambahkan minimal tiga titik yang bukan kelipatan utama, contoh:

```text
6 mA, 13 mA, 18 mA
```

Tujuannya memastikan analisis tidak hanya cocok pada titik yang sudah diketahui.

---

## 10. Test D — Below-Range

Dengan metode aman yang disetujui pengajar, observasi kondisi di bawah range valid tanpa membuat motor bergerak tidak terkendali.

Catat:

- current,
- reference display,
- VFD behavior,
- alarm/fault jika ada,
- apakah setting loss-of-signal tersedia.

Jangan mengubah fault threshold tanpa memahami manual.

---

## 11. Test E — Dynamic Step

Setelah static test lulus:

```text
8 mA → 16 mA
```

Catat waktu dan output frequency setiap interval yang memungkinkan.

Analisis apakah response terutama dibatasi oleh:

- VFD acceleration time,
- analog filter,
- motor dynamics.

---

## 12. Isi CSV dan Analisis

Kolom CSV minimal:

```text
current_command_mA
current_measured_mA
frequency_theory_Hz
frequency_reference_Hz
frequency_output_Hz
```

Jalankan:

```bash
python3 analyze_characterization.py hasil_pengamatan.csv
```

Program menghitung:

- jumlah titik valid,
- slope measured,
- intercept,
- maximum absolute error,
- RMSE.

Jika matplotlib tersedia:

```bash
python3 analyze_characterization.py hasil_pengamatan.csv --plot
```

---

## 13. Parameter VFD yang Dicatat

| Function | Parameter | Value |
|---|---|---|
| run source | | |
| frequency source | | |
| analog input type | | |
| lower current | | |
| upper current | | |
| lower Hz | | |
| upper Hz | | |
| analog filter | | |
| acceleration | | |
| deceleration | | |
| loss signal behavior | | |

---

## 14. Error Budget Sederhana

Diskusikan kontribusi:

| Source | Possible effect |
|---|---|
| PLC AO resolution | quantization |
| AO calibration | gain/offset |
| multimeter | measurement uncertainty |
| VFD ADC | gain/offset |
| analog filter | dynamic lag |
| ramp | output frequency delay |
| wiring/noise | fluctuation |

---

## 15. Troubleshooting Record

| Symptom | Test | Root cause | Correction |
|---|---|---|---|
| | | | |
| | | | |

Minimal dua kasus.

---

## 16. Pertanyaan Analisis

1. Hitung theoretical frequency untuk 7 mA pada range 0–50 Hz.
2. Hitung current command untuk 35 Hz.
3. Apa beda current command dan current measured?
4. Mengapa 4 mA adalah valid minimum, bukan fault secara otomatis?
5. Apa yang terjadi jika frequency source masih keypad?
6. Mengapa VFD reference dan output frequency dapat berbeda saat transien?
7. Apa arti slope measured yang berbeda dari slope ideal?
8. Mengapa data abnormal tidak boleh dihapus tanpa catatan?

---

## 17. Deliverable

- parameter map,
- wiring diagram,
- CSV raw data,
- screenshot/plot,
- hasil analisis Python,
- perhitungan manual dua titik,
- troubleshooting record,
- jawaban analisis.

---

## 18. Kriteria Lulus

- 4–20 mA terukur dengan metode yang benar,
- lima titik utama selesai,
- scaling teoritis benar,
- mahasiswa dapat menjelaskan error,
- source parameter berasal dari manual unit aktual,
- kondisi abnormal ditangani tanpa menjalankan motor secara berbahaya.