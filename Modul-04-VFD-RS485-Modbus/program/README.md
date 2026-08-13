# Program Pertemuan 04 — RS485 Modbus RTU

## Install

```bash
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
```

Windows PowerShell:

```powershell
py -m venv .venv
.\.venv\Scripts\Activate.ps1
pip install -r requirements.txt
```

## Utilities

```bash
python3 serial_port_list.py
python3 modbus_crc_demo.py "01 03 00 00 00 01"
```

## Configuration

```bash
cp vfd_config.example.json vfd_config.json
```

Fill every register/command value from the exact WECON VFD manual.

## Validation and read-only

```bash
python3 modbus_vfd_test.py --config vfd_config.json --validate-only
python3 modbus_vfd_test.py --config vfd_config.json --read-only
```

## Write/RUN

Only after read-only verification and lab authorization:

```bash
python3 modbus_vfd_test.py --config vfd_config.json --run --hz 5
```

The program sends STOP before RUN and attempts STOP again in `finally`. This is a software measure only; it does not replace emergency stop or VFD hardware safety.
