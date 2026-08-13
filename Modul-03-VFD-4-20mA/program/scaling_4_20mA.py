#!/usr/bin/env python3
FMIN, FMAX = 0.0, 50.0
for current in [4, 8, 12, 16, 20]:
    x = min(20.0, max(4.0, float(current)))
    freq = FMIN + (x - 4.0) / 16.0 * (FMAX - FMIN)
    print(f"{x:5.1f} mA -> {freq:6.2f} Hz")
