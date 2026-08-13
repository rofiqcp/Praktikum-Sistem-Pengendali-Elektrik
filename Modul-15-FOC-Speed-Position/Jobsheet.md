# Jobsheet 15 — Encoder FOC Speed & Position

## Persiapan
1. Isi `ENCODER_PPR` dan `POLE_PAIRS` sesuai motor.
2. Motor unloaded, VBUS 12 V current-limit.
3. Verifikasi encoder count bertambah/berkurang saat diputar manual.
4. Kalibrasi current offset.

## Alignment
Kirim `a`. Rotor akan ditarik ke satu electrical angle dengan tegangan kecil. Pastikan mekanik bebas bergerak. Setelah stabil encoder di-zero.

## Speed mode
- `v` pilih speed mode,
- `s 50`, `s 100`, `s -50` RPM,
- amati rpm, iqRef, Id/Iq dan IDC.

## Position mode
- `p` pilih position mode,
- `g 0.25`, `g -0.25`, `g 1.0` dalam turn,
- ukur overshoot, settling, steady-state error.

## Tuning berjenjang
Tune current loop dahulu, speed loop kedua, position loop terakhir. Jangan men-tune tiga loop sekaligus.
