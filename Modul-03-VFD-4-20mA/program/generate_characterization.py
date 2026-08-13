#!/usr/bin/env python3
"""Generate CSV template for P3 characterization."""

import argparse
import csv


def f_from_i(i, fmin, fmax):
    return fmin + (i - 4.0) / 16.0 * (fmax - fmin)


def main():
    p = argparse.ArgumentParser()
    p.add_argument("--fmin", type=float, default=0.0)
    p.add_argument("--fmax", type=float, default=50.0)
    p.add_argument("--output", default="hasil_pengamatan.csv")
    a = p.parse_args()

    points = [4, 6, 8, 12, 13, 16, 18, 20]
    fields = [
        "current_command_mA",
        "current_measured_mA",
        "frequency_theory_Hz",
        "frequency_reference_Hz",
        "frequency_output_Hz",
        "notes",
    ]
    with open(a.output, "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=fields)
        w.writeheader()
        for i in points:
            w.writerow({
                "current_command_mA": i,
                "frequency_theory_Hz": f"{f_from_i(i, a.fmin, a.fmax):.6f}",
            })
    print(f"Created {a.output} with {len(points)} test points")


if __name__ == "__main__":
    main()
