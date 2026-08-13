# Modul 04 — VFD WECON melalui RS485 Modbus RTU

## Praktikum Sistem Pengendali Elektrik

Pertemuan ini mengganti frequency reference analog pada P3 dengan komunikasi digital serial. Mahasiswa mempelajari sistem secara berlapis: **RS485 physical layer → serial configuration → Modbus RTU frame → register map → engineering-unit scaling → safe command sequence**.

> Alamat register, command word, scaling, function code, dan terminal RS485 berbeda antar seri VFD. Jangan menggunakan alamat contoh internet tanpa memverifikasi manual **model WECON yang benar-benar digunakan di laboratorium**.

---

## 1. Capaian Pembelajaran

Mahasiswa mampu:

1. Menjelaskan perbedaan RS485 dan Modbus RTU.
2. Mengidentifikasi terminal A/B atau S+/S- pada VFD aktual.
3. Menjelaskan differential signaling dan alasan penggunaan twisted pair.
4. Menentukan slave/server ID, baud rate, parity, dan stop bit.
5. Menjelaskan struktur frame Modbus RTU: address, function, data, CRC.
6. Membedakan register monitor dan register command.
7. Memahami function code 03/04/06/16 secara konseptual.
8. Menghindari kesalahan 0-based vs 4xxxx register notation.
9. Membuat register map dari manual VFD aktual.
10. Melakukan read-only test sebelum write.
11. Mengirim STOP sebelum frequency/RUN command.
12. Melakukan monitoring output frequency/status.
13. Menjamin STOP dicoba pada `finally`/error path.
14. Membedakan timeout komunikasi dan Modbus exception.
15. Menguji CRC menggunakan program Python.
16. Mendiagnosis salah wiring, serial setting, slave ID, register, dan scaling.

---

## 2. RS485 bukan Modbus

RS485 adalah **physical/electrical layer**. Modbus RTU adalah **protocol** yang dapat berjalan di atas RS485.

```text
Application: Register command / monitor
        ↓
Protocol   : Modbus RTU
        ↓
Serial     : baud, parity, stop bit
        ↓
Physical   : RS485 differential pair
```

Dua perangkat dapat sama-sama memakai RS485 tetapi tidak otomatis dapat berkomunikasi jika protokolnya berbeda.

---

## 3. Physical Layer RS485

RS485 mengirim informasi berdasarkan perbedaan tegangan dua konduktor. Nama terminal dapat ditulis:

- A/B,
- D+/D-,
- S+/S-,
- atau penamaan lain menurut vendor.

Jangan menyimpulkan polaritas hanya dari huruf. Ikuti manual terminal VFD dan converter yang digunakan.

### 3.1 Wiring dasar

```text
USB-RS485 / PLC master        VFD
A / + ---------------------- A / S+
B / - ---------------------- B / S-
```

Jika referensi/common komunikasi diwajibkan manual, hubungkan sesuai petunjuk vendor.

### 3.2 Kabel

Disarankan:

- twisted pair,
- routing menjauh dari kabel motor/U-V-W,
- shield/grounding mengikuti praktik dan manual instalasi,
- koneksi rapat dan polaritas konsisten.

### 3.3 Topologi

Untuk bus multidrop, praktik umum adalah daisy-chain/line topology, bukan star panjang. Terminasi dipasang pada ujung bus bila diperlukan oleh panjang/baud/topologi.

Pada praktikum satu PC–satu VFD, jangan menambah terminasi secara acak jika manual/test bench tidak membutuhkannya.

---

## 4. Serial Configuration

Master dan VFD harus sama pada:

- baud rate,
- data bits sesuai protokol/library,
- parity,
- stop bit,
- slave/server address.

Contoh konfigurasi **bukan nilai wajib**:

```text
9600 baud
8 data bits
Even parity
1 stop bit
Slave ID 1
```

Gunakan nilai dari parameter komunikasi VFD aktual.

---

## 5. Modbus RTU Master/Client dan Slave/Server

Dalam praktikum:

```text
Laptop + USB-RS485 = Modbus client/master
VFD                = Modbus server/slave
```

Master/client memulai request. VFD merespons request yang ditujukan ke slave ID miliknya.

Pada sistem multidrop, setiap slave harus memiliki ID unik.

---

## 6. Struktur Frame RTU

