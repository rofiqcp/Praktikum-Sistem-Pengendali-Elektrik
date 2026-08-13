# Jobsheet 13 — Kalibrasi Sensor PCB Rev.E

## A. Zero current
1. Motor tidak diberi command, DRV_SD shutdown.
2. Jalankan kalibrasi 1000 sample.
3. Catat IA/IB/IDC sesudah kalibrasi; seharusnya dekat 0 A.

## B. VBUS
Uji 8, 10, 12, 15, 18 V. Catat multimeter vs firmware dan hitung error.

## C. Current
Dengan beban low-voltage terkontrol, bandingkan IDC firmware dengan ammeter pada minimal lima titik aman. Untuk IA/IB gunakan kondisi motor yang dapat direproduksi atau current injection bench yang sesuai.

## D. Throttle/brake
Berikan 0–5 V hanya melalui konektor input yang dirancang. Catat input multimeter vs firmware.

## Output
Buat CSV dan grafik actual-vs-measured, slope, offset, RMSE, serta keputusan apakah konstanta nominal cukup atau perlu calibration factor.
