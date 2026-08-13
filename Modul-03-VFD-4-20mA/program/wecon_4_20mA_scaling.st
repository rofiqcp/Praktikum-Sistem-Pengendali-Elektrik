(* Generic IEC 61131-3 scaling. Sesuaikan raw AO PLC aktual. *)
VAR
  PercentCmd : REAL; (* 0..100 *)
  Current_mA : REAL;
  Frequency_Hz : REAL;
END_VAR

IF PercentCmd < 0.0 THEN PercentCmd := 0.0; END_IF;
IF PercentCmd > 100.0 THEN PercentCmd := 100.0; END_IF;
Current_mA := 4.0 + 16.0 * PercentCmd / 100.0;
Frequency_Hz := 50.0 * PercentCmd / 100.0;
(* Konversikan Current_mA ke raw analog output sesuai manual modul AO WECON. *)
