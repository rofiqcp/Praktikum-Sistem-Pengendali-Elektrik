# Modul 07 — Complementary PWM, Dead-Time, Frekuensi dan Phase

## Konsep
TIM1 menghasilkan CH1/CH1N, CH2/CH2N, CH3/CH3N. IR2110 menerima HIN dan LIN secara terpisah. Dead-time mencegah high-side dan low-side satu leg ON bersamaan selama transisi.

## Variabel eksperimen
- carrier PWM: 4–20 kHz,
- dead-time: mulai sekitar 0.8–2.0 µs pada board prototipe; jangan gunakan 0 saat VBUS terhubung,
- modulation amplitude,
- electrical phase spacing: default 120° antar phase.

Frekuensi carrier berbeda dari frekuensi elektrik. Carrier mengatur switching MOSFET; electrical frequency mengatur seberapa cepat vektor/sinus tiga-phase berputar.

## Trade-off
Dead-time terlalu kecil meningkatkan risiko shoot-through. Terlalu besar meningkatkan distortion dan error tegangan. Carrier lebih tinggi mengurangi ripple audible tetapi meningkatkan switching loss.
