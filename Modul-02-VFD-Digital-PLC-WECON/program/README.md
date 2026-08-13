# Program Pertemuan 02

## File

- `wecon_digital_logic.st` — reference Structured Text; translate/adapt to the language supported by the actual PLC project.
- `digital_logic_sim.py` — runnable software-only test of latch, FWD/REV interlock, fault, and speed code.
- `io_map_template.csv` — fill before programming PLC.
- `parameter_checklist.csv` — fill from the exact WECON VFD manual.

## Run simulator

```bash
python3 digital_logic_sim.py
python3 digital_logic_sim.py --interactive
```

## Safety rule

Python simulation does not control the VFD. PLC/VFD commissioning must follow `Jobsheet.md` and must begin with logic/terminal verification before motor RUN.
