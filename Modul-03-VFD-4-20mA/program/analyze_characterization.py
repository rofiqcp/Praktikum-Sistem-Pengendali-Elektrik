#!/usr/bin/env python3
"""Analyze completed P3 CSV using standard-library linear regression."""

import argparse
import csv
import math


def load_rows(path):
    rows = []
    with open(path, newline="", encoding="utf-8") as f:
        for r in csv.DictReader(f):
            try:
                rows.append({
                    "i": float(r["current_measured_mA"]),
                    "theory": float(r["frequency_theory_Hz"]),
                    "out": float(r["frequency_output_Hz"]),
                })
            except (ValueError, TypeError):
                continue
    return rows


def linear_fit(xs, ys):
    n = len(xs)
    mx, my = sum(xs) / n, sum(ys) / n
    denom = sum((x - mx) ** 2 for x in xs)
    if denom == 0:
        raise ValueError("all x values are identical")
    m = sum((x - mx) * (y - my) for x, y in zip(xs, ys)) / denom
    b = my - m * mx
    return m, b


def main():
    p = argparse.ArgumentParser()
    p.add_argument("csv_file")
    p.add_argument("--plot", action="store_true")
    a = p.parse_args()

    rows = load_rows(a.csv_file)
    if len(rows) < 2:
        raise SystemExit("Need at least two completed measurement rows")

    xs = [r["i"] for r in rows]
    ys = [r["out"] for r in rows]
    m, b = linear_fit(xs, ys)
    errors = [r["out"] - r["theory"] for r in rows]
    rmse = math.sqrt(sum(e * e for e in errors) / len(errors))

    print(f"valid points      : {len(rows)}")
    print(f"measured slope    : {m:.6f} Hz/mA")
    print(f"measured intercept: {b:.6f} Hz")
    print(f"max abs error     : {max(abs(e) for e in errors):.6f} Hz")
    print(f"RMSE              : {rmse:.6f} Hz")

    if a.plot:
        try:
            import matplotlib.pyplot as plt
        except ImportError:
            raise SystemExit("Install matplotlib: pip install -r requirements.txt")
        plt.figure()
        plt.plot(xs, [r["theory"] for r in rows], "o-", label="theory")
        plt.plot(xs, ys, "x-", label="measured output")
        plt.xlabel("Measured current (mA)")
        plt.ylabel("Frequency (Hz)")
        plt.grid(True)
        plt.legend()
        plt.tight_layout()
        plt.show()


if __name__ == "__main__":
    main()
