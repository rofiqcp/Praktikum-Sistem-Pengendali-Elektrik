# Changelog

## v1 — 2026-08-13

- Membuat kurikulum lengkap 16 pertemuan Sistem Pengendali Elektrik.
- Menjadikan PCB ESC Rev.E STM32F401 + IR2110 + LM358 sebagai hardware utama P5–P15.
- Mengganti rencana P5 dari BTS7960 menjadi H-bridge internal Phase A+B PCB Rev.E.
- Menambahkan Materi, Jobsheet dan TugasVideo/Project/Responsi sesuai jenis pertemuan.
- Menambahkan program WECON reference untuk digital I/O, 4–20 mA dan RS485.
- Menambahkan independent PlatformIO projects P5–P16.
- Menambahkan shared `ESCBoard` library: TIM1 complementary PWM, dead-time, H-bridge, six-step, Hall, TIM4 encoder, ADC calibration, Clarke/Park, SVPWM dan PI.
- Menambahkan inverter 1-fasa SPWM, six-step Hall, sine PWM, SVPWM, sensing, Hall-FOC dan encoder speed/position FOC.
- Menambahkan fail-safe `DRV_SD` boot shutdown dan panduan bring-up 12–18 V current-limited.
- Audit P10: komutasi hanya di-update ketika Hall/setting berubah agar commutation blanking tidak terjadi terus-menerus.
- Menyamakan semua dokumentasi pin dengan Rev.E final.
