#!/usr/bin/env python3
systems = {
    "vfd": ("Motor AC", "frequency/voltage", "digital, 4-20mA, RS485"),
    "dc": ("Motor DC", "H-bridge PWM", "duty + direction"),
    "inverter": ("AC 1-phase", "full-bridge SPWM", "carrier + output frequency"),
    "sixstep": ("BLDC", "6-step inverter", "Hall sector"),
    "foc": ("BLDC/PMSM", "SVPWM + dq current", "angle + IA/IB")
}
print("Sistem pengendali elektrik")
for key, value in systems.items():
    print(f"{key:8s}: load={value[0]:12s} converter={value[1]:20s} command={value[2]}")
