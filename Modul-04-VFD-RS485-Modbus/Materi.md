# Modul 04 — VFD WECON via RS485 Modbus RTU

## Capaian
Mahasiswa mampu mengatur komunikasi serial, membaca status, menulis command dan frequency reference dengan register yang telah diverifikasi dari manual model VFD aktual.

## Layer komunikasi
- Physical: RS485 A/B, twisted pair, common/reference sesuai manual, terminasi di ujung bus.
- Serial: baudrate, parity, stop bit.
- Modbus: slave ID, function code, register address, data scaling.

## Aturan penting
Jangan menebak register. Sebelum program dijalankan, isi tabel berikut dari manual:

| Symbol | Address | Function | Scale |
|---|---:|---|---|
| REG_COMMAND | ... | run/stop/direction | bit/word |
| REG_FREQ_REF | ... | frequency command | contoh 0.01 Hz/count |
| REG_OUT_FREQ | ... | monitored frequency | sesuai manual |
| REG_STATUS | ... | status/fault | word |

## Sequence aman
1. connect,
2. read-only test,
3. verify slave ID,
4. write STOP,
5. write low frequency,
6. write RUN,
7. monitor,
8. write STOP pada `finally`/error.
