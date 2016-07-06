    0    0          geometry
SPH blkbody    0.E+00 0.E+00 0.E+00 2000.E+00
SPH air        0.E+00 0.E+00 0.E+00 199.E+00
!@what.3=targ_dist
$start_translat 0. 0. -0.1
RPP target     -0.75E+00 0.75E+00 -0.75E+00 0.75E+00 0.E+00 0.2E+00
$end_translat 
!@what.3=sc_dist
$start_translat 0. 0. -28.65
* ***********************
* *** Start Counter
* ***********************
RCC StartC     0.E+00 0.E+00 0.E+00 0.E+00 0.E+00 0.025E+00 2.6E+0
$end_translat 
!@what.3=vtx_dist
$start_translat 0.0 0.0 0.6
RPP vtx1       -1.E+00 1.E+00 -1.E+00 1.E+00 0.E+00 0.005E+00
RPP vtx2       -1.E+00 1.E+00 -1.E+00 1.E+00 0.505E+00 0.510E+00
RPP vtx3       -1.E+00 1.E+00 -1.E+00 1.E+00 1.010E+00 1.015E+00
$end_translat 
!@what.3=vtx4_dist
$start_translat 0. 0. 14.1
RPP vtx4       -3.E+00 3.E+00 -3.E+00 3.E+00 0. 0.005E+00
RPP vtx5       -3.E+00 3.E+00 -3.E+00 3.E+00 1. 1.005E+00
$end_translat 
!@what.3=mag_dist
$start_translat 0.0 0.0 2.6
* ************
* *** Magnet
* ************
!@what.6=mag_dim+0.4E+00
RCC MagAlOu1   0.E+00 0.E+00 -0.2E+00 0.E+00 0.E+00 10.4 7.4E+00
!@what.3=mag_dim+btwmag-0.2
!@what.6=mag_dim+0.4E+00
RCC MagAlOu2   0.E+00 0.E+00 13.8 0.E+00 0.E+00 10.4 7.4E+00
!@what.6=mag_dim
RCC MagPMOu1   0.E+00 0.E+00 0.E+00 0.E+00 0.E+00 10. 7.2E+00
!@what.3=mag_dim+btwmag
!@what.6=mag_dim
RCC MagPMOu2   0.E+00 0.E+00 14. 0.E+00 0.E+00 10. 7.2E+00
!@what.6=mag_dim
RCC MagPMIn1   0.E+00 0.E+00 0.E+00 0.E+00 0.E+00 10. 3.7E+00
!@what.3=mag_dim+btwmag
!@what.6=mag_dim
RCC MagPMIn2   0.E+00 0.E+00 14. 0.E+00 0.E+00 10. 3.7E+00
ZCC Gap        0.E+00 0.E+00 3.5E+00
$end_translat 
!@what.3=drift_dist
$start_translat 0.0 0.0 -13.65
* ***********************
* *** First Drift Chamber
* ***********************
*  each cell -> 1 region
*  V view
RPP VP1C1a     -2.800000 -1.200000 -5.600000 5.600000 -6.350000 -5.350000
RPP VP1C2a     -1.200000 0.400000 -5.600000 5.600000 -6.350000 -5.350000
RPP VP1C3a     0.400000 2.000000 -5.600000 5.600000 -6.350000 -5.350000
RPP VP2C1a     -2.000000 -0.400000 -5.600000 5.600000 -3.750000 -2.750000
RPP VP2C2a     -0.400000 1.200000 -5.600000 5.600000 -3.750000 -2.750000
RPP VP2C3a     1.200000 2.800000 -5.600000 5.600000 -3.750000 -2.750000
RPP VP3C1a     -2.800000 -1.200000 -5.600000 5.600000 -1.150000 -0.150000
RPP VP3C2a     -1.200000 0.400000 -5.600000 5.600000 -1.150000 -0.150000
RPP VP3C3a     0.400000 2.000000 -5.600000 5.600000 -1.150000 -0.150000
RPP VP4C1a     -2.000000 -0.400000 -5.600000 5.600000 1.450000 2.450000
RPP VP4C2a     -0.400000 1.200000 -5.600000 5.600000 1.450000 2.450000
RPP VP4C3a     1.200000 2.800000 -5.600000 5.600000 1.450000 2.450000
RPP VP5C1a     -2.800000 -1.200000 -5.600000 5.600000 4.050000 5.050000
RPP VP5C2a     -1.200000 0.400000 -5.600000 5.600000 4.050000 5.050000
RPP VP5C3a     0.400000 2.000000 -5.600000 5.600000 4.050000 5.050000
RPP VP6C1a     -2.000000 -0.400000 -5.600000 5.600000 6.650000 7.650000
RPP VP6C2a     -0.400000 1.200000 -5.600000 5.600000 6.650000 7.650000
RPP VP6C3a     1.200000 2.800000 -5.600000 5.600000 6.650000 7.650000
*  U view
RPP UP1C1a     -5.600000 5.600000 -2.000000 -0.400000 -7.650000 -6.650000
RPP UP1C2a     -5.600000 5.600000 -0.400000 1.200000 -7.650000 -6.650000
RPP UP1C3a     -5.600000 5.600000 1.200000 2.800000 -7.650000 -6.650000
RPP UP2C1a     -5.600000 5.600000 -2.800000 -1.200000 -5.050000 -4.050000
RPP UP2C2a     -5.600000 5.600000 -1.200000 0.400000 -5.050000 -4.050000
RPP UP2C3a     -5.600000 5.600000 0.400000 2.000000 -5.050000 -4.050000
RPP UP3C1a     -5.600000 5.600000 -2.000000 -0.400000 -2.450000 -1.450000
RPP UP3C2a     -5.600000 5.600000 -0.400000 1.200000 -2.450000 -1.450000
RPP UP3C3a     -5.600000 5.600000 1.200000 2.800000 -2.450000 -1.450000
RPP UP4C1a     -5.600000 5.600000 -2.800000 -1.200000 0.150000 1.150000
RPP UP4C2a     -5.600000 5.600000 -1.200000 0.400000 0.150000 1.150000
RPP UP4C3a     -5.600000 5.600000 0.400000 2.000000 0.150000 1.150000
RPP UP5C1a     -5.600000 5.600000 -2.000000 -0.400000 2.750000 3.750000
RPP UP5C2a     -5.600000 5.600000 -0.400000 1.200000 2.750000 3.750000
RPP UP5C3a     -5.600000 5.600000 1.200000 2.800000 2.750000 3.750000
RPP UP6C1a     -5.600000 5.600000 -2.800000 -1.200000 5.350000 6.350000
RPP UP6C2a     -5.600000 5.600000 -1.200000 0.400000 5.350000 6.350000
RPP UP6C3a     -5.600000 5.600000 0.400000 2.000000 5.350000 6.350000
*  Drift Chamber mechanical structure
RPP SHIINa     -5.60 5.60 -5.60 5.60 -11.5 11.50
RPP SHIOUa     -6.85 6.85 -6.85 6.85 -11.5 11.50
* alette
RPP BARU1a     -5.00 -4.12 6.85 13.35 -7.20 7.20
RPP BARU2a     4.12 5.00 6.85 13.35 -7.20 7.20
RPP BARL1a     -13.35 -6.85 -5.00 -4.12 -7.20 7.20
RPP BARL2a     -13.35 -6.85 4.12 5.00 -7.20 7.20
RPP BARR1a     6.85 9.35 4.12 5.00 -7.20 7.20
RPP BARR2a     6.85 9.35 -5.00 -4.12 -7.20 7.20
RPP BARD1a     -5.00 -4.12 -9.35 -6.85 -7.20 7.20
RPP BARD2a     4.12 5.00 -9.35 -6.85 -7.20 7.20
* triangoli
RPP TRIA1a     -12.48 -5.00 5.00 12.48 -7.27 -7.20
RPP TRIA2a     -12.48 -5.00 5.00 12.48 7.20 7.27
PLA TRIA3a     -1.00 1.00 0.00 -5.00 12.48 7.20
* sostegno
RPP SOSA1a     -6.85 6.85 -7.98 -6.85 -8.34 -7.20
RPP SOSA2a     -6.85 6.85 -7.98 -6.85 7.20 8.34
RPP SOSB1a     -5.00 -3.75 -11.48 -7.98 7.20 8.34
RPP SOSB2a     3.75 5.00 -11.48 -7.98 7.20 8.34
RPP SOSB3a     -5.00 -3.75 -11.48 -7.98 -8.34 -7.20
RPP SOSB4a     3.75 5.00 -11.48 -7.98 -8.34 -7.20
RPP SOSC1a     -7.00 7.00 -12.98 -11.48 -8.70 -6.84
RPP SOSC2a     -7.00 7.00 -12.98 -11.48 6.84 8.70
* schede preamp U side
RPP SCHU1a     -3.25 3.25 6.85 12.47 -7.15 -7.12
RPP SCHU2a     -3.25 3.25 6.85 12.47 -4.50 -4.47
RPP SCHU3a     -3.25 3.25 6.85 12.47 -1.98 -1.95
RPP SCHU4a     -3.25 3.25 6.85 12.47 0.65 0.68
RPP SCHU5a     -3.25 3.25 6.85 12.47 3.20 3.23
RPP SCHU6a     -3.25 3.25 6.85 12.47 5.85 5.88
* schede preamp V side
RPP SCHV1a     -12.47 -6.85 -3.25 3.25 -5.85 -5.82
RPP SCHV2a     -12.47 -6.85 -3.25 3.25 -3.25 -3.22
RPP SCHV3a     -12.47 -6.85 -3.25 3.25 -0.65 -0.62
RPP SCHV4a     -12.47 -6.85 -3.25 3.25 1.95 1.98
RPP SCHV5a     -12.47 -6.85 -3.25 3.25 4.55 4.58
RPP SCHV6a     -12.47 -6.85 -3.25 3.25 7.15 7.18
$end_translat 
$start_translat 0.0 0.0 40.
* ***********************
* *** Second Drift Chamber
* ***********************
*  each cell -> 1 region
*  V view
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP1C1b     -5.20000 -3.600000 -10. 10. -6.350000 -5.350000
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP1C2b     -3.600000 -2.000000 -10. 10. -6.350000 -5.350000
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP1C3b     -2.000000 -0.400000 -10. 10. -6.350000 -5.350000
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP1C4b     -0.400000 1.200000 -10. 10. -6.350000 -5.350000
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP1C5b     1.200000 2.800000 -10. 10. -6.350000 -5.350000
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP1C6b     2.800000 4.400000 -10. 10. -6.350000 -5.350000
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP2C1b     -4.400000 -2.800000 -10. 10. -3.750000 -2.750000
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP2C2b     -2.800000 -1.200000 -10. 10. -3.750000 -2.750000
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP2C3b     -1.200000 0.400000 -10. 10. -3.750000 -2.750000
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP2C4b     0.400000 2.000000 -10. 10. -3.750000 -2.750000
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP2C5b     2.000000 3.6000000 -10. 10. -3.750000 -2.750000
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP2C6b     3.600000 5.2000000 -10. 10. -3.750000 -2.750000
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP3C1b     -5.200000 -3.600000 -10. 10. -1.150000 -0.150000
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP3C2b     -3.600000 -2.000000 -10. 10. -1.150000 -0.150000
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP3C3b     -2.000000 -0.400000 -10. 10. -1.150000 -0.150000
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP3C4b     -0.4000000 1.200000 -10. 10. -1.150000 -0.150000
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP3C5b     1.200000 2.800000 -10. 10. -1.150000 -0.150000
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP3C6b     2.800000 4.400000 -10. 10. -1.150000 -0.150000
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP4C1b     -4.400000 -2.800000 -10. 10. 1.450000 2.450000
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP4C2b     -2.800000 -1.200000 -10. 10. 1.450000 2.450000
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP4C3b     -1.200000 0.400000 -10. 10. 1.450000 2.450000
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP4C4b     0.400000 2.000000 -10. 10. 1.450000 2.450000
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP4C5b     2.000000 3.600000 -10. 10. 1.450000 2.450000
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP4C6b     3.600000 5.200000 -10. 10. 1.450000 2.450000
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP5C1b     -5.200000 -3.600000 -10. 10. 4.050000 5.050000
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP5C2b     -3.600000 -2. -10. 10. 4.050000 5.050000
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP5C3b     -2.000000 -0.400000 -10. 10. 4.050000 5.050000
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP5C4b     -0.400000 1.200000 -10. 10. 4.050000 5.050000
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP5C5b     1.200000 2.800000 -10. 10. 4.050000 5.050000
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP5C6b     2.800000 4.400000 -10. 10. 4.050000 5.050000
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP6C1b     -4.40000 -2.800000 -10. 10. 6.650000 7.650000
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP6C2b     -2.800000 -1.200000 -10. 10. 6.650000 7.650000
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP6C3b     -1.200000 0.400000 -10. 10. 6.650000 7.650000
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP6C4b     0.400000 2.000000 -10. 10. 6.650000 7.650000
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP6C5b     2.000000 3.600000 -10. 10. 6.650000 7.650000
!@what.3=-5.600000-fac1
!@what.4=5.600000+fac1
RPP VP6C6b     3.600000 5.200000 -10. 10. 6.650000 7.650000
*  U view
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP1C1b     -10. 10. -4.400000 -2.800000 -7.650000 -6.650000
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP1C2b     -10. 10. -2.800000 -1.200000 -7.650000 -6.650000
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP1C3b     -10. 10. -1.200000 0.400000 -7.650000 -6.650000
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP1C4b     -10. 10. 0.400000 2.000000 -7.650000 -6.650000
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP1C5b     -10. 10. 2.000000 3.600000 -7.650000 -6.650000
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP1C6b     -10. 10. 3.600000 5.200000 -7.650000 -6.650000
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP2C1b     -10. 10. -5.20000 -3.600000 -5.050000 -4.050000
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP2C2b     -10. 10. -3.600000 -2.00000 -5.050000 -4.050000
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP2C3b     -10. 10. -2.00000 -0.400000 -5.050000 -4.050000
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP2C4b     -10. 10. -0.400000 1.200000 -5.050000 -4.050000
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP2C5b     -10. 10. 1.200000 2.800000 -5.050000 -4.050000
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP2C6b     -10. 10. 2.800000 4.400000 -5.050000 -4.050000
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP3C1b     -10. 10. -4.400000 -2.800000 -2.450000 -1.450000
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP3C2b     -10. 10. -2.800000 -1.200000 -2.450000 -1.450000
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP3C3b     -10. 10. -1.200000 0.400000 -2.450000 -1.450000
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP3C4b     -10. 10. 0.400000 2.000000 -2.450000 -1.450000
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP3C5b     -10. 10. 2.000000 3.600000 -2.450000 -1.450000
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP3C6b     -10. 10. 3.600000 5.200000 -2.450000 -1.450000
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP4C1b     -10. 10. -5.200000 -3.600000 0.150000 1.150000
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP4C2b     -10. 10. -3.600000 -2.000000 0.150000 1.150000
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP4C3b     -10. 10. -2.000000 -0.400000 0.150000 1.150000
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP4C4b     -10. 10. -0.400000 1.200000 0.150000 1.150000
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP4C5b     -10. 10. 1.200000 2.800000 0.150000 1.150000
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP4C6b     -10. 10. 2.800000 4.400000 0.150000 1.150000
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP5C1b     -10. 10. -4.400000 -2.800000 2.750000 3.750000
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP5C2b     -10. 10. -2.8 -1.200000 2.750000 3.750000
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP5C3b     -10. 10. -1.200000 0.400000 2.750000 3.750000
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP5C4b     -10. 10. 0.400000 2.000000 2.750000 3.750000
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP5C5b     -10. 10. 2.000000 3.600000 2.750000 3.750000
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP5C6b     -10. 10. 3.600000 5.200000 2.750000 3.750000
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP6C1b     -10. 10. -5.200000 -3.600000 5.350000 6.350000
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP6C2b     -10. 10. -3.600000 -2.000000 5.350000 6.350000
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP6C3b     -10. 10. -2.000000 -0.400000 5.350000 6.350000
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP6C4b     -10. 10. -0.400000 1.200000 5.350000 6.350000
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP6C5b     -10. 10. 1.200000 2.800000 5.350000 6.350000
!@what.1=-5.600000-fac1
!@what.2=5.600000+fac1
RPP UP6C6b     -10. 10. 2.800000 4.400000 5.350000 6.350000
*  Drift Chamber mechanical structure
!@what.1=-5.60-fac1
!@what.2=5.60+fac1
!@what.3=-5.60-fac1
!@what.4=5.60+fac1
RPP SHIINb     -10. 10. -10. 10. -11.5 11.50
!@what.1=-6.85-fac1
!@what.2=6.85+fac1
!@what.3=-6.85-fac1
!@what.4=6.85+fac1
RPP SHIOUb     -11.25 11.25 -11.25 11.25 -11.5 11.50
* alette
!@what.3=6.85+fac1
!@what.4=13.35+fac1
RPP BARU1b     -5.00 -4.12 11.25 17.75 -7.20 7.20
!@what.3=6.85+fac1
!@what.4=13.35+fac1
RPP BARU2b     4.12 5.00 11.25 17.75 -7.20 7.20
!@what.1=-13.35-fac1
!@what.2=-6.85-fac1
RPP BARL1b     -17.75 -11.25 -5.00 -4.12 -7.20 7.20
!@what.1=-13.35-fac1
!@what.2=-6.85-fac1
RPP BARL2b     -17.75 -11.25 4.12 5.00 -7.20 7.20
!@what.1=6.85+fac1
!@what.2=9.35+fac1
RPP BARR1b     11.25 13.75 4.12 5.00 -7.20 7.20
!@what.1=6.85+fac1
!@what.2=9.35+fac1
RPP BARR2b     11.25 13.75 -5.00 -4.12 -7.20 7.20
!@what.3=-9.35-fac1
!@what.4=-6.85-fac1
RPP BARD1b     -5.00 -4.12 -13.75 -11.25 -7.20 7.20
!@what.3=-9.35-fac1
!@what.4=-6.85-fac1
RPP BARD2b     4.12 5.00 -13.75 -11.25 -7.20 7.20
* triangoli
RPP TRIA1b     -12.48 -5.00 5.00 12.48 -7.27 -7.20
RPP TRIA2b     -12.48 -5.00 5.00 12.48 7.20 7.27
PLA TRIA3b     -1.00 1.00 0.00 -5.00 12.48 7.20
* sostegno
!@what.3=-7.98-fac1
!@what.4=-6.85-fac1
RPP SOSA1b     -6.85 6.85 -12.38 -11.25 -8.34 -7.20
!@what.3=-7.98-fac1
!@what.4=-6.85-fac1
RPP SOSA2b     -6.85 6.85 -12.38 -11.25 7.20 8.34
!@what.3=-11.48-fac1
!@what.4=-7.98-fac1
RPP SOSB1b     -5.00 -3.75 -15.88 -12.38 7.20 8.34
!@what.3=-11.48-fac1
!@what.4=-7.98-fac1
RPP SOSB2b     3.75 5.00 -15.88 -12.38 7.20 8.34
!@what.3=-11.48-fac1
!@what.4=-7.98-fac1
RPP SOSB3b     -5.00 -3.75 -15.88 -12.38 -8.34 -7.20
!@what.3=-11.48-fac1
!@what.4=-7.98-fac1
RPP SOSB4b     3.75 5.00 -15.88 -12.38 -8.34 -7.20
!@what.3=-12.98-fac1
!@what.4=-11.48-fac1
RPP SOSC1b     -7.00 7.00 -17.380000000000003 -15.88 -8.70 -6.84
!@what.3=-12.98-fac1
!@what.4=-11.48-fac1
RPP SOSC2b     -7.00 7.00 -17.380000000000003 -15.88 6.84 8.70
* schede preamp U side
!@what.3=6.85+fac1
!@what.4=12.47+fac1
RPP SCHU1b     -3.25 3.25 11.25 16.87 -7.15 -7.12
!@what.3=6.85+fac1
!@what.4=12.47+fac1
RPP SCHU2b     -3.25 3.25 11.25 16.87 -4.50 -4.47
!@what.3=6.85+fac1
!@what.4=12.47+fac1
RPP SCHU3b     -3.25 3.25 11.25 16.87 -1.98 -1.95
!@what.3=6.85+fac1
!@what.4=12.47+fac1
RPP SCHU4b     -3.25 3.25 11.25 16.87 0.65 0.68
!@what.3=6.85+fac1
!@what.4=12.47+fac1
RPP SCHU5b     -3.25 3.25 11.25 16.87 3.20 3.23
!@what.3=6.85+fac1
!@what.4=12.47+fac1
RPP SCHU6b     -3.25 3.25 11.25 16.87 5.85 5.88
* schede preamp V side
!@what.1=-12.47-fac1
!@what.2=-6.85-fac1
RPP SCHV1b     -16.87 -11.25 -3.25 3.25 -5.85 -5.82
!@what.1=-12.47-fac1
!@what.2=-6.85-fac1
RPP SCHV2b     -16.87 -11.25 -3.25 3.25 -3.25 -3.22
!@what.1=-12.47-fac1
!@what.2=-6.85-fac1
RPP SCHV3b     -16.87 -11.25 -3.25 3.25 -0.65 -0.62
!@what.1=-12.47-fac1
!@what.2=-6.85-fac1
RPP SCHV4b     -16.87 -11.25 -3.25 3.25 1.95 1.98
!@what.1=-12.47-fac1
!@what.2=-6.85-fac1
RPP SCHV5b     -16.87 -11.25 -3.25 3.25 4.55 4.58
!@what.1=-12.47-fac1
!@what.2=-6.85-fac1
RPP SCHV6b     -16.87 -11.25 -3.25 3.25 7.15 7.18
$end_translat 
!@what.3=calo_dist
$start_translat 0. 0. 100
* *** Calo+scint ***
RPP Calo       -11.0 11.0 -11.0 11.0 0.0 7.5
XYP Pshi       0.5
YZP calx1      -9.
YZP calx2      -7.
YZP calx3      -5.
YZP calx4      -3.
YZP calx5      -1.
YZP calx6      1.
YZP calx7      3.
YZP calx8      5.
YZP calx9      7.
YZP calx10     9.0
XZP caly1      -9.
XZP caly2      -7.
XZP caly3      -5.
XZP caly4      -3.
XZP caly5      -1.
XZP caly6      1.
XZP caly7      3.
XZP caly8      5.
XZP caly9      7.0
XZP caly10     9.0
$end_translat 
END
* **********************************************************
* ***  REGIONS                                           ***
* **********************************************************
BLKBODY      5 +blkbody -air
AIR          5 +air -target -vtx1 -vtx2 -vtx3 -vtx4 -vtx5  -MagAlOu1 -MagAlOu2 -Calo -StartC
               -VP1C1a -VP1C2a -VP1C3a -UP1C1a -UP1C2a -UP1C3a
               -VP2C1a -VP2C2a -VP2C3a -UP2C1a -UP2C2a -UP2C3a
               -VP3C1a -VP3C2a -VP3C3a -UP3C1a -UP3C2a -UP3C3a
               -VP4C1a -VP4C2a -VP4C3a -UP4C1a -UP4C2a -UP4C3a
               -VP5C1a -VP5C2a -VP5C3a -UP5C1a -UP5C2a -UP5C3a
               -VP6C1a -VP6C2a -VP6C3a -UP6C1a -UP6C2a -UP6C3a
               -(SHIOUa -SHIINa) -BARU1a -BARU2a -BARL1a -BARL2a -BARR1a -BARR2a
               -BARD1a -BARD2a -SOSA1a -SOSA2a -SOSB1a -SOSB2a -SOSB3a -SOSB4a
               -SOSC1a -SOSC2a
               -(TRIA1a +TRIA3a -SHIOUa ) - (TRIA2a +TRIA3a -SHIOUa ) -SCHU1a
               -SCHU2a -SCHU3a -SCHU4a -SCHU5a -SCHU6a -SCHV1a-SCHV2a -SCHV3a
               -SCHV4a -SCHV5a -SCHV6a
               -VP1C1b -VP1C2b -VP1C3b -VP1C4b -VP1C5b -VP1C6b -UP1C1b -UP1C2b -UP1C3b -UP1C4b -UP1C5b -UP1C6b
               -VP2C1b -VP2C2b -VP2C3b -VP2C4b -VP2C5b -VP2C6b -UP2C1b -UP2C2b -UP2C3b -UP2C4b -UP2C5b -UP2C6b
               -VP3C1b -VP3C2b -VP3C3b -VP3C4b -VP3C5b -VP3C6b -UP3C1b -UP3C2b -UP3C3b -UP3C4b -UP3C5b -UP3C6b
               -VP4C1b -VP4C2b -VP4C3b -VP4C4b -VP4C5b -VP4C6b -UP4C1b -UP4C2b -UP4C3b -UP4C4b -UP4C5b -UP4C6b
               -VP5C1b -VP5C2b -VP5C3b -VP5C4b -VP5C5b -VP5C6b -UP5C1b -UP5C2b -UP5C3b -UP5C4b -UP5C5b -UP5C6b
               -VP6C1b -VP6C2b -VP6C3b -VP6C4b -VP6C5b -VP6C6b -UP6C1b -UP6C2b -UP6C3b -UP6C4b -UP6C5b -UP6C6b
               -(SHIOUb -SHIINb) -BARU1b -BARU2b -BARL1b -BARL2b -BARR1b -BARR2b
               -BARD1b -BARD2b -SOSA1b -SOSA2b -SOSB1b -SOSB2b -SOSB3b -SOSB4b
               -SOSC1b -SOSC2b
               -(TRIA1b +TRIA3b -SHIOUb) - (TRIA2b +TRIA3b -SHIOUb) -SCHU1b
               -SCHU2b -SCHU3b -SCHU4b -SCHU5b -SCHU6b -SCHV1b -SCHV2b -SCHV3b
               -SCHV4b -SCHV5b -SCHV6b
