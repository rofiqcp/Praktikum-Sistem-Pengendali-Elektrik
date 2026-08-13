#!/usr/bin/env bash
set -euo pipefail

projects=(
  Modul-05-Motor-DC-HBridge/program
  Modul-06-Project-PCB-Motor-Control/program
  Modul-07-PWM-Deadtime-Phase/program
  Modul-08-Responsi-1/program
  Modul-09-Inverter-1-Phase-SPWM/program
  Modul-10-BLDC-SixStep-Hall/program
  Modul-11-BLDC-SinePWM/program
  Modul-12-BLDC-SVPWM/program
  Modul-13-Sensor-Arus-Tegangan/program
  Modul-14-Clarke-Park-FOC/program
  Modul-15-FOC-Speed-Position/program
  Modul-16-Responsi-Final/program
)

for p in "${projects[@]}"; do
  echo "===== BUILD $p ====="
  platformio run -d "$p"
done

python3 Modul-01-Pengenalan-Pengendali-Elektrik/program/control_selector.py >/dev/null
python3 Modul-03-VFD-4-20mA/program/scaling_4_20mA.py >/dev/null
python3 -m py_compile Modul-04-VFD-RS485-Modbus/program/modbus_vfd_test.py

echo "All PlatformIO and Python smoke checks passed."
