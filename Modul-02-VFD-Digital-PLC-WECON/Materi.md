# Modul 02 — VFD WECON dengan Kontrol Digital dari PLC WECON

## Praktikum Sistem Pengendali Elektrik

Pertemuan ini mempelajari cara **PLC memberikan command digital ke VFD**. Mahasiswa tidak langsung masuk ke analog atau Modbus. Tujuannya adalah memahami dengan sangat jelas hubungan antara logika PLC, terminal digital VFD, interlock, arah putar, preset speed, dan kondisi fault.

> Nomor terminal, alamat X/Y PLC, parameter VFD, dan jenis sinking/sourcing harus diverifikasi dari **model PLC dan VFD aktual** di laboratorium. Seri WECON berbeda dapat menggunakan software, alamat, dan parameter yang berbeda.

---

## 1. Capaian Pembelajaran

Setelah praktikum, mahasiswa mampu:

1. Mengidentifikasi model PLC WECON dan VFD WECON yang dipakai.
2. Membedakan input PLC, output PLC, input digital VFD, relay/status output VFD, dan power terminal.
3. Memeriksa apakah output PLC bertipe relay atau transistor.
4. Memeriksa konfigurasi NPN/sinking atau PNP/sourcing.
5. Membuat logic START/STOP dengan latch.
6. Membuat FWD/REV dengan interlock mutual exclusion.
7. Membuat minimum tiga preset/multi-speed.
8. Memblokir RUN ketika fault/interlock tidak valid.
9. Menjamin STOP memiliki prioritas tertinggi.
10. Menguji logika secara software sebelum motor dijalankan.
11. Melakukan commissioning bertahap dari PLC output → DI VFD → motor.
12. Mendokumentasikan parameter VFD berdasarkan manual unit aktual.

---

## 2. Arsitektur Sistem

```text
Push Button / Selector
        ↓
    PLC WECON
        ↓ Digital Output Y
        ↓
  Digital Input VFD
        ↓
 Internal VFD Control
        ↓
 Power Stage VFD
        ↓ U/V/W
     Motor AC
```

Status/fault dapat dikembalikan ke PLC melalui relay/digital output VFD jika tersedia:

```text
VFD RUN/FAULT relay → PLC X input → interlock program
```

PLC **tidak** membangkitkan U/V/W motor. PLC hanya menentukan command logic.

---

## 3. Identifikasi PLC WECON

Sebelum membuat program, catat:

- seri PLC,
- tipe CPU,
- supply CPU,
- jumlah digital input,
- jumlah digital output,
- output relay/transistor,
- common input,
- common output,
- software editor yang sesuai seri.

Dokumentasi resmi WECON membedakan keluarga PLC lama seperti LX3V/LX3VP/LX3VE/LX3VM dari keluarga yang menggunakan PLC Editor2. Karena itu jangan menganggap semua seri memakai workflow identik.

### 3.1 Digital input PLC

Input PLC biasanya membaca keadaan tombol/sensor.

Contoh mapping konseptual:

| Input | Fungsi |
|---|---|
| X0 | START |
| X1 | STOP_NC |
| X2 | selector FWD |
| X3 | selector REV |
| X4 | speed select bit 0 |
| X5 | speed select bit 1 |
| X6 | VFD fault OK |

Alamat tersebut **contoh pembelajaran**, bukan kewajiban untuk semua CPU.

### 3.2 Digital output PLC

Contoh:

| Output | Fungsi |
|---|---|
| Y0 | VFD FWD |
| Y1 | VFD REV |
| Y2 | multi-speed bit 0 |
| Y3 | multi-speed bit 1 |
| Y4 | lamp RUN |
| Y5 | lamp FAULT |

---

## 4. Relay Output vs Transistor Output

### 4.1 Relay output

Kelebihan:

- galvanic isolation secara mekanik,
- lebih fleksibel untuk berbagai tegangan dalam rating,
- mudah dipahami.

Kekurangan:

- lebih lambat,
- umur kontak terbatas,
- tidak cocok untuk switching frekuensi tinggi.

### 4.2 Transistor output

Kelebihan:

- cepat,
- tidak memiliki kontak mekanik.

Kekurangan:

- polaritas/sinking/sourcing harus benar,
- rating tegangan/arus lebih ketat.

Sebelum menghubungkan ke DI VFD, pastikan **jenis output PLC kompatibel dengan input VFD**.

---

## 5. Konsep Sinking / Sourcing, NPN / PNP

Istilah sering membingungkan karena perspektif input dan output dapat berbeda.

Yang wajib dilakukan mahasiswa bukan menghafal satu wiring, tetapi membaca diagram terminal unit aktual dan menentukan:

1. Dari mana arus control berasal?
2. Ke mana arus kembali?
3. Apa fungsi COM/S/S/SC/24V pada unit tersebut?
4. Apakah output PLC menghubungkan load ke 0 V atau ke +24 V?
5. Apakah DI VFD dikonfigurasi untuk NPN atau PNP?

Dokumentasi WECON pada beberapa seri VFD menyediakan konfigurasi input digital NPN/PNP. Namun detail jumper/terminal harus mengikuti seri yang benar.

