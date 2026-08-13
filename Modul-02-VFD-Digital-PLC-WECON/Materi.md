# Modul 02 — VFD WECON dengan Kontrol Digital dari PLC WECON

## Capaian
Mahasiswa mampu membuat kontrol RUN/STOP, FWD/REV dan preset frequency melalui terminal digital VFD.

## Konsep
PLC tidak memberi daya motor. PLC hanya memberi command logic ke input VFD. VFD harus dikonfigurasi agar sumber RUN dan sumber frequency sesuai terminal yang digunakan.

Karena nomor parameter berbeda antar seri WECON, praktikum menggunakan **nama fungsi**, lalu mahasiswa mengisi nomor parameter dari manual unit aktual.

| Fungsi yang dicari di manual | Nilai yang dibutuhkan |
|---|---|
| Run command source | terminal/digital input |
| Frequency source | keypad atau multi-step terminal |
| DI1 | Forward run |
| DI2 | Reverse run / sesuai skenario |
| DI3/DI4 | multi-speed select |
| Acceleration time | nilai aman lab |
| Deceleration time | nilai aman lab |
| Max/min frequency | batas aman motor |

## Interlock penting
FWD dan REV tidak boleh aktif bersamaan. STOP/emergency harus memiliki prioritas tertinggi. Pergantian arah dilakukan setelah command speed turun dan motor berhenti atau mengikuti aturan VFD.

## Alur program PLC
1. tombol START melakukan latch `RUN_REQ`,
2. STOP mereset latch,
3. selector arah menghasilkan FWD xor REV,
4. selector speed menghasilkan kombinasi preset input,
5. fault VFD memblokir RUN.
