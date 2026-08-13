# Modul 06 — Project PCB Motor Control Rev.E

Pertemuan ini menjadikan PCB sebagai alat praktikum utama semester: satu board harus dapat digunakan untuk motor DC, inverter 1-fasa, BLDC six-step, sine PWM, SVPWM dan FOC.

## Arsitektur board
- VBUS target 42 V, fuse dan DC-link.
- regulator 42→12 V gate, 12→5 V logic, Black Pill menghasilkan 3.3 V.
- 3×IR2110 dan 6 N-MOSFET membentuk inverter 3-phase.
- shunt phase A/B dan DC masing-masing 2 mΩ.
- LM358 current amplifier gain sekitar 20; zero-current sekitar VREF 1.65 V.
- VBUS divider 21:1.
- Hall/Encoder PB6/PB7/PB2.
- MCP2515+MCP2562 CAN, I2C, USART dan empat tombol.

## Prinsip layout kritis
Kelvin trace shunt dipisahkan dari power trace. Gate driver dan resistor gate dekat MOSFET. DC-link film/ceramic dekat switching loop. Pisahkan PGND dan GND sesuai netlist. Cek pinout MOSFET fisik sebelum footprint dikunci.

## Filosofi bring-up
Board belum dinyatakan selesai hanya karena soldering selesai. Deliverable project adalah **PCB yang lolos checklist listrik dan dapat menjalankan firmware diagnostic**.