### Aturan commissioning

Sebelum VFD diberi command RUN:

- ukur tegangan logic dengan multimeter,
- uji output PLC tanpa motor,
- pastikan DI VFD berubah sesuai indikator/display,
- baru izinkan RUN pada frekuensi rendah.

---

## 6. Command Source pada VFD

VFD biasanya dapat menerima RUN dari beberapa sumber, misalnya:

- keypad,
- terminal digital,
- komunikasi.

Untuk P2, pilih **terminal digital** sebagai run command source sesuai manual model aktual.

Frequency reference dapat berasal dari:

- keypad,
- preset/multi-speed melalui DI,
- analog,
- komunikasi.

Pada P2 gunakan preset/multi-speed agar seluruh praktik tetap berbasis digital.

---

## 7. RUN Latch

START biasanya berupa push button sesaat. Karena itu dibutuhkan memory/latch.

Logika dasar:

```text
IF STOP tidak valid → RUN_LATCH = 0
ELSE IF FAULT → RUN_LATCH = 0
ELSE IF START → RUN_LATCH = 1
```

STOP harus lebih dominan daripada START.

### Mengapa STOP dominan?

Jika START dan STOP kebetulan terbaca bersamaan, kondisi aman adalah tidak menjalankan motor.

---

## 8. FWD / REV Interlock

Kondisi yang dilarang:

```text
FWD = 1 dan REV = 1
```

Logika aman:

```text
FWD_CMD = RUN_LATCH AND FWD_SEL AND NOT REV_SEL
REV_CMD = RUN_LATCH AND REV_SEL AND NOT FWD_SEL
```

Jika kedua selector aktif, kedua command harus OFF dan dapat menghasilkan alarm `DIR_CONFLICT`.

### Pergantian arah

Untuk praktikum dasar:

1. tekan STOP,
2. tunggu output frequency turun ke 0,
3. tunggu motor berhenti,
4. pilih arah baru,
5. START kembali.

Jangan membuat pembalikan mendadak hanya untuk menunjukkan bahwa program bisa reverse.

---

## 9. Multi-Speed / Preset Frequency

Dua digital input dapat memilih empat kombinasi.

| MS1 | MS0 | Preset |
|---:|---:|---|
| 0 | 0 | Speed 0 |
| 0 | 1 | Speed 1 |
| 1 | 0 | Speed 2 |
| 1 | 1 | Speed 3 |

Nilai Hz setiap preset ditentukan pada parameter VFD sesuai manual.

Contoh target aman untuk motor unloaded **hanya jika diizinkan pengajar**:

- Speed 0 = 5 Hz,
- Speed 1 = 10 Hz,
- Speed 2 = 20 Hz,
- Speed 3 = 30 Hz.

Nilai di atas adalah contoh laboratorium, bukan rating universal motor.

---

## 10. Acceleration dan Deceleration Time

VFD tidak langsung melompat dari 0 ke frekuensi target. Ramp digunakan untuk membatasi perubahan kecepatan/arus.

### Acceleration time
Waktu yang digunakan untuk menaikkan command sesuai definisi VFD.

### Deceleration time
Waktu untuk menurunkan command.

Deceleration terlalu cepat dapat menyebabkan peningkatan tegangan DC bus pada sistem tertentu karena energi mekanik kembali ke drive.

Untuk P2, gunakan nilai konservatif yang diberikan dosen/laboran.

---

## 11. Fault dan Interlock

Program yang baik tidak hanya bekerja pada kondisi normal.

Contoh interlock:

- VFD fault,
- emergency stop,
- guard open,
- overload relay,
- motor overtemperature,
- komunikasi/status tidak valid bila diperlukan.

Logika:

```text
PERMIT_RUN = ESTOP_OK AND VFD_OK AND GUARD_OK
RUN_LATCH hanya boleh TRUE jika PERMIT_RUN = TRUE
```

Jika `PERMIT_RUN` menjadi false ketika motor running:

```text
RUN_LATCH → FALSE
FWD → FALSE
REV → FALSE
```

---

## 12. State Machine yang Disarankan

Selain ladder latch, mahasiswa diperkenalkan pada state machine:

```text
STOPPED
   ↓ START + permit
RUN_FWD / RUN_REV
   ↓ STOP/fault
STOPPED

Jika selector konflik:
DIR_CONFLICT → output OFF
```

State machine membantu ketika sistem mulai kompleks.

---

## 13. Contoh I/O Mapping

Buat tabel sebelum coding:

| Symbol | PLC address | Type | Normal state | Description |
|---|---|---|---|---|
| START_PB | X0 | DI | 0 | push button START |
| STOP_OK | X1 | DI | 1 | NC stop healthy |
| FWD_SEL | X2 | DI | 0 | forward selector |
| REV_SEL | X3 | DI | 0 | reverse selector |
| SPEED_BIT0 | X4 | DI | 0 | preset bit 0 |
| SPEED_BIT1 | X5 | DI | 0 | preset bit 1 |
| VFD_OK | X6 | DI | 1 | no fault/permit |
| DO_FWD | Y0 | DO | 0 | VFD forward DI |
| DO_REV | Y1 | DO | 0 | VFD reverse DI |
| DO_MS0 | Y2 | DO | 0 | preset select 0 |
| DO_MS1 | Y3 | DO | 0 | preset select 1 |