Secara konseptual:

```text
[Slave Address] [Function Code] [Data ...] [CRC Low] [CRC High]
```

Contoh request read satu register memiliki field:

```text
slave
function
starting address
quantity
CRC16
```

Library Modbus biasanya menghitung CRC otomatis. P4 tetap mempelajari CRC agar mahasiswa memahami bagaimana frame divalidasi.

---

## 7. CRC16 Modbus

Modbus RTU menggunakan CRC16 untuk mendeteksi kerusakan frame.

Algoritma menggunakan polynomial yang umum direpresentasikan sebagai `0xA001` pada implementasi bitwise LSB-first.

Program:

```bash
python3 modbus_crc_demo.py "01 03 00 00 00 01"
```

menampilkan CRC dan frame lengkap.

Tujuan program bukan menggantikan library Modbus, melainkan memahami struktur komunikasi.

---

## 8. Function Code yang Sering Ditemui

| Function | Konsep |
|---:|---|
| 03 | Read Holding Registers |
| 04 | Read Input Registers |
| 06 | Write Single Register |
| 16 | Write Multiple Registers |

VFD tertentu dapat menggunakan subset atau pola register tertentu. Gunakan function code persis seperti manual.

---

## 9. Register Map

Sebelum coding, buat tabel:

| Symbol | Manual notation | Library address | Access | Function | Scale |
|---|---|---:|---|---|---|
| REG_COMMAND | | | R/W | run/stop/direction | word/bit |
| REG_FREQ_REF | | | R/W | frequency command | Hz/count |
| REG_OUT_FREQ | | | R | actual/output frequency | Hz/count |
| REG_STATUS | | | R | status | bit/word |
| REG_FAULT | | | R | fault code | code |

File template tersedia di `program/register_map_template.csv`.

---

## 10. Masalah 4xxxx dan Zero-Based Addressing

Dokumentasi Modbus kadang menulis holding register sebagai `40001`, `40002`, dan seterusnya. Library software sering meminta **offset register**, bukan nomor referensi 4xxxx secara literal.

Contoh konseptual:

```text
Manual notation 40001
library offset   0
```

Tetapi vendor VFD juga dapat menulis alamat langsung dalam hexadecimal/decimal tanpa notasi 4xxxx.

Karena itu:

1. baca bagaimana manual mendefinisikan address,
2. baca bagaimana library mendefinisikan parameter `address`,
3. lakukan read-only verification pada register monitor yang nilainya mudah dibandingkan dengan display.

Kesalahan satu register (`off-by-one`) adalah fault diagnosis yang sangat umum.

---

## 11. Scaling Register

Register adalah bilangan integer 16-bit, sedangkan engineering unit dapat berupa Hz, A, V, rpm, dan lainnya.

Contoh jika manual menyatakan:

```text
1 count = 0.01 Hz
```

maka:

```text
Hz = register × 0.01
register = round(Hz / 0.01)
```

Jika target 12.50 Hz:

```text
register = 1250
```

**Jangan memakai 0.01 Hz/count jika manual model aktual menyatakan scaling lain.**

---

## 12. Endianness dan Multi-Register Value

Satu register Modbus adalah 16 bit. Beberapa data dapat memakai dua register atau lebih.

Hal yang perlu diperiksa bila data 32-bit:

- word order,
- byte order,
- signed/unsigned,
- float/integer.

P4 memulai dari register 16-bit sederhana. Jangan menggabungkan dua word dengan asumsi tertentu sebelum manual menyatakan formatnya.

---

## 13. Command Word

Banyak VFD menggunakan sebuah register command untuk RUN/STOP/direction, tetapi encoding berbeda antar drive.

Contoh konsep:

```text
COMMAND_STOP
COMMAND_RUN_FORWARD
COMMAND_RUN_REVERSE
```

Nilai numerik **harus diambil dari manual**.

Program P4 menolak write jika field penting dalam konfigurasi masih `null`.

---

## 14. Safe Sequence

Urutan yang digunakan pada P4:

```text
1. Connect serial
2. Read-only monitor test
3. Verify slave ID and register scaling
4. Write STOP
5. Write low frequency reference
6. Write RUN forward
7. Monitor output frequency/status
8. Write STOP
9. Close serial port
```

Jika terjadi exception:

```text
finally:
    attempt STOP
    close serial
```

