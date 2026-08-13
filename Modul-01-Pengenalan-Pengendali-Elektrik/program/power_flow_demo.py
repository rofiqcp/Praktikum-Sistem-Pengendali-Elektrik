#!/usr/bin/env python3
"""Menampilkan power path dan control path secara tekstual."""

import argparse

PATHS = {
    "vfd": {
        "power": ["AC SUPPLY", "VFD RECTIFIER", "DC LINK", "3-PHASE INVERTER", "MOTOR AC"],
        "control": ["PUSH BUTTON/HMI", "PLC", "DI / 4-20mA / RS485", "VFD CONTROLLER"],
        "feedback": ["VFD STATUS", "OUTPUT FREQUENCY", "FAULT"],
    },
    "dc": {
        "power": ["DC SUPPLY", "DC LINK", "H-BRIDGE", "MOTOR DC"],
        "control": ["SETPOINT", "STM32", "PWM + DIRECTION", "GATE DRIVER"],
        "feedback": ["CURRENT", "ENCODER (OPTIONAL)"],
    },
    "bldc": {
        "power": ["DC SUPPLY", "DC LINK", "3 HALF-BRIDGE", "BLDC/PMSM"],
        "control": ["SETPOINT", "STM32", "SIX-STEP/SVPWM/FOC", "IR2110", "MOSFET"],
        "feedback": ["IA/IB", "VBUS", "HALL/ENCODER"],
    },
}


def arrow(items):
    return " -> ".join(items)


def main():
    p = argparse.ArgumentParser()
    p.add_argument("--system", choices=PATHS, default="vfd")
    a = p.parse_args()
    s = PATHS[a.system]
    print(f"SYSTEM: {a.system.upper()}")
    print("POWER   :", arrow(s["power"]))
    print("CONTROL :", arrow(s["control"]))
    print("FEEDBACK:", arrow(s["feedback"]))
    print("\nJelaskan mengapa jalur POWER dan CONTROL tidak boleh dianggap sama.")


if __name__ == "__main__":
    main()
