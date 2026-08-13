#!/usr/bin/env python3
"""Calculate Modbus RTU CRC16 for educational frame inspection."""

import argparse


def crc16_modbus(data: bytes) -> int:
    crc = 0xFFFF
    for byte in data:
        crc ^= byte
        for _ in range(8):
            if crc & 1:
                crc = (crc >> 1) ^ 0xA001
            else:
                crc >>= 1
    return crc & 0xFFFF


def parse_hex(text: str) -> bytes:
    cleaned = text.replace(",", " ").replace("0x", "")
    try:
        return bytes(int(x, 16) for x in cleaned.split())
    except ValueError as exc:
        raise SystemExit(f"Invalid hex input: {exc}")


def main():
    p = argparse.ArgumentParser()
    p.add_argument("hex_bytes", help='Example: "01 03 00 00 00 01"')
    a = p.parse_args()
    payload = parse_hex(a.hex_bytes)
    crc = crc16_modbus(payload)
    lo = crc & 0xFF
    hi = (crc >> 8) & 0xFF
    frame = payload + bytes([lo, hi])
    print("payload :", " ".join(f"{b:02X}" for b in payload))
    print(f"CRC16   : 0x{crc:04X}")
    print(f"CRC low : 0x{lo:02X}")
    print(f"CRC high: 0x{hi:02X}")
    print("frame   :", " ".join(f"{b:02X}" for b in frame))


if __name__ == "__main__":
    main()
