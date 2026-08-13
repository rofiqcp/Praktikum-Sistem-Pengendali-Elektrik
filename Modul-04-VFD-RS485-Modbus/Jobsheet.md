# Jobsheet 04 — Modbus RTU VFD WECON

## Persiapan
Isi `vfd_config.json` hanya setelah manual unit lab diperiksa. Pastikan VFD diset menerima run/frequency dari communication.

## Pengujian
1. Cek A/B dan termination.
2. Samakan slave ID, baud, parity, stop bit.
3. Jalankan mode `--read-only` dan baca register monitor.
4. Cocokkan nilai display VFD dengan data register.
5. Dengan motor unloaded dan frequency rendah, tulis frequency reference.
6. Tulis RUN forward.
7. Baca output frequency/status minimal 5 detik.
8. Kirim STOP.
9. Putuskan kabel RS485 saat STOP dan dokumentasikan timeout handling.

## Kriteria lulus
Tidak ada write sebelum register tervalidasi, timeout tidak menyebabkan program menggantung, dan program selalu mencoba STOP ketika keluar.
