(*
  Pertemuan 02 - Reference IEC 61131-3 Structured Text
  -----------------------------------------------------
  Logic reference only. Sesuaikan syntax dan mapping I/O dengan PLC WECON
  aktual serta editor yang digunakan di laboratorium.
*)

VAR
  START_PB     : BOOL;
  STOP_OK      : BOOL;   (* TRUE = stop circuit healthy *)
  FWD_SEL      : BOOL;
  REV_SEL      : BOOL;
  SPEED_BIT0   : BOOL;
  SPEED_BIT1   : BOOL;
  VFD_OK       : BOOL;
  ESTOP_OK     : BOOL;

  RUN_LATCH    : BOOL;
  PERMIT_RUN   : BOOL;
  DIR_CONFLICT : BOOL;
  SPEED_CODE   : INT;

  DO_FWD       : BOOL;
  DO_REV       : BOOL;
  DO_MS0       : BOOL;
  DO_MS1       : BOOL;
  LAMP_RUN     : BOOL;
  LAMP_FAULT   : BOOL;
END_VAR

(* Permit *)
PERMIT_RUN := STOP_OK AND VFD_OK AND ESTOP_OK;
DIR_CONFLICT := FWD_SEL AND REV_SEL;

(* STOP / FAULT dominant latch *)
IF (NOT PERMIT_RUN) OR DIR_CONFLICT THEN
  RUN_LATCH := FALSE;
ELSIF START_PB THEN
  RUN_LATCH := TRUE;
END_IF;

(* Mutual exclusion *)
DO_FWD := RUN_LATCH AND FWD_SEL AND NOT REV_SEL;
DO_REV := RUN_LATCH AND REV_SEL AND NOT FWD_SEL;

(* Multi-speed selection remains available as a code; VFD decides actual Hz. *)
DO_MS0 := SPEED_BIT0;
DO_MS1 := SPEED_BIT1;

IF SPEED_BIT1 THEN
  SPEED_CODE := 2;
ELSE
  SPEED_CODE := 0;
END_IF;
IF SPEED_BIT0 THEN
  SPEED_CODE := SPEED_CODE + 1;
END_IF;

LAMP_RUN := DO_FWD OR DO_REV;
LAMP_FAULT := (NOT VFD_OK) OR (NOT ESTOP_OK) OR DIR_CONFLICT;
