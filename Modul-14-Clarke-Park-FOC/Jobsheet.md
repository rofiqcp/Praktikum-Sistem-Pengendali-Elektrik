# Jobsheet 14 — Current FOC dengan Hall Angle

1. Verifikasi P13 selesai dan offset arus stabil.
2. Putar rotor manual, pastikan Hall sector valid.
3. Dengan gate shutdown, log IA/IB/IC→Iα/Iβ→Id/Iq.
4. Uji transform menggunakan arus/sinyal yang diketahui atau data offline.
5. Enable FOC pada VBUS 12 V current-limit dan `Iq*=0.3 A`.
6. Naikkan 0.5, 1.0 A bila stabil.
7. Uji Iq positif dan negatif dari kondisi aman.
8. Catat Id, Iq, Vd, Vq, duty dan IDC.

STOP bila Hall invalid, VBUS di bawah batas, current melebihi limit, atau motor bergetar keras. Jika torque direction salah, koreksi phase/Hall angle offset—jangan menutupinya dengan gain lebih besar.
