# Modul 14 — Clarke, Park dan Current FOC

## Alur matematis
1. Baca IA dan IB, rekonstruksi `IC=-IA-IB`.
2. Clarke:
   - `Iα=IA`
   - `Iβ=(IA+2IB)/√3`
3. Park menggunakan electrical angle θ:
   - `Id=Iα cosθ + Iβ sinθ`
   - `Iq=-Iα sinθ + Iβ cosθ`
4. PI current menghasilkan `Vd,Vq`.
5. Inverse Park → `Vα,Vβ`.
6. SVPWM → duty A/B/C.

`Id` mengatur komponen flux-axis, sedangkan `Iq` terutama berkaitan dengan torque-producing current pada PMSM/BLDC yang dimodelkan sinusoidal.

## Angle P14
P14 memakai Hall sector sebagai estimasi electrical angle kasar (60° resolution). Ini cukup untuk demonstrasi alur FOC low-speed/low-current, tetapi ripple akan lebih besar. P15 beralih ke encoder AB untuk angle lebih halus.

## Kontrol
Reference awal `Id*=0 A`, `Iq*` kecil. PI gain adalah nilai awal eksperimen, bukan universal. Selalu tune dari low voltage/current.
