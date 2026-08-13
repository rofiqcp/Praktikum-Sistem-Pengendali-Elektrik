# Modul 15 — FOC Speed dan Position dengan Encoder AB

## Sensor
PB6/PB7 berfungsi sebagai TIM4 encoder A/B. PB2 dapat dipakai sebagai index bila diperlukan. Encoder push-pull harus 3.3 V; 5 V hanya aman bila open-collector/open-drain yang ditarik ke 3.3 V.

## Struktur cascaded loop
`Position P → Speed PI → Iq reference → Current PI(d,q) → inverse Park → SVPWM → inverter → motor`

Current loop paling cepat, speed loop lebih lambat, position loop paling lambat.

## Electrical angle
`θe = pole_pairs × θm + offset`

Program menyediakan alignment: apply stator vector kecil pada electrical angle 0, tunggu rotor mengunci, lalu zero encoder. Alignment hanya dilakukan pada low voltage/current-limited dan area mekanik aman.

## Parameter wajib diedit
- encoder PPR/CPR,
- pole pair motor,
- arah encoder,
- current limit,
- gain current/speed/position.

Tidak ada satu set gain yang benar untuk semua motor.
