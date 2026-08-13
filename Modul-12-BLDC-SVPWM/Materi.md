# Modul 12 — Space Vector PWM (SVPWM)

## Tujuan
Mengubah reference voltage pada bidang αβ menjadi duty tiga phase dengan common-mode injection sehingga pemanfaatan DC bus lebih baik dan waveform cocok untuk FOC.

## Alur
1. Tentukan `Vα`, `Vβ`.
2. Inverse Clarke menjadi `Va,Vb,Vc`.
3. Cari `Vmax` dan `Vmin`.
4. Common-mode `Voffset = -(Vmax+Vmin)/2`.
5. Duty: `0.5 + (Vphase+Voffset)/Vbus`.
6. Clamp duty agar tersedia margin switching/bootstrap.

Implementasi library menggunakan bentuk min-max injection yang ekuivalen dengan SVPWM linear tanpa harus menulis sector timing secara eksplisit.

## Eksperimen
Bandingkan duty sine PWM P11 dan SVPWM P12 pada magnitude reference yang sama. Amati common-mode dan batas modulation.
