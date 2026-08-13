#!/usr/bin/env python3
"""Safe teaching client for a WECON VFD over Modbus RTU.

The program contains NO hard-coded VFD register addresses. Fill the JSON file
from the exact VFD manual. Default operational mode is validation/read-only;
writes occur only with --run and complete write configuration.
"""

import argparse
import json
import time
from pathlib import Path

from pymodbus.client import ModbusSerialClient


def load_config(path):
    with open(path, encoding="utf-8") as f:
        return json.load(f)


def require(cfg, keys):
    missing = [k for k in keys if cfg.get(k) is None]
    if missing:
        raise SystemExit("Missing config fields: " + ", ".join(missing))


def build_client(cfg):
    return ModbusSerialClient(
        port=cfg["port"],
        baudrate=int(cfg["baudrate"]),
        parity=str(cfg["parity"]),
        stopbits=int(cfg["stopbits"]),
        bytesize=8,
        timeout=float(cfg.get("timeout_s", 1.0)),
    )


def read_holding(client, address, slave):
    r = client.read_holding_registers(address, count=1, slave=slave)
    if r.isError():
        raise RuntimeError(f"Modbus error reading {address}: {r}")
    return r.registers[0]


def read_monitor(client, cfg):
    slave = int(cfg["slave"])
    raw_f = read_holding(client, int(cfg["register_output_frequency"]), slave)
    scale = float(cfg["output_frequency_scale_hz_per_count"])
    out = {"output_frequency_raw": raw_f, "output_frequency_hz": raw_f * scale}
    if cfg.get("register_status") is not None:
        out["status_raw"] = read_holding(client, int(cfg["register_status"]), slave)
    if cfg.get("register_fault") is not None:
        out["fault_raw"] = read_holding(client, int(cfg["register_fault"]), slave)
    return out


def write_register_checked(client, address, value, slave, label):
    r = client.write_register(int(address), int(value), slave=slave)
    if r.isError():
        raise RuntimeError(f"Modbus error writing {label}: {r}")


def validate(cfg, for_write=False):
    require(cfg, [
        "port", "baudrate", "parity", "stopbits", "slave",
        "register_output_frequency", "output_frequency_scale_hz_per_count",
    ])
    if for_write:
        require(cfg, [
            "register_command", "register_frequency_ref",
            "frequency_scale_hz_per_count",
            "command_stop", "command_run_forward",
        ])
        if float(cfg["frequency_scale_hz_per_count"]) <= 0:
            raise SystemExit("frequency_scale_hz_per_count must be > 0")


def main():
    p = argparse.ArgumentParser()
    p.add_argument("--config", default="vfd_config.json")
    mode = p.add_mutually_exclusive_group()
    mode.add_argument("--validate-only", action="store_true")
    mode.add_argument("--read-only", action="store_true")
    mode.add_argument("--run", action="store_true")
    p.add_argument("--hz", type=float, default=5.0)
    p.add_argument("--samples", type=int, default=10)
    p.add_argument("--interval", type=float, default=0.5)
    a = p.parse_args()

    if not Path(a.config).exists():
        raise SystemExit(f"Config not found: {a.config}. Copy vfd_config.example.json first.")
    cfg = load_config(a.config)
    validate(cfg, for_write=a.run)

    if a.validate_only:
        print("Configuration fields required for this mode are present.")
        return

    # Safe default: if no mode chosen, behave as read-only.
    if not a.run:
        a.read_only = True

    client = build_client(cfg)
    if not client.connect():
        raise SystemExit("Serial/RS485 connection failed")

    slave = int(cfg["slave"])
    stop_possible = a.run and cfg.get("register_command") is not None and cfg.get("command_stop") is not None

    try:
        first = read_monitor(client, cfg)
        print("Initial monitor:", first)

        if a.read_only:
            return

        max_hz = float(cfg.get("software_max_hz", 20.0))
        if a.hz < 0 or a.hz > max_hz:
            raise SystemExit(f"Requested {a.hz} Hz outside software lab limit 0..{max_hz} Hz")

        # Always STOP first.
        write_register_checked(client, cfg["register_command"], cfg["command_stop"], slave, "STOP")
        time.sleep(float(cfg.get("pre_run_stop_delay_s", 0.5)))

        counts = round(a.hz / float(cfg["frequency_scale_hz_per_count"]))
        write_register_checked(client, cfg["register_frequency_ref"], counts, slave, "frequency reference")
        write_register_checked(client, cfg["register_command"], cfg["command_run_forward"], slave, "RUN FORWARD")

        for idx in range(max(1, a.samples)):
            m = read_monitor(client, cfg)
            print(f"sample={idx:02d} command={a.hz:.3f}Hz monitor={m}")
            time.sleep(max(0.05, a.interval))

    finally:
        if stop_possible:
            try:
                write_register_checked(client, cfg["register_command"], cfg["command_stop"], slave, "STOP(finally)")
                print("STOP command attempted in finally block")
            except Exception as exc:
                print(f"WARNING: STOP attempt failed: {exc}")
        client.close()


if __name__ == "__main__":
    main()