Alamat hanya template.

---

## 14. Parameter Checklist VFD

Sebelum praktikum isi tabel berikut dari manual:

| Function | Parameter actual | Setting | Verified by |
|---|---|---|---|
| motor rated data | | | |
| run command source | | terminal | |
| frequency source | | multi-speed | |
| DI for FWD | | | |
| DI for REV | | | |
| DI for MS0 | | | |
| DI for MS1 | | | |
| preset speed 0 | | | |
| preset speed 1 | | | |
| preset speed 2 | | | |
| preset speed 3 | | | |
| accel time | | | |
| decel time | | | |
| max frequency | | | |
| min frequency | | | |

---

## 15. Ladder Concept

Representasi tekstual:

```text
RUNG 1 — PERMIT
STOP_OK -- VFD_OK --------------------(PERMIT)

RUNG 2 — RUN LATCH
PERMIT -- [ START || RUN_LATCH ] ------(RUN_LATCH)

RUNG 3 — FWD
RUN_LATCH -- FWD_SEL -- /REV_SEL ------(Y_FWD)

RUNG 4 — REV
RUN_LATCH -- REV_SEL -- /FWD_SEL ------(Y_REV)

RUNG 5
SPEED_BIT0 -----------------------------(Y_MS0)

RUNG 6
SPEED_BIT1 -----------------------------(Y_MS1)
```

Pada program final, alamat dan instruction harus dibuat menggunakan editor WECON yang sesuai CPU.

---

## 16. Structured Text Reference

Folder `program/wecon_digital_logic.st` memberikan logic referensi IEC 61131-3. Tidak semua WECON CPU/editor menggunakan Structured Text dengan cara yang sama. Jika CPU praktikum menggunakan ladder, terjemahkan logic tersebut menjadi ladder.

---

## 17. Simulator Python

Sebelum menyalakan VFD jalankan:

```bash
python3 digital_logic_sim.py
```

Simulator menguji beberapa skenario:

- STOP,
- START forward,
- START reverse,
- direction conflict,
- fault,
- preset speed.

Mahasiswa harus memastikan simulator tidak pernah menghasilkan FWD dan REV bersamaan.

---

## 18. Urutan Commissioning P2

### Level 0 — power OFF

- cek wiring,
- cek common,
- cek alamat I/O,
- cek parameter list.

### Level 1 — PLC only

- motor/VFD command belum aktif,
- monitor X/Y,
- tekan tombol,
- verifikasi logic.

### Level 2 — VFD terminal monitor

- VFD STOP,
- verifikasi DI berubah,
- jangan RUN motor dulu.

### Level 3 — low frequency

- motor area aman,
- pilih 5–10 Hz atau nilai yang ditentukan pengajar,
- RUN forward.

### Level 4 — reverse

- STOP,
- tunggu motor berhenti,
- ubah selector,
- RUN reverse.

### Level 5 — multi-speed

- uji semua kombinasi,
- catat reference dan output frequency.

### Level 6 — fault/interlock

- lakukan fault simulation yang aman,
- buktikan RUN terblokir.

---

## 19. Troubleshooting

### PLC output tidak ON
Periksa:

- input START/STOP,
- permit,
- latch,
- alamat output,
- CPU RUN mode.

### Output PLC ON tetapi DI VFD tidak berubah
Periksa:

- wiring common,
- NPN/PNP,
- relay/transistor output,
- tegangan control,
- input terminal VFD.

### DI VFD berubah tetapi motor tidak RUN
Periksa:

- run command source,
- assignment DI,
- fault,
- frequency reference,
- motor wiring.

### Motor hanya satu arah
Periksa:

- assignment DI reverse,
- interlock program,
- selector,
- parameter reverse inhibit bila ada.

### Preset speed tidak sesuai
Periksa:

- assignment multi-speed,
- kombinasi bit,
- preset values,
- frequency source.

---

## 20. Pertanyaan Evaluasi

1. Mengapa STOP harus dominan terhadap START?
2. Mengapa FWD dan REV dibuat mutual-exclusive?
3. Apa beda relay output dan transistor output PLC?
4. Apa arti NPN/PNP dalam wiring control?
5. Mengapa common terminal harus diperiksa sebelum wiring?
6. Mengapa command source VFD perlu diubah dari keypad ke terminal?
7. Bagaimana dua bit memilih empat preset speed?
8. Apa risiko pergantian arah mendadak?
9. Mengapa fault VFD harus memblokir RUN?
10. Apa keuntungan membuat simulator logic sebelum commissioning motor?

---

## 21. Referensi

- Manual resmi PLC WECON sesuai CPU yang digunakan.
- Manual resmi VFD WECON sesuai seri yang digunakan.
- WECON PLC programming documentation untuk instruction `LD`, `LDI`, `OUT`, `AND`, `ANI`, `OR`, dan instruction lain yang relevan.

> **Jangan menyalin parameter dari kelompok lain jika model VFD berbeda.**