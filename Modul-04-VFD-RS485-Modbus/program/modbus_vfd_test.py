#!/usr/bin/env python3
import argparse, json, time
from pymodbus.client import ModbusSerialClient

p = argparse.ArgumentParser()
p.add_argument("--config", default="vfd_config.json")
p.add_argument("--read-only", action="store_true")
p.add_argument("--hz", type=float, default=5.0)
a = p.parse_args()
cfg = json.load(open(a.config, encoding="utf-8"))

required_read = ["register_output_frequency"]
required_write = ["register_command", "register_frequency_ref", "frequency_scale_hz_per_count", "command_stop", "command_run_forward"]
for k in required_read + ([] if a.read_only else required_write):
    if cfg.get(k) is None:
        raise SystemExit(f"Isi '{k}' dari manual VFD aktual sebelum menjalankan program.")

client = ModbusSerialClient(port=cfg["port"], baudrate=cfg["baudrate"], parity=cfg["parity"], stopbits=cfg["stopbits"], timeout=1)
if not client.connect():
    raise SystemExit("RS485 tidak terhubung")
slave = cfg["slave"]

def read_out():
    r = client.read_holding_registers(cfg["register_output_frequency"], count=1, slave=slave)
    if r.isError(): raise RuntimeError(r)
    return r.registers[0] * float(cfg.get("frequency_scale_hz_per_count") or 1.0)

try:
    print("Output frequency:", read_out(), "Hz")
    if a.read_only:
        raise SystemExit(0)
    client.write_register(cfg["register_command"], cfg["command_stop"], slave=slave)
    counts = round(a.hz / cfg["frequency_scale_hz_per_count"])
    client.write_register(cfg["register_frequency_ref"], counts, slave=slave)
    client.write_register(cfg["register_command"], cfg["command_run_forward"], slave=slave)
    for _ in range(10):
        print(f"f_out={read_out():.2f} Hz")
        time.sleep(0.5)
finally:
    if not a.read_only and cfg.get("register_command") is not None:
        try: client.write_register(cfg["register_command"], cfg["command_stop"], slave=slave)
        except Exception: pass
    client.close()
