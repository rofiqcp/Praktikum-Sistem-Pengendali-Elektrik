#!/usr/bin/env python3
"""Demo matematis signed-duty motor DC.

Ini BUKAN driver hardware. Program hanya membantu memahami hubungan ideal
signed duty dan tegangan rata-rata H-bridge.
"""

import argparse


def clamp(x, lo, hi):
    return max(lo, min(hi, x))


def main():
    p = argparse.ArgumentParser()
    p.add_argument("--vbus", type=float, default=12.0, help="DC bus voltage")
    p.add_argument("--duty", type=float, default=50.0, help="signed duty -100..100 %%")
    a = p.parse_args()

    duty = clamp(a.duty, -100.0, 100.0)
    signed_ratio = duty / 100.0
    vavg = signed_ratio * a.vbus

    if duty > 0:
        direction = "FORWARD"
    elif duty < 0:
        direction = "REVERSE"
    else:
        direction = "STOP/COAST (depends on switching strategy)"

    print("DC MOTOR PWM IDEAL DEMO")
    print(f"VBUS            = {a.vbus:.3f} V")
    print(f"signed duty     = {duty:.2f} %")
    print(f"direction       = {direction}")
    print(f"Vmotor average  ≈ {vavg:.3f} V (ideal simplified model)")
    print("\nCatatan: kecepatan aktual tidak dihitung karena dipengaruhi R, L, Ke, load, friction, and losses.")


if __name__ == "__main__":
    main()
