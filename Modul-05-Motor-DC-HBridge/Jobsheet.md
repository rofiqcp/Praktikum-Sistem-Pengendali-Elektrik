# Jobsheet 05 — PWM Motor DC memakai Phase A+B

## Sebelum motor dipasang
1. VBUS 12 V current-limited.
2. Upload program, PA12 harus shutdown.
3. Command duty diset 0.
4. Enable dan ukur gate A/B dengan osiloskop.
5. Pastikan complementary gate tidak overlap.

## Uji motor
1. Motor antara PH_A dan PH_B.
2. Mulai `u=0.10`, lalu 0.20, 0.40, 0.60.
3. Ulangi untuk arah negatif setelah motor berhenti.
4. Ulangi pada PWM 4, 8, 12, 16, 20 kHz.
5. Catat VBUS, IDC, duty command, arah, RPM bila tachometer tersedia, suara, suhu dan ripple.

## Serial command
- `e` enable gate.
- `x` emergency software shutdown.
- `d 0.3` command +30%.
- `d -0.3` command reverse.
- `f 12000` ubah carrier 12 kHz dengan shutdown otomatis.

## Analisis
Bandingkan pengaruh duty dan carrier frequency. Jelaskan mengapa command nol pada locked-antiphase tidak sama dengan semua MOSFET off.
