# Keselamatan Praktikum Power Electronics

PCB ini dapat bekerja pada energi yang cukup untuk merusak MOSFET, PCB, alat ukur, dan mencederai operator.

## Bring-up wajib berurutan

1. Board belum diberi VBUS: continuity test VBUS-GND/PGND dan gate-source.
2. Catu logic terbatas: verifikasi 12V_GATE, 5 V, 3.3 V, VREF ≈1.65 V.
3. Firmware boot harus menjaga PA12/DRV_SD HIGH (shutdown).
4. Tanpa motor, VBUS 12–18 V current-limited: periksa HO/LO setiap phase dengan osiloskop/differential probe.
5. Ukur dead-time sebelum motor dipasang.
6. Kalibrasi offset IA, IB, IDC pada arus nol.
7. Baru pasang motor kecil/beban rendah.
8. Naikkan tegangan/arus bertahap setelah waveform bersih dan proteksi bekerja.

## Larangan

- Jangan langsung mulai dari 42 V.
- Jangan menghubungkan Hall/encoder push-pull 5 V ke PB6/PB7/PB2.
- Jangan mengubah dead-time ke 0 pada power stage yang terhubung VBUS.
- Jangan mengukur switch-node/high-side dengan ground clip osiloskop biasa tanpa metode pengukuran yang sesuai.
- Output sekunder trafo P9 dapat mencapai sekitar 220 VAC; anggap sebagai tegangan berbahaya walaupun input hanya 12 V.

## Fail-safe firmware

`DRV_SD` dipull-up oleh hardware. Firmware selalu melakukan `shutdown(true)` sebelum konfigurasi timer dan ADC. Driver baru di-enable setelah konfigurasi valid.
