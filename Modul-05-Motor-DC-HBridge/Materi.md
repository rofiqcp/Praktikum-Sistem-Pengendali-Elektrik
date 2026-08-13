# Modul 05 — Motor DC dengan H-Bridge PCB Rev.E

## Tujuan
Mengendalikan arah dan tegangan rata-rata motor DC menggunakan **dua half-bridge internal PCB Rev.E**, bukan BTS7960.

## Konfigurasi hardware
- Motor DC dipasang antara `PH_A` dan `PH_B`.
- `PH_C` tidak dipakai.
- PWM A: PA8/PB13; PWM B: PA9/PB14.
- PA12 = shutdown gate driver.
- Bring-up 12 V current-limited.

## Locked-antiphase PWM
Program menggunakan:
- `dA = 0.5 + 0.45u`
- `dB = 0.5 - 0.45u`
- `u ∈ [-1,1]`

`u>0` menghasilkan beda tegangan rata-rata satu arah, `u<0` arah sebaliknya, `u=0` menghasilkan beda tegangan rata-rata mendekati nol. Kedua half-bridge tetap switching sehingga bootstrap high-side terus direfresh.

## Frekuensi PWM
Eksperimen dilakukan misalnya 4, 8, 12, 16, 20 kHz. Amati suara motor, ripple arus, respons, suhu MOSFET, dan bentuk gate. Jangan mengubah frekuensi ketika gate aktif; firmware melakukan shutdown sebelum reconfigure TIM1.

## Dead-time
Default awal 1000 ns. Nilai ini sengaja konservatif untuk board prototipe. P7 membahas variasinya lebih detail.
