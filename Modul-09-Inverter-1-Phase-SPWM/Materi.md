# Modul 09 — Inverter 1-Fasa SPWM dengan PCB Rev.E

## Konfigurasi
Phase A dan B dipakai sebagai full bridge. Primer trafo 12 V dipasang **antara PH_A dan PH_B**, bukan ke GND. PH_C tidak dipakai. VBUS praktikum = 12 V current-limited.

## SPWM bipolar/full-bridge
Firmware menghasilkan:
- `dA = 0.5 + 0.5 M sin(θ)`
- `dB = 0.5 - 0.5 M sin(θ)`

Sehingga tegangan rata-rata diferensial A-B berbentuk sinus dengan fundamental mengikuti `f_out`.

## Carrier vs output frequency
Carrier PWM misalnya 12–20 kHz. Output fundamental untuk trafo 50 Hz umumnya dijaga di sekitar ratingnya. Eksperimen frekuensi jauh di bawah 50 Hz dilakukan **tanpa trafo**, atau amplitudo harus diturunkan mengikuti prinsip V/f agar inti tidak saturasi.

## Keselamatan trafo 12→220 V
Sekunder dapat menghasilkan tegangan berbahaya. Jangan menyentuh sekunder, jangan menghubungkan ke jaringan PLN, gunakan fuse, enclosure, emergency stop, dan alat ukur yang sesuai. Pengujian awal dilakukan dengan sekunder tidak dibebani.
