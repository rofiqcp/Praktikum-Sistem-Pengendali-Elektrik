#!/usr/bin/env python3
"""Software-only simulator untuk logic PLC→VFD P2."""

import argparse
from dataclasses import dataclass


@dataclass
class Inputs:
    start: bool = False
    stop_ok: bool = True
    fwd: bool = False
    rev: bool = False
    ms0: bool = False
    ms1: bool = False
    vfd_ok: bool = True
    estop_ok: bool = True


class Controller:
    def __init__(self):
        self.run_latch = False

    def step(self, x: Inputs):
        permit = x.stop_ok and x.vfd_ok and x.estop_ok
        conflict = x.fwd and x.rev
        if (not permit) or conflict:
            self.run_latch = False
        elif x.start:
            self.run_latch = True

        fwd_out = self.run_latch and x.fwd and not x.rev
        rev_out = self.run_latch and x.rev and not x.fwd
        speed_code = (2 if x.ms1 else 0) + (1 if x.ms0 else 0)
        return {
            "permit": permit,
            "run_latch": self.run_latch,
            "dir_conflict": conflict,
            "fwd_out": fwd_out,
            "rev_out": rev_out,
            "speed_code": speed_code,
        }


def assert_safe(name, out):
    assert not (out["fwd_out"] and out["rev_out"]), f"UNSAFE {name}: FWD+REV"


def run_tests():
    tests = [
        ("idle", Inputs()),
        ("start_fwd", Inputs(start=True, fwd=True)),
        ("direction_conflict", Inputs(start=True, fwd=True, rev=True)),
        ("start_rev", Inputs(start=True, rev=True, ms0=True)),
        ("fault", Inputs(start=True, fwd=True, vfd_ok=False)),
        ("estop", Inputs(start=True, fwd=True, estop_ok=False)),
        ("speed3", Inputs(start=True, fwd=True, ms0=True, ms1=True)),
    ]
    for name, inp in tests:
        c = Controller()  # independent test
        out = c.step(inp)
        assert_safe(name, out)
        print(f"{name:20s} -> {out}")
    print("\nPASS: simulator tidak menghasilkan FWD dan REV bersamaan.")


def interactive():
    c = Controller()
    print("Masukkan 0/1. Ctrl+C untuk keluar.")
    while True:
        def b(prompt, default="0"):
            raw = input(f"{prompt} [{default}]: ").strip() or default
            return raw == "1"

        x = Inputs(
            start=b("START"),
            stop_ok=b("STOP_OK", "1"),
            fwd=b("FWD_SEL"),
            rev=b("REV_SEL"),
            ms0=b("MS0"),
            ms1=b("MS1"),
            vfd_ok=b("VFD_OK", "1"),
            estop_ok=b("ESTOP_OK", "1"),
        )
        out = c.step(x)
        assert_safe("interactive", out)
        print(out, "\n")


def main():
    p = argparse.ArgumentParser()
    p.add_argument("--interactive", action="store_true")
    a = p.parse_args()
    interactive() if a.interactive else run_tests()


if __name__ == "__main__":
    main()