* *** Target ***
TARGET       5 +target
* *** Start Counter ***
STARTC       5 +StartC
* *** Vertex ***
VTX1         5 +vtx1
VTX2         5 +vtx2
VTX3         5 +vtx3
VTX4         5 +vtx4
VTX5         5 +vtx5
* *** Magnet box ***
MagPM1       5 +MagPMOu1 -MagPMIn1
MagPM2       5 +MagPMOu2 -MagPMIn2
MagCov1      5 +MagAlOu1 -(+MagPMOu1 -MagPMIn1) -Gap
MagCov2      5 +MagAlOu2 -(+MagPMOu2 -MagPMIn2) -Gap
GapMag1      5 +Gap +MagPMIn1
GapMag2      5 +Gap +MagPMIn2
MagAir1      5 +Gap +MagAlOu1 -MagPMIn1
MagAir2      5 +Gap +MagAlOu2 -MagPMIn2
* *** First Drift Chamber ***
vp1c1a       5 VP1C1a
vp1c2a       5 VP1C2a
vp1c3a       5 VP1C3a
vp2c1a       5 VP2C1a
vp2c2a       5 VP2C2a
vp2c3a       5 VP2C3a
vp3c1a       5 VP3C1a
vp3c2a       5 VP3C2a
vp3c3a       5 VP3C3a
vp4c1a       5 VP4C1a
vp4c2a       5 VP4C2a
vp4c3a       5 VP4C3a
vp5c1a       5 VP5C1a
vp5c2a       5 VP5C2a
vp5c3a       5 VP5C3a
vp6c1a       5 VP6C1a
vp6c2a       5 VP6C2a
vp6c3a       5 VP6C3a
up1c1a       5 UP1C1a
up1c2a       5 UP1C2a
up1c3a       5 UP1C3a
up2c1a       5 UP2C1a
up2c2a       5 UP2C2a
up2c3a       5 UP2C3a
up3c1a       5 UP3C1a
up3c2a       5 UP3C2a
up3c3a       5 UP3C3a
up4c1a       5 UP4C1a
up4c2a       5 UP4C2a
up4c3a       5 UP4C3a
up5c1a       5 UP5C1a
up5c2a       5 UP5C2a
up5c3a       5 UP5C3a
up6c1a       5 UP6C1a
up6c2a       5 UP6C2a
up6c3a       5 UP6C3a
SCHERMOa     5 SHIOUa-SHIINa
BARREa       5 BARU1a|BARU2a|BARL1a|BARL2a|BARR1a|BARR2a|BARD1a|BARD2a
SOSTEa       5 SOSA1a|SOSA2a|SOSB1a|SOSB2a|SOSB3a|SOSB4a|SOSC1a|SOSC2a
TRIAa        5 (TRIA1a+TRIA3a-SHIOUa)|(TRIA2a+TRIA3a-SHIOUa)
SCHEDEa      5 SCHV1a|SCHV2a|SCHV3a|SCHV4a|SCHV5a|SCHV6a|SCHU1a|SCHU2a|SCHU3a|SCHU4a|SCHU5a|SCHU6a
* *** Second Drift Chamber ***
vp1c1b       5 VP1C1b
vp1c2b       5 VP1C2b
vp1c3b       5 VP1C3b
vp1c4b       5 VP1C4b
vp1c5b       5 VP1C5b
vp1c6b       5 VP1C6b
vp2c1b       5 VP2C1b
vp2c2b       5 VP2C2b
vp2c3b       5 VP2C3b
vp2c4b       5 VP2C4b
vp2c5b       5 VP2C5b
vp2c6b       5 VP2C6b
vp3c1b       5 VP3C1b
vp3c2b       5 VP3C2b
vp3c3b       5 VP3C3b
vp3c4b       5 VP3C4b
vp3c5b       5 VP3C5b
vp3c6b       5 VP3C6b
vp4c1b       5 VP4C1b
vp4c2b       5 VP4C2b
vp4c3b       5 VP4C3b
vp4c4b       5 VP4C4b
vp4c5b       5 VP4C5b
vp4c6b       5 VP4C6b
vp5c1b       5 VP5C1b
vp5c2b       5 VP5C2b
vp5c3b       5 VP5C3b
vp5c4b       5 VP5C4b
vp5c5b       5 VP5C5b
vp5c6b       5 VP5C6b
vp6c1b       5 VP6C1b
vp6c2b       5 VP6C2b
vp6c3b       5 VP6C3b
vp6c4b       5 VP6C4b
vp6c5b       5 VP6C5b
vp6c6b       5 VP6C6b
up1c1b       5 UP1C1b
up1c2b       5 UP1C2b
up1c3b       5 UP1C3b
up1c4b       5 UP1C4b
up1c5b       5 UP1C5b
up1c6b       5 UP1C6b
up2c1b       5 UP2C1b
up2c2b       5 UP2C2b
up2c3b       5 UP2C3b
up2c4b       5 UP2C4b
up2c5b       5 UP2C5b
up2c6b       5 UP2C6b
up3c1b       5 UP3C1b
up3c2b       5 UP3C2b
up3c3b       5 UP3C3b
up3c4b       5 UP3C4b
up3c5b       5 UP3C5b
up3c6b       5 UP3C6b
up4c1b       5 UP4C1b
up4c2b       5 UP4C2b
up4c3b       5 UP4C3b
up4c4b       5 UP4C4b
up4c5b       5 UP4C5b
up4c6b       5 UP4C6b
up5c1b       5 UP5C1b
up5c2b       5 UP5C2b
up5c3b       5 UP5C3b
up5c4b       5 UP5C4b
up5c5b       5 UP5C5b
up5c6b       5 UP5C6b
up6c1b       5 UP6C1b
up6c2b       5 UP6C2b
up6c3b       5 UP6C3b
up6c4b       5 UP6C4b
up6c5b       5 UP6C5b
up6c6b       5 UP6C6b
SCHERMOb     5 SHIOUb-SHIINb
BARREb       5 BARU1b|BARU2b|BARL1b|BARL2b|BARR1b|BARR2b|BARD1b|BARD2b
SOSTEb       5 SOSA1b|SOSA2b|SOSB1b|SOSB2b|SOSB3b|SOSB4b|SOSC1b|SOSC2b
TRIAb        5 (TRIA1b+TRIA3b-SHIOUb)|(TRIA2b+TRIA3b-SHIOUb)
SCHEDEb      5 SCHV1b|SCHV2b|SCHV3b|SCHV4b|SCHV5b|SCHV6b|SCHU1b|SCHU2b|SCHU3b|SCHU4b|SCHU5b|SCHU6b
* *** Scintillator ***
SCIN0101     5 +Calo +Pshi +calx1 +caly1
SCIN0102     5 +Calo +Pshi -calx1 +calx2 +caly1
SCIN0103     5 +Calo +Pshi -calx2 +calx3 +caly1
SCIN0104     5 +Calo +Pshi -calx3 +calx4 +caly1
SCIN0105     5 +Calo +Pshi -calx4 +calx5 +caly1
SCIN0106     5 +Calo +Pshi -calx5 +calx6 +caly1
SCIN0107     5 +Calo +Pshi -calx6 +calx7 +caly1
SCIN0108     5 +Calo +Pshi -calx7 +calx8 +caly1
SCIN0109     5 +Calo +Pshi -calx8 +calx9 +caly1
SCIN0110     5 +Calo +Pshi -calx9 +calx10 +caly1
SCIN0111     5 +Calo +Pshi -calx10 +caly1
SCIN0201     5 +Calo +Pshi +calx1 -caly1 +caly2
SCIN0202     5 +Calo +Pshi -calx1 +calx2 -caly1 +caly2
SCIN0203     5 +Calo +Pshi -calx2 +calx3 -caly1 +caly2
SCIN0204     5 +Calo +Pshi -calx3 +calx4 -caly1 +caly2
SCIN0205     5 +Calo +Pshi -calx4 +calx5 -caly1 +caly2
SCIN0206     5 +Calo +Pshi -calx5 +calx6 -caly1 +caly2
SCIN0207     5 +Calo +Pshi -calx6 +calx7 -caly1 +caly2
SCIN0208     5 +Calo +Pshi -calx7 +calx8 -caly1 +caly2
SCIN0209     5 +Calo +Pshi -calx8 +calx9 -caly1 +caly2
SCIN0210     5 +Calo +Pshi -calx9 +calx10 -caly1 +caly2
SCIN0211     5 +Calo +Pshi -calx10 -caly1 +caly2
SCIN0301     5 +Calo +Pshi +calx1 -caly2 +caly3
SCIN0302     5 +Calo +Pshi -calx1 +calx2 -caly2 +caly3
SCIN0303     5 +Calo +Pshi -calx2 +calx3 -caly2 +caly3
SCIN0304     5 +Calo +Pshi -calx3 +calx4 -caly2 +caly3
SCIN0305     5 +Calo +Pshi -calx4 +calx5 -caly2 +caly3
SCIN0306     5 +Calo +Pshi -calx5 +calx6 -caly2 +caly3
SCIN0307     5 +Calo +Pshi -calx6 +calx7 -caly2 +caly3
SCIN0308     5 +Calo +Pshi -calx7 +calx8 -caly2 +caly3
SCIN0309     5 +Calo +Pshi -calx8 +calx9 -caly2 +caly3
SCIN0310     5 +Calo +Pshi -calx9 +calx10 -caly2 +caly3
SCIN0311     5 +Calo +Pshi -calx10 -caly2 +caly3
SCIN0401     5 +Calo +Pshi +calx1 -caly3 +caly4
SCIN0402     5 +Calo +Pshi -calx1 +calx2 -caly3 +caly4
SCIN0403     5 +Calo +Pshi -calx2 +calx3 -caly3 +caly4
SCIN0404     5 +Calo +Pshi -calx3 +calx4 -caly3 +caly4
SCIN0405     5 +Calo +Pshi -calx4 +calx5 -caly3 +caly4
SCIN0406     5 +Calo +Pshi -calx5 +calx6 -caly3 +caly4
SCIN0407     5 +Calo +Pshi -calx6 +calx7 -caly3 +caly4
SCIN0408     5 +Calo +Pshi -calx7 +calx8 -caly3 +caly4
SCIN0409     5 +Calo +Pshi -calx8 +calx9 -caly3 +caly4
SCIN0410     5 +Calo +Pshi -calx9 +calx10 -caly3 +caly4
SCIN0411     5 +Calo +Pshi -calx10 -caly3 +caly4
SCIN0501     5 +Calo +Pshi +calx1 -caly4 +caly5
SCIN0502     5 +Calo +Pshi -calx1 +calx2 -caly4 +caly5
SCIN0503     5 +Calo +Pshi -calx2 +calx3 -caly4 +caly5
SCIN0504     5 +Calo +Pshi -calx3 +calx4 -caly4 +caly5
SCIN0505     5 +Calo +Pshi -calx4 +calx5 -caly4 +caly5
SCIN0506     5 +Calo +Pshi -calx5 +calx6 -caly4 +caly5
SCIN0507     5 +Calo +Pshi -calx6 +calx7 -caly4 +caly5
SCIN0508     5 +Calo +Pshi -calx7 +calx8 -caly4 +caly5
SCIN0509     5 +Calo +Pshi -calx8 +calx9 -caly4 +caly5
SCIN0510     5 +Calo +Pshi -calx9 +calx10 -caly4 +caly5
SCIN0511     5 +Calo +Pshi -calx10 -caly4 +caly5
SCIN0601     5 +Calo +Pshi +calx1 -caly5 +caly6
SCIN0602     5 +Calo +Pshi -calx1 +calx2 -caly5 +caly6
SCIN0603     5 +Calo +Pshi -calx2 +calx3 -caly5 +caly6
SCIN0604     5 +Calo +Pshi -calx3 +calx4 -caly5 +caly6
SCIN0605     5 +Calo +Pshi -calx4 +calx5 -caly5 +caly6
SCIN0606     5 +Calo +Pshi -calx5 +calx6 -caly5 +caly6
SCIN0607     5 +Calo +Pshi -calx6 +calx7 -caly5 +caly6
SCIN0608     5 +Calo +Pshi -calx7 +calx8 -caly5 +caly6
SCIN0609     5 +Calo +Pshi -calx8 +calx9 -caly5 +caly6
SCIN0610     5 +Calo +Pshi -calx9 +calx10 -caly5 +caly6
SCIN0611     5 +Calo +Pshi -calx10  -caly5 +caly6
SCIN0701     5 +Calo +Pshi +calx1 -caly6 +caly7
SCIN0702     5 +Calo +Pshi -calx1 +calx2 -caly6 +caly7
SCIN0703     5 +Calo +Pshi -calx2 +calx3 -caly6 +caly7
SCIN0704     5 +Calo +Pshi -calx3 +calx4 -caly6 +caly7
SCIN0705     5 +Calo +Pshi -calx4 +calx5 -caly6 +caly7
SCIN0706     5 +Calo +Pshi -calx5 +calx6 -caly6 +caly7
SCIN0707     5 +Calo +Pshi -calx6 +calx7 -caly6 +caly7
SCIN0708     5 +Calo +Pshi -calx7 +calx8 -caly6 +caly7
SCIN0709     5 +Calo +Pshi -calx8 +calx9 -caly6 +caly7
SCIN0710     5 +Calo +Pshi -calx9 +calx10 -caly6 +caly7
SCIN0711     5 +Calo +Pshi -calx10 -caly6 +caly7
SCIN0801     5 +Calo +Pshi +calx1 -caly7 +caly8
SCIN0802     5 +Calo +Pshi -calx1 +calx2 -caly7 +caly8
SCIN0803     5 +Calo +Pshi -calx2 +calx3 -caly7 +caly8
SCIN0804     5 +Calo +Pshi -calx3 +calx4 -caly7 +caly8
SCIN0805     5 +Calo +Pshi -calx4 +calx5 -caly7 +caly8
SCIN0806     5 +Calo +Pshi -calx5 +calx6 -caly7 +caly8
SCIN0807     5 +Calo +Pshi -calx6 +calx7 -caly7 +caly8
SCIN0808     5 +Calo +Pshi -calx7 +calx8 -caly7 +caly8
SCIN0809     5 +Calo +Pshi -calx8 +calx9 -caly7 +caly8
SCIN0810     5 +Calo +Pshi -calx9 +calx10 -caly7 +caly8
SCIN0811     5 +Calo +Pshi -calx10 -caly7 +caly8
SCIN0901     5 +Calo +Pshi +calx1 -caly8 +caly9
SCIN0902     5 +Calo +Pshi -calx1 +calx2 -caly8 +caly9
SCIN0903     5 +Calo +Pshi -calx2 +calx3 -caly8 +caly9
SCIN0904     5 +Calo +Pshi -calx3 +calx4 -caly8 +caly9
SCIN0905     5 +Calo +Pshi -calx4 +calx5 -caly8 +caly9
SCIN0906     5 +Calo +Pshi -calx5 +calx6 -caly8 +caly9
SCIN0907     5 +Calo +Pshi -calx6 +calx7 -caly8 +caly9
SCIN0908     5 +Calo +Pshi -calx7 +calx8 -caly8 +caly9
SCIN0909     5 +Calo +Pshi -calx8 +calx9 -caly8 +caly9
SCIN0910     5 +Calo +Pshi -calx9 +calx10 -caly8 +caly9
SCIN0911     5 +Calo +Pshi -calx10 -caly8 +caly9
SCIN1001     5 +Calo +Pshi +calx1 -caly9 +caly10
SCIN1002     5 +Calo +Pshi -calx1 +calx2 -caly9 +caly10
SCIN1003     5 +Calo +Pshi -calx2 +calx3 -caly9 +caly10
SCIN1004     5 +Calo +Pshi -calx3 +calx4 -caly9 +caly10
SCIN1005     5 +Calo +Pshi -calx4 +calx5 -caly9 +caly10
SCIN1006     5 +Calo +Pshi -calx5 +calx6 -caly9 +caly10
SCIN1007     5 +Calo +Pshi -calx6 +calx7 -caly9 +caly10
SCIN1008     5 +Calo +Pshi -calx7 +calx8 -caly9 +caly10
SCIN1009     5 +Calo +Pshi -calx8 +calx9 -caly9 +caly10
SCIN1010     5 +Calo +Pshi -calx9 +calx10 -caly9 +caly10
SCIN1011     5 +Calo +Pshi -calx10 -caly9 +caly10
SCIN1101     5 +Calo +Pshi +calx1 -caly10
SCIN1102     5 +Calo +Pshi -calx1 +calx2 -caly10
SCIN1103     5 +Calo +Pshi -calx2 +calx3 -caly10
SCIN1104     5 +Calo +Pshi -calx3 +calx4 -caly10
SCIN1105     5 +Calo +Pshi -calx4 +calx5 -caly10
SCIN1106     5 +Calo +Pshi -calx5 +calx6 -caly10
SCIN1107     5 +Calo +Pshi -calx6 +calx7 -caly10
SCIN1108     5 +Calo +Pshi -calx7 +calx8 -caly10
SCIN1109     5 +Calo +Pshi -calx8 +calx9 -caly10
SCIN1110     5 +Calo +Pshi -calx9 +calx10 -caly10
SCIN1111     5 +Calo +Pshi -calx10 -caly10
* *** Calorimeter ***
CALO0101     5 +Calo -Pshi +calx1 +caly1
CALO0102     5 +Calo -Pshi -calx1 +calx2 +caly1
CALO0103     5 +Calo -Pshi -calx2 +calx3 +caly1
CALO0104     5 +Calo -Pshi -calx3 +calx4 +caly1
CALO0105     5 +Calo -Pshi -calx4 +calx5 +caly1
CALO0106     5 +Calo -Pshi -calx5 +calx6 +caly1
CALO0107     5 +Calo -Pshi -calx6 +calx7 +caly1
CALO0108     5 +Calo -Pshi -calx7 +calx8 +caly1
CALO0109     5 +Calo -Pshi -calx8 +calx9 +caly1
CALO0110     5 +Calo -Pshi -calx9 +calx10 +caly1
CALO0111     5 +Calo -Pshi -calx10 +caly1
CALO0201     5 +Calo -Pshi +calx1 -caly1 +caly2
CALO0202     5 +Calo -Pshi -calx1 +calx2 -caly1 +caly2
CALO0203     5 +Calo -Pshi -calx2 +calx3 -caly1 +caly2
CALO0204     5 +Calo -Pshi -calx3 +calx4 -caly1 +caly2
CALO0205     5 +Calo -Pshi -calx4 +calx5 -caly1 +caly2
CALO0206     5 +Calo -Pshi -calx5 +calx6 -caly1 +caly2
CALO0207     5 +Calo -Pshi -calx6 +calx7 -caly1 +caly2
CALO0208     5 +Calo -Pshi -calx7 +calx8 -caly1 +caly2
CALO0209     5 +Calo -Pshi -calx8 +calx9 -caly1 +caly2
CALO0210     5 +Calo -Pshi -calx9 +calx10 -caly1 +caly2
CALO0211     5 +Calo -Pshi -calx10 -caly1 +caly2
CALO0301     5 +Calo -Pshi +calx1 -caly2 +caly3
CALO0302     5 +Calo -Pshi -calx1 +calx2 -caly2 +caly3
CALO0303     5 +Calo -Pshi -calx2 +calx3 -caly2 +caly3
CALO0304     5 +Calo -Pshi -calx3 +calx4 -caly2 +caly3
CALO0305     5 +Calo -Pshi -calx4 +calx5 -caly2 +caly3
CALO0306     5 +Calo -Pshi -calx5 +calx6 -caly2 +caly3
CALO0307     5 +Calo -Pshi -calx6 +calx7 -caly2 +caly3
CALO0308     5 +Calo -Pshi -calx7 +calx8 -caly2 +caly3
CALO0309     5 +Calo -Pshi -calx8 +calx9 -caly2 +caly3
CALO0310     5 +Calo -Pshi -calx9 +calx10 -caly2 +caly3
CALO0311     5 +Calo -Pshi -calx10 -caly2 +caly3
CALO0401     5 +Calo -Pshi +calx1 -caly3 +caly4
CALO0402     5 +Calo -Pshi -calx1 +calx2 -caly3 +caly4
CALO0403     5 +Calo -Pshi -calx2 +calx3 -caly3 +caly4
CALO0404     5 +Calo -Pshi -calx3 +calx4 -caly3 +caly4
CALO0405     5 +Calo -Pshi -calx4 +calx5 -caly3 +caly4
CALO0406     5 +Calo -Pshi -calx5 +calx6 -caly3 +caly4
CALO0407     5 +Calo -Pshi -calx6 +calx7 -caly3 +caly4
CALO0408     5 +Calo -Pshi -calx7 +calx8 -caly3 +caly4
CALO0409     5 +Calo -Pshi -calx8 +calx9 -caly3 +caly4
CALO0410     5 +Calo -Pshi -calx9 +calx10 -caly3 +caly4
CALO0411     5 +Calo -Pshi -calx10 -caly3 +caly4
CALO0501     5 +Calo -Pshi +calx1 -caly4 +caly5
CALO0502     5 +Calo -Pshi -calx1 +calx2 -caly4 +caly5
CALO0503     5 +Calo -Pshi -calx2 +calx3 -caly4 +caly5
CALO0504     5 +Calo -Pshi -calx3 +calx4 -caly4 +caly5
CALO0505     5 +Calo -Pshi -calx4 +calx5 -caly4 +caly5
CALO0506     5 +Calo -Pshi -calx5 +calx6 -caly4 +caly5
CALO0507     5 +Calo -Pshi -calx6 +calx7 -caly4 +caly5
CALO0508     5 +Calo -Pshi -calx7 +calx8 -caly4 +caly5
CALO0509     5 +Calo -Pshi -calx8 +calx9 -caly4 +caly5
CALO0510     5 +Calo -Pshi -calx9 +calx10 -caly4 +caly5
CALO0511     5 +Calo -Pshi -calx10 -caly4 +caly5
CALO0601     5 +Calo -Pshi +calx1 -caly5 +caly6
CALO0602     5 +Calo -Pshi -calx1 +calx2 -caly5 +caly6
CALO0603     5 +Calo -Pshi -calx2 +calx3 -caly5 +caly6
CALO0604     5 +Calo -Pshi -calx3 +calx4 -caly5 +caly6
CALO0605     5 +Calo -Pshi -calx4 +calx5 -caly5 +caly6
CALO0606     5 +Calo -Pshi -calx5 +calx6 -caly5 +caly6
CALO0607     5 +Calo -Pshi -calx6 +calx7 -caly5 +caly6
CALO0608     5 +Calo -Pshi -calx7 +calx8 -caly5 +caly6
CALO0609     5 +Calo -Pshi -calx8 +calx9 -caly5 +caly6
CALO0610     5 +Calo -Pshi -calx9 +calx10 -caly5 +caly6
CALO0611     5 +Calo -Pshi -calx10 -caly5 +caly6
CALO0701     5 +Calo -Pshi +calx1 -caly6 +caly7
CALO0702     5 +Calo -Pshi -calx1 +calx2 -caly6 +caly7
CALO0703     5 +Calo -Pshi -calx2 +calx3 -caly6 +caly7
CALO0704     5 +Calo -Pshi -calx3 +calx4 -caly6 +caly7
CALO0705     5 +Calo -Pshi -calx4 +calx5 -caly6 +caly7
CALO0706     5 +Calo -Pshi -calx5 +calx6 -caly6 +caly7
CALO0707     5 +Calo -Pshi -calx6 +calx7 -caly6 +caly7
CALO0708     5 +Calo -Pshi -calx7 +calx8 -caly6 +caly7
CALO0709     5 +Calo -Pshi -calx8 +calx9 -caly6 +caly7
CALO0710     5 +Calo -Pshi -calx9 +calx10 -caly6 +caly7
CALO0711     5 +Calo -Pshi -calx10 -caly6 +caly7
CALO0801     5 +Calo -Pshi +calx1 -caly7 +caly8
CALO0802     5 +Calo -Pshi -calx1 +calx2 -caly7 +caly8
CALO0803     5 +Calo -Pshi -calx2 +calx3 -caly7 +caly8
CALO0804     5 +Calo -Pshi -calx3 +calx4 -caly7 +caly8
CALO0805     5 +Calo -Pshi -calx4 +calx5 -caly7 +caly8
CALO0806     5 +Calo -Pshi -calx5 +calx6 -caly7 +caly8
CALO0807     5 +Calo -Pshi -calx6 +calx7 -caly7 +caly8
CALO0808     5 +Calo -Pshi -calx7 +calx8 -caly7 +caly8
CALO0809     5 +Calo -Pshi -calx8 +calx9 -caly7 +caly8
CALO0810     5 +Calo -Pshi -calx9 +calx10 -caly7 +caly8
CALO0811     5 +Calo -Pshi -calx10 -caly7 +caly8
CALO0901     5 +Calo -Pshi +calx1 -caly8 +caly9
CALO0902     5 +Calo -Pshi -calx1 +calx2 -caly8 +caly9
CALO0903     5 +Calo -Pshi -calx2 +calx3 -caly8 +caly9
CALO0904     5 +Calo -Pshi -calx3 +calx4 -caly8 +caly9
CALO0905     5 +Calo -Pshi -calx4 +calx5 -caly8 +caly9
CALO0906     5 +Calo -Pshi -calx5 +calx6 -caly8 +caly9
CALO0907     5 +Calo -Pshi -calx6 +calx7 -caly8 +caly9
CALO0908     5 +Calo -Pshi -calx7 +calx8 -caly8 +caly9
CALO0909     5 +Calo -Pshi -calx8 +calx9 -caly8 +caly9
CALO0910     5 +Calo -Pshi -calx9 +calx10 -caly8 +caly9
CALO0911     5 +Calo -Pshi -calx10 -caly8 +caly9
CALO1001     5 +Calo -Pshi +calx1 -caly9 +caly10
CALO1002     5 +Calo -Pshi -calx1 +calx2 -caly9 +caly10
CALO1003     5 +Calo -Pshi -calx2 +calx3 -caly9 +caly10
CALO1004     5 +Calo -Pshi -calx3 +calx4 -caly9 +caly10
CALO1005     5 +Calo -Pshi -calx4 +calx5 -caly9 +caly10
CALO1006     5 +Calo -Pshi -calx5 +calx6 -caly9 +caly10
CALO1007     5 +Calo -Pshi -calx6 +calx7 -caly9 +caly10
CALO1008     5 +Calo -Pshi -calx7 +calx8 -caly9 +caly10
CALO1009     5 +Calo -Pshi -calx8 +calx9 -caly9 +caly10
CALO1010     5 +Calo -Pshi -calx9 +calx10 -caly9 +caly10
CALO1011     5 +Calo -Pshi -calx10 -caly9 +caly10
CALO1101     5 +Calo -Pshi +calx1 -caly10
CALO1102     5 +Calo -Pshi -calx1 +calx2 -caly10
CALO1103     5 +Calo -Pshi -calx2 +calx3 -caly10
CALO1104     5 +Calo -Pshi -calx3 +calx4 -caly10
CALO1105     5 +Calo -Pshi -calx4 +calx5 -caly10
CALO1106     5 +Calo -Pshi -calx5 +calx6 -caly10
CALO1107     5 +Calo -Pshi -calx6 +calx7 -caly10
CALO1108     5 +Calo -Pshi -calx7 +calx8 -caly10
CALO1109     5 +Calo -Pshi -calx8 +calx9 -caly10
CALO1110     5 +Calo -Pshi -calx9 +calx10 -caly10
CALO1111     5 +Calo -Pshi -calx10 -caly10
END
