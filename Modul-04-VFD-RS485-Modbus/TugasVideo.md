# Tugas Video 04 — VFD WECON via RS485 Modbus RTU

## Durasi

**12–18 menit**.

## Struktur Wajib

### 1. Hardware
Tunjukkan:

- model VFD,
- USB-RS485,
- terminal RS485,
- motor.

### 2. Layer komunikasi
Jelaskan diagram:

```text
Register/application → Modbus RTU → serial setting → RS485
```

### 3. Wiring
Jelaskan A/B atau S+/S- sesuai manual aktual, termasuk kabel/terminasi yang digunakan.

### 4. Serial setting
Tunjukkan:

- slave ID,
- baud,
- parity,
- stop bit.

### 5. Register map
Tampilkan manual asli dan tabel:

- output frequency,
- status,
- command,
- frequency reference,
- STOP/FWD values,
- scaling.

Jelaskan 0-based/address notation jika relevan.

### 6. CRC demo

```bash
python3 modbus_crc_demo.py "01 03 00 00 00 01"
```

Jelaskan field frame.

### 7. Read-only

```bash
python3 modbus_vfd_test.py --config vfd_config.json --read-only
```

Bandingkan output frequency dengan display VFD.

### 8. Safe RUN

Setelah read-only lulus:

```bash
python3 modbus_vfd_test.py --config vfd_config.json --run --hz 5
```

Jelaskan urutan STOP → frequency → RUN → monitor → STOP.

### 9. Timeout test
Pada kondisi STOP, tunjukkan satu test no-response/incorrect slave dan jelaskan hasilnya.

### 10. Troubleshooting
Jelaskan satu contoh masalah dari masing-masing layer:

- physical,
- serial,
- Modbus/register.

### 11. Kesimpulan
Bandingkan P2 digital, P3 analog 4–20 mA, dan P4 RS485.

---

## Rubrik

| Aspek | Bobot |
|---|---:|
| Layer RS485/Modbus | 15% |
| Wiring & serial setting | 15% |
| Register map berbasis manual | 20% |
| CRC/read-only demonstration | 15% |
| Safe RUN + monitoring | 15% |
| Timeout/troubleshooting | 10% |
| Penjelasan & keselamatan | 10% |

### Tidak diperbolehkan

- menebak register command,
- menguji random write ke alamat yang tidak diketahui,
- melakukan communication-loss test saat RUN tanpa fail behavior yang sudah diverifikasi dan izin pengajar.