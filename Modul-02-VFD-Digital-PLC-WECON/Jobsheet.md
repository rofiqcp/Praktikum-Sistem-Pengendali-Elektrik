# Jobsheet 02 — RUN/STOP, FWD/REV, dan Multi-Speed VFD WECON via PLC WECON

## 1. Tujuan

Membuat sistem kontrol digital PLC→VFD yang memenuhi:

- START latch,
- STOP dominan,
- FWD/REV interlock,
- minimum 3 preset speed,
- fault/interlock,
- commissioning bertahap.

---

## 2. Peralatan

- PLC WECON aktual,
- VFD WECON aktual,
- motor AC,
- push button START,
- push button STOP NC,
- selector FWD/REV,
- selector speed,
- multimeter,
- laptop dengan WECON PLC Editor/PLC Editor2 sesuai seri,
- Python 3 untuk simulator,
- manual PLC dan VFD.

---

## 3. Pre-Lab — Wajib Sebelum Wiring

Isi:

| Item | Hasil |
|---|---|
| Model PLC | |
| Software PLC | |
| Supply PLC | |
| Output PLC relay/transistor | |
| Model VFD | |
| Supply VFD | |
| Motor rated voltage | |
| Motor rated current | |
| Motor rated frequency | |
| DI VFD NPN/PNP | |
| Common terminal | |

Jika tabel belum lengkap, jangan lanjut ke RUN motor.

---

## 4. I/O Map

Isi file `program/io_map_template.csv` dan tabel berikut:

| Symbol | Address | Terminal fisik | Fungsi |
|---|---|---|---|
| START_PB | | | |
| STOP_OK | | | |
| FWD_SEL | | | |
| REV_SEL | | | |
| SPEED_BIT0 | | | |
| SPEED_BIT1 | | | |
| VFD_OK | | | |
| DO_FWD | | | |
| DO_REV | | | |
| DO_MS0 | | | |
| DO_MS1 | | | |

---

## 5. Parameter Map VFD

Isi berdasarkan manual unit aktual:

| Function | Parameter | Original | New value |
|---|---|---|---|
| run source | | | |
| frequency source | | | |
| DI FWD | | | |
| DI REV | | | |
| DI MS0 | | | |
| DI MS1 | | | |
| preset 0 | | | |
| preset 1 | | | |
| preset 2 | | | |
| preset 3 | | | |
| accel | | | |
| decel | | | |
| maximum Hz | | | |

Foto halaman manual dan parameter display untuk lampiran.

---

## 6. Simulasi Logic Sebelum Hardware

```bash
cd Modul-02-VFD-Digital-PLC-WECON/program
python3 digital_logic_sim.py
```

Expected:

- tidak ada test yang menghasilkan FWD+REV bersamaan,
- STOP/fault selalu mematikan RUN,
- speed select menghasilkan kode 0..3.

Lalu jalankan interactive mode:

```bash
python3 digital_logic_sim.py --interactive
```

Masukkan kombinasi input yang diminta asisten.

---

## 7. Program PLC

Gunakan `wecon_digital_logic.st` sebagai referensi logic. Implementasikan ke language yang didukung dan diwajibkan oleh CPU/editor laboratorium, umumnya ladder untuk praktikum ini.

### Rung minimum

1. permit,
2. START/STOP latch,
3. FWD output,
4. REV output,
5. MS0,
6. MS1,
7. lamp RUN,
8. lamp fault/conflict bila output tersedia.

---

## 8. Test 1 — PLC Output Tanpa VFD RUN

Motor belum dijalankan.

| Test | START | STOP_OK | FWD | REV | MS1:MS0 | Expected |
|---:|---:|---:|---:|---:|---|---|
| 1 | 0 | 1 | 0 | 0 | 00 | all off |
| 2 | pulse | 1 | 1 | 0 | 00 | FWD on |
| 3 | pulse | 1 | 0 | 1 | 00 | REV on |
| 4 | pulse | 1 | 1 | 1 | 00 | FWD/REV both off |
| 5 | pulse | 0 | 1 | 0 | 00 | all run off |
| 6 | pulse | 1 | 1 | 0 | 11 | FWD + speed code 3 |

Catat hasil actual Y output.

---

## 9. Test 2 — Verifikasi Digital Input VFD

VFD tetap STOP / motor tidak diizinkan bergerak.

Aktifkan satu output PLC pada satu waktu dan lihat terminal monitor/indikator VFD.

| PLC output | Voltage measured | VFD DI detected | PASS/FAIL |
|---|---:|---|---|
| FWD | | | |
| REV | | | |
| MS0 | | | |
| MS1 | | | |

Jika DI tidak sesuai, **jangan lanjut ke motor**.

---

## 10. Test 3 — Forward Low Speed

1. Area motor aman.
2. Set preset terendah.
3. Pilih FWD.
4. Tekan START.
5. Amati output frequency.
6. Catat acceleration time aktual.
7. Tekan STOP.
8. Catat deceleration time.

| Parameter | Nilai |
|---|---:|
| preset Hz | |
| output Hz steady | |
| acceleration time | |
| deceleration time | |
| direction | |
| fault | |

---

## 11. Test 4 — Reverse

Wajib:

1. STOP,
2. output frequency 0 Hz,
3. motor berhenti,
4. baru pilih REV,
5. START.

Catat sama seperti forward.

---

## 12. Test 5 — Multi-Speed

Uji seluruh kombinasi.

| MS1 | MS0 | Set Hz | Output Hz | Error Hz | PASS |
|---:|---:|---:|---:|---:|---|
| 0 | 0 | | | | |
| 0 | 1 | | | | |
| 1 | 0 | | | | |
| 1 | 1 | | | | |

---

## 13. Test 6 — Interlock

Lakukan hanya simulasi aman yang disetujui pengajar.

### A. Direction conflict
Aktifkan FWD dan REV selector bersamaan.

Expected:

```text
DO_FWD = 0
DO_REV = 0
DIR_CONFLICT = 1
```

### B. STOP
Saat RUN, tekan STOP.

Expected:

```text
RUN_LATCH = 0
DO_FWD = 0
DO_REV = 0
```

### C. VFD_OK false
Simulasikan input permit false tanpa menyebabkan fault berbahaya.

Expected sama: RUN OFF.

---

## 14. Troubleshooting Record

Minimal isi dua kejadian, walaupun hanya salah setting kecil.

| Symptom | Suspected cause | Test performed | Root cause | Fix |
|---|---|---|---|---|
| | | | | |
| | | | | |

---

## 15. Analisis

Jawab:

1. Mengapa logic software saja tidak cukup jika wiring common salah?
2. Mengapa commissioning dipisah antara PLC output test dan motor RUN?
3. Apa perbedaan run source dengan frequency source?
4. Mengapa preset frequency berguna pada kontrol digital sederhana?
5. Bagaimana sistem seharusnya bereaksi jika dua arah dipilih bersamaan?
6. Apa yang harus terjadi ketika fault muncul ketika motor sedang running?

---

## 16. Deliverable

- file project PLC,
- screenshot ladder online monitor,
- I/O map,
- parameter map,
- tabel seluruh pengujian,
- screenshot simulator Python,
- foto wiring control,
- troubleshooting record,
- jawaban analisis.

---

## 17. Kriteria Lulus

- STOP dominan,
- FWD/REV tidak pernah simultan,
- minimum 3 preset speed bekerja,
- fault/interlock mematikan RUN,
- parameter dibuktikan dari manual unit aktual,
- wiring NPN/PNP/common dijelaskan dengan benar.