Software STOP bukan pengganti emergency stop atau hardware safety.

---

## 15. Read-Only First

Mode awal:

```bash
python3 modbus_vfd_test.py --config vfd_config.json --read-only
```

Program hanya membaca monitor register.

Sebelum write diizinkan, mahasiswa harus membuktikan:

- slave ID benar,
- serial setting benar,
- register monitor benar,
- scaling cocok dengan display VFD.

---

## 16. Timeout vs Modbus Exception

### Timeout/no response

Kemungkinan:

- port salah,
- A/B terbalik,
- baud/parity salah,
- slave ID salah,
- VFD mati,
- converter bermasalah.

### Modbus exception response

Perangkat merespons tetapi menolak request, misalnya karena:

- function tidak didukung,
- address invalid,
- value invalid,
- kondisi drive tidak mengizinkan operasi.

Keduanya harus dibedakan dalam troubleshooting.

---

## 17. Communication Loss

Jika komunikasi hilang saat sistem running, tindakan VFD bergantung konfigurasi communication-loss/fault behavior pada drive aktual.

Dalam P4:

- behavior tersebut **harus dibaca dari manual**, bukan ditebak,
- test disconnect dasar dilakukan pada kondisi STOP untuk menguji timeout software,
- test communication-loss saat motor running hanya dilakukan bila pengajar menyetujui dan fail behavior telah dikonfigurasi aman.

---

## 18. Configuration File

Salin:

```bash
cp vfd_config.example.json vfd_config.json
```

Isi:

- serial port,
- baud,
- parity,
- stop bits,
- slave ID,
- register monitor,
- register command,
- register frequency reference,
- frequency scale,
- command STOP/FWD/REV.

`vfd_config.json` bersifat local bench configuration dan sebaiknya tidak dianggap portable ke VFD model lain.

---

## 19. Program yang Harus Dijalankan

### A. List serial port

```bash
python3 serial_port_list.py
```

### B. CRC demo

```bash
python3 modbus_crc_demo.py "01 03 00 00 00 01"
```

### C. Validate configuration

```bash
python3 modbus_vfd_test.py --config vfd_config.json --validate-only
```

### D. Read-only

```bash
python3 modbus_vfd_test.py --config vfd_config.json --read-only
```

### E. Low-speed run

Hanya setelah read-only lulus:

```bash
python3 modbus_vfd_test.py --config vfd_config.json --run --hz 5
```

Program mengirim STOP sebelum RUN dan mencoba STOP kembali pada akhir program.

---

## 20. Troubleshooting Matrix

| Symptom | Kemungkinan | Test awal |
|---|---|---|
| port tidak ditemukan | device/driver/permission | serial_port_list.py |
| timeout | wiring/serial/slave | cek A/B + settings |
| exception illegal address | register salah | manual + addressing convention |
| value 10×/100× salah | scaling | cek Hz/count |
| write tidak berpengaruh | command source | cek source communication |
| RUN berhasil tetapi monitor salah | monitor register/scale | bandingkan display |
| data kadang error | wiring/noise/topology | kabel, routing, terminasi |

---

## 21. Pertanyaan Evaluasi

1. Apa beda RS485 dan Modbus RTU?
2. Mengapa baud/parity/slave ID harus sama?
3. Apa fungsi CRC?
4. Apa beda FC03 dan FC06 secara konsep?
5. Mengapa register 40001 dapat menjadi address 0 pada library tertentu?
6. Mengapa read-only test dilakukan sebelum write?
7. Apa fungsi `frequency_scale_hz_per_count`?
8. Apa beda timeout dan Modbus exception?
9. Mengapa STOP dikirim dalam `finally`?
10. Mengapa software STOP tidak menggantikan emergency stop?
11. Apa risiko menyalin register map dari seri VFD lain?
12. Bagaimana memverifikasi bahwa register output frequency benar?

---

## 22. Hubungan P2–P4

| Pertemuan | Media command | Kelebihan latihan |
|---|---|---|
| P2 | digital terminal | logic/interlock mudah diamati |
| P3 | 4–20 mA | continuous analog reference |
| P4 | RS485 Modbus | command + monitoring melalui satu bus |

P4 menutup blok industrial-drive sebelum P5 berpindah ke STM32 dan PCB motor-control buatan sendiri.