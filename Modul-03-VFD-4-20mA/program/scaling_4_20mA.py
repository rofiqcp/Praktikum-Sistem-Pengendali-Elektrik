#!/usr/bin/env python3
"""Bidirectional 4-20 mA scaling calculator."""

import argparse


def clamp(x, lo, hi):
    return max(lo, min(hi, x))


def current_to_frequency(i_ma, fmin, fmax, do_clamp=True):
    i = clamp(i_ma, 4.0, 20.0) if do_clamp else i_ma
    return fmin + (i - 4.0) / 16.0 * (fmax - fmin)


def frequency_to_current(freq, fmin, fmax, do_clamp=True):
    f = clamp(freq, fmin, fmax) if do_clamp else freq
    if fmax == fmin:
        raise ValueError("fmax must differ from fmin")
    return 4.0 + 16.0 * (f - fmin) / (fmax - fmin)


def classify_current(i):
    if i < 4.0:
        return "BELOW_RANGE"
    if i > 20.0:
        return "ABOVE_RANGE"
    return "VALID_RANGE"


def main():
    p = argparse.ArgumentParser()
    g = p.add_mutually_exclusive_group(required=True)
    g.add_argument("--current", type=float, help="current in mA")
    g.add_argument("--frequency", type=float, help="frequency in Hz")
    p.add_argument("--fmin", type=float, default=0.0)
    p.add_argument("--fmax", type=float, default=50.0)
    p.add_argument("--no-clamp", action="store_true")
    a = p.parse_args()

    if a.current is not None:
        f = current_to_frequency(a.current, a.fmin, a.fmax, not a.no_clamp)
        print(f"current input : {a.current:.4f} mA")
        print(f"classification: {classify_current(a.current)}")
        print(f"frequency     : {f:.4f} Hz")
    else:
        i = frequency_to_current(a.frequency, a.fmin, a.fmax, not a.no_clamp)
        print(f"frequency cmd : {a.frequency:.4f} Hz")
        print(f"current cmd   : {i:.4f} mA")


if __name__ == "__main__":
    main()
