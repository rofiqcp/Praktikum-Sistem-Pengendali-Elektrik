# Jobsheet 02 — RUN/STOP, FWD/REV, Multi-Speed VFD WECON

## Persiapan
Catat model PLC dan VFD, tegangan kontrol terminal, tipe input sinking/sourcing, common terminal, dan manual resmi unit lab.

## Langkah
1. Motor belum dihubungkan: verifikasi output PLC berubah sesuai tombol.
2. Set VFD agar command RUN berasal dari terminal.
3. Set DI sesuai fungsi FWD/REV/preset speed.
4. Uji RUN forward pada frekuensi rendah.
5. STOP dan tunggu motor berhenti.
6. Uji reverse.
7. Uji minimal tiga preset speed.
8. Aktifkan simulasi fault/interlock dan pastikan RUN terblokir.

## Tabel pengamatan
Catat command PLC, status DI VFD, set frequency, output frequency, arah, waktu akselerasi, waktu deselerasi, dan fault.

## Kriteria lulus
Tidak ada kondisi FWD+REV simultan, STOP selalu dominan, dan seluruh preset dapat diverifikasi dari display VFD.
