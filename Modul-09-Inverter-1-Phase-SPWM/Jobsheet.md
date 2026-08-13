# Jobsheet 09 — Full-Bridge SPWM 12 V ke Trafo 12/220 V

## Tahap 1 — tanpa trafo
1. VBUS 12 V current limit.
2. Output diukur antara PH_A dan PH_B.
3. Uji carrier 8, 12, 16, 20 kHz.
4. Uji fundamental 5, 10, 25, 50, 75 Hz hanya pada waveform/no-load tanpa trafo.
5. Uji modulation index 0.2, 0.4, 0.6, 0.8.

## Tahap 2 — dengan trafo
1. Pastikan trafo primer benar 12 V AC dan sekunder terisolasi.
2. Gunakan sekitar 50 Hz untuk rating 50 Hz.
3. Mulai modulation rendah.
4. Ukur VBUS, IDC, tegangan primer dan sekunder tanpa menyentuh sisi sekunder.
5. Hentikan jika arus input naik abnormal, trafo berdengung keras, atau MOSFET/trafo cepat panas.

## Serial
`e` enable, `x` shutdown, `f 50` output Hz, `m 0.5` modulation, `c 16000` carrier Hz, `t 1200` dead-time ns.
