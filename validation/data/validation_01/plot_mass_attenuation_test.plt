#!/usr/bin/gnuplot

set term post eps enhanced color "Helvetica" 32 size 9in,7in

set grid

#set key bottom 

set log xy
set format xy "10^{%T}"

set ylabel "Mass attenuation coefficient ({/Symbol m}/{/Symbol r}) in cm^2/g"
set xlabel "Energy in keV"





set xrange [10E-3:10E-2]

set output "test_water_zoom.eps"
plot    "water.csv"                              using 1:2 t "Reference"                w l lc rgb "#222222" lt -1 lw 5, \
	"test_mass_attenuation_coefficients.csv"     using 1:2 t "Computed using: XCOM database"                     w l lc rgb "#E92222" lt -1 lw 4, \
	"test_mass_attenuation_coefficients.csv"     using 1:3 t "{/Symbol m}={/Symbol m}_{water}(1+HU_{water}/1000) (interpolation in linear scale)" w l lc rgb "#2222E9" lt -1 lw 3, \
	"test_mass_attenuation_coefficients.csv"     using 1:4 t "{/Symbol m}={/Symbol m}_{water}(1+HU_{water}/1000) (interpolation in log scale)" w l lc rgb "#FF00FF" lt -1 lw 2

set output "test_bone_zoom.eps"
plot    "bone.csv"                               using 1:2 t "Reference"                w l lc rgb "#222222" lt -1 lw 5, \
	"test_mass_attenuation_coefficients.csv"     using 1:5 t "Computed using: XCOM database"                     w l lc rgb "#E92222" lt -1 lw 4, \
	"test_mass_attenuation_coefficients.csv"     using 1:6 t "{/Symbol m}={/Symbol m}_{water}(1+HU_{bone}/1000) (interpolation in linear scale)" w l lc rgb "#2222E9" lt -1 lw 3, \
	"test_mass_attenuation_coefficients.csv"     using 1:7 t "{/Symbol m}={/Symbol m}_{water}(1+HU_{bone}/1000) (interpolation in log scale)" w l lc rgb "#FF00FF" lt -1 lw 2

set output "test_brain_zoom.eps"
plot    "brain.csv"                              using 1:2  t "Reference"                w l lc rgb "#222222" lt -1 lw 5, \
	"test_mass_attenuation_coefficients.csv"     using 1:8  t "Computed using: XCOM database"                     w l lc rgb "#E92222" lt -1 lw 4, \
	"test_mass_attenuation_coefficients.csv"     using 1:9  t "{/Symbol m}={/Symbol m}_{water}(1+HU_{brain}/1000) (interpolation in linear scale)" w l lc rgb "#2222E9" lt -1 lw 3, \
	"test_mass_attenuation_coefficients.csv"     using 1:10 t "{/Symbol m}={/Symbol m}_{water}(1+HU_{brain}/1000) (interpolation in log scale)" w l lc rgb "#FF00FF" lt -1 lw 2

set output "test_gas_zoom.eps"
plot    "gas.csv"                                using 1:2  t "Reference"                w l lc rgb "#222222" lt -1 lw 5, \
	"test_mass_attenuation_coefficients.csv"     using 1:11 t "Computed using: XCOM database"                     w l lc rgb "#E92222" lt -1 lw 4, \
	"test_mass_attenuation_coefficients.csv"     using 1:12 t "{/Symbol m}={/Symbol m}_{water}(1+HU_{gas}/1000) (interpolation in linear scale)" w l lc rgb "#2222E9" lt -1 lw 3, \
	"test_mass_attenuation_coefficients.csv"     using 1:13 t "{/Symbol m}={/Symbol m}_{water}(1+HU_{gas}/1000) (interpolation in log scale)" w l lc rgb "#FF00FF" lt -1 lw 2

set output "test_soft_tissue_zoom.eps"
plot    "tissue.csv"                             using 1:2  t "Reference"                w l lc rgb "#222222" lt -1 lw 5, \
	"test_mass_attenuation_coefficients.csv"     using 1:14 t "Computed using: XCOM database"                     w l lc rgb "#E92222" lt -1 lw 4, \
	"test_mass_attenuation_coefficients.csv"     using 1:15 t "{/Symbol m}={/Symbol m}_{water}(1+HU_{soft tissue}/1000) (interpolation in linear scale)" w l lc rgb "#2222E9" lt -1 lw 3, \
	"test_mass_attenuation_coefficients.csv"     using 1:16 t "{/Symbol m}={/Symbol m}_{water}(1+HU_{soft tissue}/1000) (interpolation in log scale)" w l lc rgb "#FF00FF" lt -1 lw 2








