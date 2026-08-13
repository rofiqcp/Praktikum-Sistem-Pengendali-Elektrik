# Modul 11 — BLDC 3-Phase Sine PWM

## Tujuan
Menghasilkan tiga duty sinus berjarak 120° untuk membentuk rotating stator voltage vector secara open-loop.

`dA=0.5+0.5M sin(θ)`

`dB=0.5+0.5M sin(θ-120°)`

`dC=0.5+0.5M sin(θ+120°)`

`θ` bertambah sesuai electrical frequency. Mechanical speed ideal bergantung jumlah pole pair: `f_mech = f_elec / pole_pairs`.

## Perbedaan dengan six-step
Six-step hanya menggunakan enam kombinasi utama dan back-EMF cenderung trapezoidal. Sine PWM memberi excitation lebih halus tetapi open-loop belum menjamin vektor mengikuti rotor.

## Variasi
- electrical frequency,
- modulation index,
- PWM carrier,
- dead-time.

Gunakan low voltage dan unloaded motor karena open-loop dapat kehilangan sinkronisasi.
