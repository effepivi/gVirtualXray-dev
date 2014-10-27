#!/usr/bin/gnuplot

set term post eps enhanced color "Helvetica" 32 size 9in,7in

set grid


set ylabel "Mass attenuation coefficient\n({/Symbol m}/{/Symbol r}) in cm^2/g"
set xlabel "Energy in keV"

set xrange [60:120]

set output "test_water_zoom.eps"
plot    "water.csv"                              using ($1*1000):2 t "Reference"                w l lc rgb "#222222" lt -1 lw 4, \
	"test_mass_attenuation_coefficients.csv"     using ($1*1000):2 t "Computed using XCOM database"                     w l lc rgb "#E92222" lt -1 lw 2, \
	"test_mass_attenuation_coefficients.csv"     using ($1*1000):3 t "Computed using {/Symbol m}={/Symbol m}_{water}(1+HU_{water}/1000)" w l lc rgb "#2222E9" lt -1 lw 2

set output "test_brain_zoom.eps"
plot    "brain.csv"                              using ($1*1000):2 t "Reference"                w l lc rgb "#222222" lt -1 lw 4, \
	"test_mass_attenuation_coefficients.csv"     using ($1*1000):6 t "Computed using XCOM database"                     w l lc rgb "#E92222" lt -1 lw 2, \
	"test_mass_attenuation_coefficients.csv"     using ($1*1000):7 t "Computed using {/Symbol m}={/Symbol m}_{water}(1+HU_{brain}/1000)" w l lc rgb "#2222E9" lt -1 lw 2

set output "test_gas_zoom.eps"
plot    "gas.csv"                                using ($1*1000):2 t "Reference"                w l lc rgb "#222222" lt -1 lw 4, \
	"test_mass_attenuation_coefficients.csv"     using ($1*1000):8 t "Computed using XCOM database"                     w l lc rgb "#E92222" lt -1 lw 2, \
	"test_mass_attenuation_coefficients.csv"     using ($1*1000):9 t "Computed using {/Symbol m}={/Symbol m}_{water}(1+HU_{gas}/1000)" w l lc rgb "#2222E9" lt -1 lw 2

set output "test_soft_tissue_zoom.eps"
plot    "tissue.csv"                             using ($1*1000):2 t "Reference"                w l lc rgb "#222222" lt -1 lw 4, \
	"test_mass_attenuation_coefficients.csv"     using ($1*1000):10 t "Computed using XCOM database"                     w l lc rgb "#E92222" lt -1 lw 2, \
	"test_mass_attenuation_coefficients.csv"     using ($1*1000):11 t "Computed using {/Symbol m}={/Symbol m}_{water}(1+HU_{soft tissue}/1000)" w l lc rgb "#2222E9" lt -1 lw 2

set yrange [0.16:0.32]
set output "test_bone_zoom.eps"
plot    "bone.csv"                               using ($1*1000):2 t "Reference"                w l lc rgb "#222222" lt -1 lw 4, \
	"test_mass_attenuation_coefficients.csv"     using ($1*1000):4 t "Computed using XCOM database"                     w l lc rgb "#E92222" lt -1 lw 2, \
	"test_mass_attenuation_coefficients.csv"     using ($1*1000):5 t "Computed using {/Symbol m}={/Symbol m}_{water}(1+HU_{bone}/1000)" w l lc rgb "#2222E9" lt -1 lw 2



set ylabel "Mass attenuation coefficient\n({/Symbol m}/{/Symbol r}) in cm^2/g"
set xlabel "Energy in MeV"

set xrange [1.00E-03:1.00E+02]
set yrange [1.00E-02:1.00E+04]

set log xy

set format xy "10^{%T}"

set output "test_water.eps"
plot    "water.csv"                              using 1:2 t "Reference"                w l lc rgb "#222222" lt -1 lw 4, \
	"test_mass_attenuation_coefficients.csv"     using 1:2 t "Computed using XCOM database"                     w l lc rgb "#E92222" lt -1 lw 2, \
	"test_mass_attenuation_coefficients.csv"     using 1:3 t "Computed using {/Symbol m}_{water}={/Symbol m}_{water}(1+HU_{water}/1000)" w l lc rgb "#2222E9" lt -1 lw 2

set output "test_bone.eps"
plot    "bone.csv"                               using 1:2 t "Reference"                w l lc rgb "#222222" lt -1 lw 4, \
	"test_mass_attenuation_coefficients.csv"     using 1:4 t "Computed using XCOM database"                     w l lc rgb "#E92222" lt -1 lw 2, \
	"test_mass_attenuation_coefficients.csv"     using 1:5 t "Computed using {/Symbol m}_{bone}={/Symbol m}_{water}(1+HU_{bone}/1000)" w l lc rgb "#2222E9" lt -1 lw 2

set output "test_brain.eps"
plot    "brain.csv"                              using 1:2 t "Reference"                w l lc rgb "#222222" lt -1 lw 4, \
	"test_mass_attenuation_coefficients.csv"     using 1:6 t "Computed using XCOM database"                     w l lc rgb "#E92222" lt -1 lw 2, \
	"test_mass_attenuation_coefficients.csv"     using 1:7 t "Computed using {/Symbol m}_{brain}={/Symbol m}_{water}(1+HU_{brain}/1000)" w l lc rgb "#2222E9" lt -1 lw 2

set output "test_gas.eps"
plot    "gas.csv"                                using 1:2 t "Reference"                w l lc rgb "#222222" lt -1 lw 4, \
	"test_mass_attenuation_coefficients.csv"     using 1:8 t "Computed using XCOM database"                     w l lc rgb "#E92222" lt -1 lw 2, \
	"test_mass_attenuation_coefficients.csv"     using 1:9 t "Computed using {/Symbol m}_{gas}={/Symbol m}_{water}(1+HU_{gas}/1000)" w l lc rgb "#2222E9" lt -1 lw 2

set output "test_soft_tissue.eps"
plot    "tissue.csv"                             using 1:2 t "Reference"                w l lc rgb "#222222" lt -1 lw 4, \
	"test_mass_attenuation_coefficients.csv"     using 1:10 t "Computed using XCOM database"                     w l lc rgb "#E92222" lt -1 lw 2, \
	"test_mass_attenuation_coefficients.csv"     using 1:11 t "Computed using {/Symbol m}_{soft tissue}={/Symbol m}_{water}(1+HU_{soft tissue}/1000)" w l lc rgb "#2222E9" lt -1 lw 2

