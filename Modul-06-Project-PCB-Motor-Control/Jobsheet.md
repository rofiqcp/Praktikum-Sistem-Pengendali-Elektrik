# Jobsheet 06 — Assembly & Bring-up PCB Rev.E

## Tahap A — sebelum solder
- cek orientasi Black Pill, IR2110 PDIP-14, LM358 PDIP-8, diode dan elco;
- cocokkan pin MOSFET aktual G-D-S;
- continuity net penting terhadap schematic.

## Tahap B — tanpa Black Pill / tanpa MOSFET bila memungkinkan
Uji short VBUS-GND, 12V_GATE-GND, 5V-GND, 3V3-GND.

## Tahap C — power logic
Dengan supply current-limited, ukur 12V_GATE, 5 V, 3.3 V, VREF sekitar 1.65 V. Jika salah, STOP.

## Tahap D — firmware shutdown
Upload diagnostic. Saat reset, `DRV_SD` harus HIGH. Verifikasi pin 11 ketiga IR2110 berada pada shutdown.

## Tahap E — gate waveform tanpa motor
VBUS 12–18 V. Enable PWM hanya dengan izin praktikan/dosen. Ukur HO/LO phase A/B/C, frekuensi, duty dan dead-time.

## Tahap F — analog
Dengan driver shutdown, kalibrasi PA0/PA1/PA4. Verifikasi PA5 terhadap VBUS multimeter. Uji throttle/brake 0–5 V melalui input yang memang dirancang.

## Tahap G — Hall/Encoder
Gunakan sensor 3.3 V atau open-collector. Putar motor manual dan lihat state berubah.

## Tahap H — low-power motor test
Motor kecil, VBUS 12 V current-limit. Baru setelah lulus semua tahap board diberi status `READY FOR P7-P15`.
