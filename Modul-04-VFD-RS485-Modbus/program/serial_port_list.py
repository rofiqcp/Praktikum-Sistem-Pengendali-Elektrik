#!/usr/bin/env python3
"""List serial ports visible to Python."""

from serial.tools import list_ports

ports = list(list_ports.comports())
if not ports:
    print("No serial ports detected")
else:
    for p in ports:
        print(f"{p.device:20s} | {p.description} | hwid={p.hwid}")
