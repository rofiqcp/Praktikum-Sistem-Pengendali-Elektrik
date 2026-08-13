# Tugas Video 02 — Kontrol Digital VFD WECON via PLC WECON

## Durasi

**12–18 menit**.

## Tujuan

Video harus menunjukkan proses engineering lengkap: identifikasi → mapping → simulation → PLC program → terminal test → motor test → fault/interlock.

---

## Struktur Wajib

### 1. Identitas perangkat
Tunjukkan nameplate:

- PLC WECON,
- VFD WECON,
- motor.

Sebutkan apakah output PLC relay/transistor dan konfigurasi DI VFD.

### 2. Wiring control
Gambar dan jelaskan:

```text
Push button → PLC input
PLC output → VFD DI
VFD → motor U/V/W
```

Bedakan jalur power dan control.

### 3. Parameter VFD
Tampilkan halaman manual dan parameter aktual untuk:

- run command source,
- frequency source,
- FWD,
- REV,
- MS0/MS1,
- preset speeds,
- acceleration/deceleration.

### 4. Program PLC
Jelaskan minimal:

- START latch,
- STOP priority,
- permit,
- FWD/REV XOR interlock,
- multi-speed.

### 5. Simulator
Jalankan:

```bash
python3 digital_logic_sim.py
```

Tunjukkan test direction conflict dan fault.

### 6. PLC online monitor
Tampilkan perubahan input/output ketika tombol ditekan.

### 7. VFD terminal test
Sebelum motor RUN, tunjukkan bahwa DI VFD membaca output PLC dengan benar.

### 8. Motor test
Demo:

- forward low speed,
- STOP,
- reverse setelah motor berhenti,
- minimum 3 preset speed.

### 9. Fault/interlock
Tunjukkan satu simulasi permit/fault yang aman dan buktikan RUN terblokir.

### 10. Kesimpulan
Jelaskan mengapa P2 merupakan dasar sebelum P3 4–20 mA dan P4 RS485.

---

## Rubrik

| Aspek | Bobot |
|---|---:|
| Identifikasi hardware & wiring | 15% |
| Parameter VFD berbasis manual | 15% |
| Logic PLC | 25% |
| Simulation & online monitoring | 10% |
| Motor test | 15% |
| Interlock/fault | 10% |
| Penjelasan teknis & keselamatan | 10% |

## Tidak diperbolehkan

- reverse mendadak hanya untuk demo,
- menyalin nomor parameter tanpa menunjukkan model/manual,
- mengabaikan konflik FWD+REV,
- bypass STOP/fault agar demo terlihat berhasil.