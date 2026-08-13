# Program Pertemuan 01

Semua contoh P1 adalah **software-only** dan aman dijalankan tanpa hardware.

## Requirement

- Python 3.9+
- tanpa library eksternal

## Run

```bash
python3 control_selector.py
python3 control_selector.py --system foc
python3 power_flow_demo.py --system vfd
python3 power_flow_demo.py --system bldc
python3 dc_motor_pwm_demo.py --vbus 12 --duty 50
python3 dc_motor_pwm_demo.py --vbus 12 --duty -50
```

## Google Colab

Upload file `.py`, lalu jalankan:

```python
!python control_selector.py --system foc
```

## Target belajar

Program tidak mengendalikan hardware. Tujuannya adalah membuat mahasiswa memahami struktur sistem sebelum masuk ke PLC/VFD dan power electronics.