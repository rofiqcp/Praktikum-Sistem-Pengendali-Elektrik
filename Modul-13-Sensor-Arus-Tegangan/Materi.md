# Modul 13 — Sensor Arus, Tegangan, Throttle dan Brake

## Channel PCB
- PA0: IA,
- PA1: IB,
- PA4: IDC,
- PA5: VBUS,
- PA6: throttle,
- PA7: brake.

## Current sensing
Shunt 2 mΩ dan differential amplifier gain ≈20 memberi sensitivitas ideal:

`V/A = 0.002 × 20 = 0.040 V/A`

Arus dihitung dari `I=(Vadc-Vzero)/0.040`. Nilai `Vzero` **tidak diasumsikan 1.65 V**; firmware meng-average ADC saat gate shutdown dan arus nol.

Karena hanya IA dan IB yang diukur per-phase, untuk sistem tiga kawat tanpa netral:

`IC = -IA - IB`

## VBUS
Divider 100k+100k terhadap 10k memberi rasio ideal 21:1, sehingga `VBUS≈Vadc×21`.

## Throttle/brake
Divider 15k/22k menghasilkan `Vadc=Vin×22/37`, sehingga `Vin≈Vadc×37/22`.

## Kalibrasi
Bandingkan ADC dengan multimeter/current source atau beban terukur. Simpan slope dan offset hasil regresi bila deviasi komponen signifikan.
