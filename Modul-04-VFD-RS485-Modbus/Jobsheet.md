# Jobsheet 04 — VFD WECON via RS485 Modbus RTU

## 1. Tujuan

Mengimplementasikan komunikasi:

```text
Laptop / Python → USB-RS485 → Modbus RTU → VFD WECON
```

dengan tahapan read-only → verified write → monitoring → STOP.

---

## 2. Peralatan

- VFD WECON aktual,
- motor AC,
- laptop,
- USB-RS485 converter,
- twisted pair,
- multimeter,
- Python 3,
- manual VFD aktual.

---

## 3. Pre-Lab Register Mapping

Sebelum menghubungkan motor melalui program, isi `program/register_map_template.csv`.

Minimal temukan:

| Item | Manual address | Access | Scale/value |
|---|---|---|---|
| output frequency monitor | | R | |
| status | | R | |
| fault code | | R | |
| frequency reference | | R/W | |
| command word | | R/W | |
| STOP value | | W | |
| RUN forward value | | W | |
| RUN reverse value | | W | |

Sertakan halaman manual sebagai bukti.

---

## 4. Serial Parameter Sheet

| Setting | Value |
|---|---|
| VFD slave ID | |
| baudrate | |
| parity | |
| stop bits | |
| RS485 terminal + | |
| RS485 terminal - | |
| communication source parameter | |
| communication loss behavior | |

---

## 5. Setup Python

Linux/macOS:

```bash
cd Modul-04-VFD-RS485-Modbus/program
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
```

Windows PowerShell:

```powershell
cd Modul-04-VFD-RS485-Modbus/program
py -m venv .venv
.\.venv\Scripts\Activate.ps1
pip install -r requirements.txt
```

---

## 6. Cari Serial Port

```bash
python3 serial_port_list.py
```

Contoh nama:

- Linux: `/dev/ttyUSB0`, `/dev/ttyACM0`,
- Windows: `COM3`, `COM4`, dll.

Jangan hard-code port kelompok lain.

---

## 7. Persiapkan Config

```bash
cp vfd_config.example.json vfd_config.json
```

Isi field serial dan register dari manual.

Jalankan:

```bash
python3 modbus_vfd_test.py --config vfd_config.json --validate-only
```

Jika field wajib masih `null`, program harus menolak operasi.

---

## 8. Wiring RS485

Power/control dalam kondisi sesuai prosedur laboratorium.

```text
USB-RS485        VFD
+ / A ---------- RS485+
- / B ---------- RS485-
```

Gunakan label terminal aktual dari manual.

Catat:

- panjang kabel,
- jenis kabel,
- terminasi digunakan/tidak,
- common/shield jika ada.

---

## 9. Test A — Read-Only Connectivity

Motor belum diberi command RUN dari script.

```bash
python3 modbus_vfd_test.py --config vfd_config.json --read-only
```

Catat:

| Test | Result |
|---|---|
| serial connected | |
| slave responds | |
| output frequency raw | |
| output frequency scaled | |
| display VFD | |
| status raw | |

Jika nilai tidak masuk akal, jangan write.

---

## 10. Test B — Addressing Verification

Bandingkan manual dan library address.

| Symbol | Manual notation | Software address | Evidence |
|---|---|---:|---|
| OUT_FREQ | | | |
| STATUS | | | |

Jika manual menggunakan 4xxxx notation, jelaskan konversinya. Jika manual menggunakan address langsung/hex, tulis cara interpretasinya.

---

## 11. Test C — CRC Demo

Ambil salah satu contoh request dari catatan/manual, atau gunakan frame latihan:

```bash
python3 modbus_crc_demo.py "01 03 00 00 00 01"
```

Catat:

- payload tanpa CRC,
- CRC value,
- low byte,
- high byte,
- frame lengkap.

---

## 12. Test D — STOP Write

Sebelum RUN, verifikasi command STOP sesuai manual.

Program `--run` otomatis mencoba STOP dahulu. Untuk commissioning, monitor display/status untuk memastikan drive benar-benar berada pada STOP state.

---

## 13. Test E — Low Frequency RUN

Hanya setelah A–D PASS dan area motor aman:

```bash
python3 modbus_vfd_test.py --config vfd_config.json --run --hz 5
```

atau frekuensi rendah lain yang ditentukan pengajar.

Program melakukan:

```text
STOP
→ set frequency
→ RUN forward
→ monitor
→ STOP
```

Catat:

| Time | F command | F output | status | fault |
|---:|---:|---:|---|---|
| | | | | |

---

## 14. Test F — Frequency Steps

Setelah 5 Hz berhasil, uji tiga setpoint yang diizinkan, contoh:

```text
5 Hz, 10 Hz, 20 Hz
```

Jalankan terpisah dan selalu akhiri STOP.

| Set Hz | register count expected | F output | error |
|---:|---:|---:|---:|
| | | | |
| | | | |
| | | | |

---

## 15. Test G — Timeout

Pada kondisi **STOP**, cabut koneksi komunikasi atau gunakan port/slave ID yang sengaja salah sesuai instruksi asisten.

Amati:

- waktu timeout,
- error program,
- apakah program keluar tanpa hang.

Jangan melakukan running communication-loss test sebelum fail behavior VFD diverifikasi dan pengajar menyetujui.

---

## 16. Test H — Salah Setting Terencana

Satu per satu, pada kondisi STOP:

1. baud salah,
2. parity salah,
3. slave ID salah.

Catat symptom dan kembalikan setting benar.

Tujuan: mahasiswa mengenali ciri communication mismatch.

---

## 17. Monitoring Log

Simpan output terminal sebagai bukti. Jika perlu redirect:

```bash
python3 modbus_vfd_test.py --config vfd_config.json --read-only > read_test.txt
```

Untuk RUN:

```bash
python3 modbus_vfd_test.py --config vfd_config.json --run --hz 5 | tee run_5hz.txt
```

---

## 18. Troubleshooting Record

| Symptom | Layer | Test | Root cause | Fix |
|---|---|---|---|---|
| no response | physical/serial | | | |
| illegal address | protocol/register | | | |
| scale wrong | application | | | |
| | | | | |

Minimal tiga kasus/layer.

---

## 19. Analisis

Jawab:

1. Apa beda physical-layer failure dan wrong-register failure?
2. Mengapa register monitor dipilih untuk test awal?
3. Apa akibat off-by-one address?
4. Bagaimana 12.5 Hz dikonversi jika scale 0.01 Hz/count?
5. Mengapa `finally` mencoba STOP?
6. Apa yang masih dapat gagal walaupun program sudah mengirim STOP?
7. Mengapa communication-loss behavior harus dikonfigurasi pada VFD?
8. Apa keuntungan P4 dibanding P2 jika jumlah monitor/command banyak?

---

## 20. Deliverable

- serial parameter sheet,
- register map CSV,
- halaman manual sumber register,
- `vfd_config.json` dengan data nonrahasia unit lab,
- screenshot read-only,
- log RUN low-speed,
- CRC calculation,
- timeout test,
- troubleshooting record,
- jawaban analisis.

---

## 21. Kriteria Lulus

- tidak menebak register,
- read-only berhasil sebelum write,
- scaling diverifikasi terhadap display,
- STOP dilakukan sebelum dan sesudah RUN,
- timeout tidak membuat program hang,
- mahasiswa dapat menjelaskan layer RS485/serial/Modbus/register.