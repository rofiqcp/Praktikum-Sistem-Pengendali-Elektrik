(*
  P3 reference Structured Text.
  RAW_MIN/RAW_MAX MUST be taken from the actual WECON analog output manual.
*)
VAR
  F_CMD_HZ   : REAL;
  F_MIN_HZ   : REAL := 0.0;
  F_MAX_HZ   : REAL := 50.0;
  I_CMD_MA   : REAL;
  NORM       : REAL;
  RAW_MIN    : DINT := 0;      (* replace from manual *)
  RAW_MAX    : DINT := 4000;   (* replace from manual/example only *)
  AO_RAW     : DINT;
  I_MEAS_MA  : REAL;
  BELOW_RANGE : BOOL;
  ABOVE_RANGE : BOOL;
END_VAR

(* Clamp frequency command *)
IF F_CMD_HZ < F_MIN_HZ THEN
  F_CMD_HZ := F_MIN_HZ;
ELSIF F_CMD_HZ > F_MAX_HZ THEN
  F_CMD_HZ := F_MAX_HZ;
END_IF;

IF F_MAX_HZ > F_MIN_HZ THEN
  NORM := (F_CMD_HZ - F_MIN_HZ) / (F_MAX_HZ - F_MIN_HZ);
ELSE
  NORM := 0.0;
END_IF;

I_CMD_MA := 4.0 + 16.0 * NORM;
AO_RAW := RAW_MIN + REAL_TO_DINT(NORM * DINT_TO_REAL(RAW_MAX - RAW_MIN));

BELOW_RANGE := I_MEAS_MA < 4.0;
ABOVE_RANGE := I_MEAS_MA > 20.0;
