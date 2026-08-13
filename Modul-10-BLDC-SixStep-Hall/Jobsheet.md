# Jobsheet 10 — Hall Decode & Six-Step

## A. Hall tanpa power stage
1. Gate shutdown.
2. Putar rotor manual satu putaran.
3. Catat urutan state Hall.
4. Pastikan tidak menetap di 000/111.
5. Jika sequence berbeda dari default, ubah map firmware atau wiring sensor dengan dokumentasi.

## B. Gate tanpa motor
Uji masing-masing sector dengan osiloskop. Pastikan hanya satu phase source, satu sink, satu floating.

## C. Motor low voltage
1. VBUS 12 V current-limit.
2. Duty mulai 0.10–0.15.
3. Enable forward.
4. Jika motor hanya bergetar/arus tinggi, STOP dan koreksi Hall-to-phase mapping; jangan menaikkan duty.
5. Setelah forward halus, uji reverse dari kondisi berhenti.
6. Catat Hall state, sector, VBUS, IDC dan arah.
