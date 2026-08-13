# Tugas Video 01 — Peta Sistem Pengendali Elektrik

## Tujuan

Video membuktikan bahwa mahasiswa memahami **hubungan antarblok**, bukan sekadar menghafal definisi.

## Durasi

**10–15 menit**.

---

## Struktur Video Wajib

### 1. Pembukaan — ±30 detik

Sebutkan:

- nama,
- NIM,
- kelas,
- judul praktikum,
- tujuan video.

### 2. Konsep sistem kontrol — ±2 menit

Jelaskan dengan diagram buatan sendiri:

```text
Setpoint → Controller → Power Converter → Motor → Sensor → Feedback
```

Jelaskan:

- setpoint,
- controller,
- actuator/plant,
- sensor,
- feedback,
- protection.

### 3. Power path vs control path — ±1 menit

Gunakan contoh PLC + VFD + motor AC.

Tunjukkan mana:

- kabel daya,
- command PLC,
- feedback/status.

### 4. Lima sistem penggerak — ±4 menit

Wajib menjelaskan singkat:

1. PLC + VFD,
2. motor DC H-bridge,
3. inverter 1-fasa,
4. BLDC six-step,
5. BLDC/PMSM FOC.

Untuk setiap sistem sebutkan:

- motor/plant,
- power converter,
- command,
- feedback,
- risiko utama.

### 5. PCB ESC Rev.E — ±3 menit

Tunjukkan board secara fisik **tanpa menyalakan VBUS**.

Wajib tunjukkan:

- STM32F401,
- IR2110,
- MOSFET,
- PH_A/B/C,
- VBUS,
- current sensing,
- Hall/Encoder,
- PA12/DRV_SD.

Jelaskan:

```text
DRV_SD HIGH = shutdown
DRV_SD LOW  = enable
```

### 6. Demonstrasi program — ±2 menit

Jalankan minimal:

```bash
python3 control_selector.py --system foc
python3 power_flow_demo.py --system vfd
python3 dc_motor_pwm_demo.py --vbus 12 --duty -50
```

Jelaskan output, jangan hanya menampilkan terminal.

### 7. Keselamatan — ±1 menit

Sebutkan minimal lima aturan bring-up, termasuk:

- mulai 12–18 V current-limited,
- gate test tanpa motor,
- cek dead-time,
- current offset calibration,
- shutdown saat fault.

### 8. Kesimpulan — ±30 detik

Hubungkan P1 dengan P2–P16.

---

## Ketentuan

- Diagram harus dibuat sendiri.
- Suara harus jelas.
- Board harus terlihat nyata jika tersedia di laboratorium.
- Tidak perlu memberi VBUS pada board.
- Tidak boleh menyalakan perangkat secara berbahaya demi video.
- Jika model VFD/PLC disebut, tampilkan nameplate agar tidak ambigu.

---

## Rubrik Penilaian

| Komponen | Bobot |
|---|---:|
| Ketepatan konsep | 30% |
| Hubungan power/control/feedback | 15% |
| Penjelasan VFD–DC–inverter–BLDC–FOC | 20% |
| Pengenalan PCB Rev.E | 15% |
| Demonstrasi program | 10% |
| Keselamatan dan komunikasi teknis | 10% |

### Pengurangan nilai

- hanya membaca slide tanpa demonstrasi: -15,
- diagram tidak menjelaskan arah aliran: -10,
- salah membedakan PWM frequency dan electrical frequency: -10,
- demonstrasi hardware tidak aman: dapat dinyatakan tidak lulus.