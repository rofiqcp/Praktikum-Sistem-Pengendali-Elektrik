# Program Pertemuan 03

## Standard-library programs

```bash
python3 scaling_4_20mA.py --current 12 --fmin 0 --fmax 50
python3 scaling_4_20mA.py --frequency 25 --fmin 0 --fmax 50
python3 generate_characterization.py --fmin 0 --fmax 50 --output hasil_pengamatan.csv
python3 analyze_characterization.py hasil_pengamatan.csv
```

## Plot

```bash
python3 -m venv .venv
source .venv/bin/activate     # Linux/macOS
# .venv\Scripts\activate     # Windows PowerShell
pip install -r requirements.txt
python3 analyze_characterization.py hasil_pengamatan.csv --plot
```

## PLC reference

`wecon_4_20mA_scaling.st` is a logic reference. Replace `RAW_MIN` and `RAW_MAX` from the exact analog-output module manual before use.
