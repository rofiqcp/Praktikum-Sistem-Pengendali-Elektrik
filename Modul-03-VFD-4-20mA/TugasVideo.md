# Tugas Video 03 — Scaling dan Karakterisasi 4–20 mA

## Durasi

**12–18 menit**.

## Isi Wajib

### 1. Identitas hardware
Tunjukkan model:

- PLC/AO module,
- VFD,
- motor.

### 2. Teori
Jelaskan:

- mengapa 4–20 mA,
- live zero,
- scaling equation,
- inverse scaling.

Hitung manual contoh 12 mA untuk range yang digunakan.

### 3. Wiring
Gambar current loop dan jelaskan mengapa multimeter dipasang seri.

### 4. Parameter VFD
Tampilkan manual dan setting:

- analog current mode,
- frequency source,
- lower/upper scaling,
- acceleration/deceleration.

### 5. Software
Jalankan:

```bash
python3 scaling_4_20mA.py --current 12 --fmin 0 --fmax 50
python3 generate_characterization.py --fmin 0 --fmax 50 --output hasil_pengamatan.csv
```

### 6. Pengujian 5 titik
Tampilkan 4, 8, 12, 16, 20 mA dan data aktual.

### 7. Analisis
Jalankan:

```bash
python3 analyze_characterization.py hasil_pengamatan.csv --plot
```

Jelaskan:

- slope,
- intercept,
- max error,
- RMSE.

### 8. Abnormal condition
Tunjukkan satu kondisi below-range/loop abnormal secara aman dan jelaskan behavior VFD.

### 9. Kesimpulan
Jelaskan perbedaan P2 digital preset dengan P3 analog continuous reference.

---

## Rubrik

| Aspek | Bobot |
|---|---:|
| Teori dan scaling | 20% |
| Wiring dan measurement | 15% |
| Parameter berbasis manual | 15% |
| Data 5+ titik | 20% |
| Analisis Python/grafik | 15% |
| Abnormal condition & safety | 10% |
| Komunikasi teknis | 5% |

Tidak diperbolehkan mengubah data pengukuran hanya agar grafik terlihat linear.