set xrange [1.00E-03:1.00E+02]
set yrange [1.00E-02:1.00E+04]

set output "test_water.eps"
plot    "water.csv"                              using 1:2 t "Reference"                w l lc rgb "#222222" lt -1 lw 5, \
	"test_mass_attenuation_coefficients.csv"     using 1:2 t "Computed using: XCOM database"                     w l lc rgb "#E92222" lt -1 lw 4, \
	"test_mass_attenuation_coefficients.csv"     using 1:3 t "{/Symbol m}={/Symbol m}_{water}(1+HU_{water}/1000) (interpolation in linear scale)" w l lc rgb "#2222E9" lt -1 lw 3, \
	"test_mass_attenuation_coefficients.csv"     using 1:4 t "{/Symbol m}={/Symbol m}_{water}(1+HU_{water}/1000) (interpolation in log scale)" w l lc rgb "#FF00FF" lt -1 lw 2

set output "test_bone.eps"
plot    "bone.csv"                               using 1:2 t "Reference"                w l lc rgb "#222222" lt -1 lw 5, \
	"test_mass_attenuation_coefficients.csv"     using 1:5 t "Computed using: XCOM database"                     w l lc rgb "#E92222" lt -1 lw 4, \
	"test_mass_attenuation_coefficients.csv"     using 1:6 t "{/Symbol m}={/Symbol m}_{water}(1+HU_{bone}/1000) (interpolation in linear scale)" w l lc rgb "#2222E9" lt -1 lw 3, \
	"test_mass_attenuation_coefficients.csv"     using 1:7 t "{/Symbol m}={/Symbol m}_{water}(1+HU_{bone}/1000) (interpolation in log scale)" w l lc rgb "#FF00FF" lt -1 lw 2

set output "test_brain.eps"
plot    "brain.csv"                              using 1:2  t "Reference"                w l lc rgb "#222222" lt -1 lw 5, \
	"test_mass_attenuation_coefficients.csv"     using 1:8  t "Computed using: XCOM database"                     w l lc rgb "#E92222" lt -1 lw 4, \
	"test_mass_attenuation_coefficients.csv"     using 1:9  t "{/Symbol m}={/Symbol m}_{water}(1+HU_{brain}/1000) (interpolation in linear scale)" w l lc rgb "#2222E9" lt -1 lw 3, \
	"test_mass_attenuation_coefficients.csv"     using 1:10 t "{/Symbol m}={/Symbol m}_{water}(1+HU_{brain}/1000) (interpolation in log scale)" w l lc rgb "#FF00FF" lt -1 lw 2

set output "test_gas.eps"
plot    "gas.csv"                                using 1:2  t "Reference"                w l lc rgb "#222222" lt -1 lw 5, \
	"test_mass_attenuation_coefficients.csv"     using 1:11 t "Computed using: XCOM database"                     w l lc rgb "#E92222" lt -1 lw 4, \
	"test_mass_attenuation_coefficients.csv"     using 1:12 t "{/Symbol m}={/Symbol m}_{water}(1+HU_{gas}/1000) (interpolation in linear scale)" w l lc rgb "#2222E9" lt -1 lw 3, \
	"test_mass_attenuation_coefficients.csv"     using 1:13 t "{/Symbol m}={/Symbol m}_{water}(1+HU_{gas}/1000) (interpolation in log scale)" w l lc rgb "#FF00FF" lt -1 lw 2

set output "test_soft_tissue.eps"
plot    "tissue.csv"                             using 1:2  t "Reference"                w l lc rgb "#222222" lt -1 lw 5, \
	"test_mass_attenuation_coefficients.csv"     using 1:14 t "Computed using: XCOM database"                     w l lc rgb "#E92222" lt -1 lw 4, \
	"test_mass_attenuation_coefficients.csv"     using 1:15 t "{/Symbol m}={/Symbol m}_{water}(1+HU_{soft tissue}/1000) (interpolation in linear scale)" w l lc rgb "#2222E9" lt -1 lw 3, \
	"test_mass_attenuation_coefficients.csv"     using 1:16 t "{/Symbol m}={/Symbol m}_{water}(1+HU_{soft tissue}/1000) (interpolation in log scale)" w l lc rgb "#FF00FF" lt -1 lw 2

