# Jobsheet 04 — Modbus RTU VFD WECON

## Persiapan
Isi `vfd_config.json` hanya setelah manual unit lab diperiksa. Pastikan VFD diset menerima run/frequency dari communication.

Siapkan Python environment:

```bash
cd Modul-04-VFD-RS485-Modbus/program
python3 -m venv .venv
source .venv/bin/activate   # Linux/macOS
# Windows PowerShell: .venv\Scripts\Activate.ps1
pip install -r requirements.txt
cp vfd_config.example.json vfd_config.json
```

Contoh dikunci ke **PyModbus 3.6.9**, karena source menggunakan parameter `slave=` pada operasi Modbus serial.

## Pengujian
1. Cek A/B dan termination.
2. Samakan slave ID, baud, parity, stop bit.
3. Isi alamat register dari manual VFD WECON model aktual; jangan menebak register.
4. Jalankan `python modbus_vfd_test.py --read-only` dan baca register monitor.
5. Cocokkan nilai display VFD dengan data register.
6. Dengan motor unloaded dan frequency rendah, jalankan misalnya `python modbus_vfd_test.py --hz 5`.
7. Program menulis STOP terlebih dahulu, mengisi reference, menjalankan forward, membaca output frequency, lalu mencoba STOP lagi saat keluar.
8. Putuskan kabel RS485 saat VFD sudah STOP dan dokumentasikan timeout handling.

## Kriteria lulus
Tidak ada write sebelum register tervalidasi, timeout tidak menyebabkan program menggantung, dan program selalu mencoba STOP ketika keluar.
