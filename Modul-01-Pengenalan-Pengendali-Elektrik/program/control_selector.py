#!/usr/bin/env python3
"""Perbandingan struktur sistem pengendali elektrik.

Run:
  python3 control_selector.py
  python3 control_selector.py --system foc
"""

import argparse

SYSTEMS = {
    "vfd": {
        "plant": "Motor AC induksi",
        "converter": "VFD: rectifier + DC-link + inverter 3-phase",
        "command": "Digital terminal / 4-20 mA / RS485 Modbus",
        "feedback": "Status VFD, output frequency, optional encoder/sensor",
        "controlled": "run, direction, frequency/speed",
        "protection": "VFD fault, overload, interlock, emergency stop",
    },
    "dc": {
        "plant": "Motor DC brushed",
        "converter": "H-bridge",
        "command": "signed duty (-100..100%)",
        "feedback": "optional current + encoder",
        "controlled": "direction, voltage average, speed/position",
        "protection": "current limit, dead-time, shutdown",
    },
    "inverter": {
        "plant": "Beban AC / transformer",
        "converter": "Full-bridge 1-phase",
        "command": "SPWM amplitude + output frequency",
        "feedback": "voltage/current as required",
        "controlled": "AC fundamental voltage/frequency",
        "protection": "overcurrent, VBUS, dead-time, isolation",
    },
    "sixstep": {
        "plant": "BLDC",
        "converter": "3-phase inverter / six-step commutation",
        "command": "direction + duty",
        "feedback": "Hall sector",
        "controlled": "commutation and speed",
        "protection": "invalid Hall, current limit, shutdown",
    },
    "foc": {
        "plant": "BLDC/PMSM",
        "converter": "3-phase inverter + SVPWM",
        "command": "Id/Iq or cascaded speed/position setpoint",
        "feedback": "IA/IB + electrical angle from encoder/Hall/observer",
        "controlled": "torque/current, speed, position",
        "protection": "current/voltage limit, sensor validity, shutdown",
    },
}


def print_system(name: str) -> None:
    s = SYSTEMS[name]
    print(f"\n=== {name.upper()} ===")
    for key in ("plant", "converter", "command", "feedback", "controlled", "protection"):
        print(f"{key:11s}: {s[key]}")


def main() -> None:
    parser = argparse.ArgumentParser(description="Peta sistem pengendali elektrik")
    parser.add_argument("--system", choices=SYSTEMS, help="Tampilkan satu sistem")
    args = parser.parse_args()

    print("PRAKTIKUM SISTEM PENGENDALI ELEKTRIK - P1")
    if args.system:
        print_system(args.system)
    else:
        for name in SYSTEMS:
            print_system(name)

    print("\nPertanyaan: mana yang merupakan power path, control path, dan feedback path?")


if __name__ == "__main__":
    main()
