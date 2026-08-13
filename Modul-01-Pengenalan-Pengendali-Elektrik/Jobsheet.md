# Jobsheet 01 — Identifikasi Sistem Pengendali Elektrik

## Tujuan
Mengidentifikasi jalur energi, jalur informasi, sensor, aktuator, proteksi, dan mode kontrol.

## Alat
PLC WECON, VFD WECON, motor AC, motor DC, BLDC, PCB ESC Rev.E, multimeter. Board belum diberi VBUS pada sesi identifikasi.

## Langkah
1. Gambar diagram blok PLC→VFD→motor AC.
2. Tandai mana **power path** dan mana **control path**.
3. Pada motor DC, gambar H-bridge dan jelaskan arti duty positif, nol, dan negatif.
4. Pada PCB Rev.E, telusuri konektor VBUS, PH_A/B/C, Hall/Encoder, USART, CAN, dan shutdown.
5. Cocokkan PA8/PB13, PA9/PB14, PA10/PB15 terhadap phase A/B/C.
6. Jelaskan mengapa PA12 harus berada pada kondisi shutdown selama reset.
7. Buat tabel perbandingan VFD, H-bridge DC, inverter 1-fasa, six-step BLDC, dan FOC.

## Data yang dikumpulkan
- jenis aktuator,
- supply,
- variabel yang dikontrol,
- sensor,
- controller,
- power converter,
- metode komunikasi,
- minimal tiga risiko dan pengendaliannya.

## Pertanyaan analisis
1. Mengapa PLC tidak langsung menggerakkan motor 3-fasa?
2. Apa beda command speed dan feedback speed?
3. Apa beda PWM carrier frequency dan electrical frequency?
4. Mengapa dead-time diperlukan?
5. Mengapa current sensing penting untuk FOC?
