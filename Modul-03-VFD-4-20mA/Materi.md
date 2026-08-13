# Modul 03 — Reference VFD melalui 4–20 mA

## Capaian
Mahasiswa mampu melakukan scaling arus 4–20 mA menjadi frequency reference.

## Persamaan
Untuk rentang 4–20 mA yang dipetakan ke `Fmin..Fmax`:

`F = Fmin + (I_mA - 4) / 16 × (Fmax - Fmin)`

Clamp input ke 4–20 mA. Contoh bila 4 mA→0 Hz dan 20 mA→50 Hz: 12 mA→25 Hz.

## Mengapa 4–20 mA?
Live-zero 4 mA membantu membedakan nilai minimum dengan loop putus/kehilangan sinyal. Arus juga lebih tahan terhadap drop tegangan kabel dibanding reference tegangan pada jarak panjang.

## PLC/VFD
Periksa jenis output analog PLC: active current output atau membutuhkan converter. Periksa terminal analog current input VFD dan common analog. Konfigurasikan source frequency ke analog current dan tentukan lower/upper scaling.

## Pengujian
Gunakan nilai 4, 8, 12, 16, 20 mA. Bandingkan frequency teoritis dan output VFD aktual.
