set term post eps enhanced color "Helvetica" 32 size 9in,7in

#set key off

set grid


set ylabel "Mass attenuation coefficient\n({/Symbol m}/{/Symbol r}) in cm^2/g"
set xlabel "Energy in keV"

set xrange [60:120]
set yrange [0.16:0.21]

#set log y

set output "test_water_zoom.eps"
plot    "water.csv"                                  using ($1 * 1000):2 t "Reference"                w l lc rgb "#222222" lt -1 lw 4, \
	"test_mass_attenuation_coefficients.csv"     using ($1 * 1000):2 t "XCOM"                     w l lc rgb "#E92222" lt -1 lw 2, \
	"test_mass_attenuation_coefficients.csv"     using ($1 * 1000):3 t "Log-linear interpolation" w l lc rgb "#2222E9" lt -1 lw 2

set output "test_soft_tissue_zoom.eps"
plot    "tissue.csv"                                 using ($1 * 1000):2 t "Reference"                w l lc rgb "#222222" lt -1 lw 4, \
	"test_mass_attenuation_coefficients.csv"     using ($1 * 1000):4 t "XCOM"                     w l lc rgb "#E92222" lt -1 lw 2, \
	"test_mass_attenuation_coefficients.csv"     using ($1 * 1000):5 t "Log-linear interpolation" w l lc rgb "#2222E9" lt -1 lw 2

set output "test_bone_zoom.eps"
set yrange [0.16:0.32]
plot    "bone.csv"                                   using ($1 * 1000):2 t "Reference"                w l lc rgb "#222222" lt -1 lw 4, \
	"test_mass_attenuation_coefficients.csv"     using ($1 * 1000):6 t "XCOM"                     w l lc rgb "#E92222" lt -1 lw 2, \
	"test_mass_attenuation_coefficients.csv"     using ($1 * 1000):7 t "Log-linear interpolation" w l lc rgb "#2222E9" lt -1 lw 2









set ylabel "Mass attenuation coefficient\n({/Symbol m}/{/Symbol r}) in cm^2/g"
set xlabel "Energy in MeV"

set xrange [1.00E-03:1.00E+02]
set yrange [1.00E-02:1.00E+04]

set log xy

set format xy "10^{%T}"

set output "test_water.eps"
plot    "water.csv"                                  using 1:2 t "Reference"                w l lc rgb "#222222" lt -1 lw 4, \
	"test_mass_attenuation_coefficients.csv"     using 1:2 t "XCOM"                     w l lc rgb "#E92222" lt -1 lw 2, \
	"test_mass_attenuation_coefficients.csv"     using 1:3 t "Log-linear interpolation" w l lc rgb "#2222E9" lt -1 lw 2

set output "test_soft_tissue.eps"
plot    "tissue.csv"                                 using 1:2 t "Reference"                w l lc rgb "#222222" lt -1 lw 4, \
	"test_mass_attenuation_coefficients.csv"     using 1:4 t "XCOM"                     w l lc rgb "#E92222" lt -1 lw 2, \
	"test_mass_attenuation_coefficients.csv"     using 1:5 t "Log-linear interpolation" w l lc rgb "#2222E9" lt -1 lw 2

set output "test_bone.eps"
plot    "bone.csv"                                   using 1:2 t "Reference"                w l lc rgb "#222222" lt -1 lw 4, \
	"test_mass_attenuation_coefficients.csv"     using 1:6 t "XCOM"                     w l lc rgb "#E92222" lt -1 lw 2, \
	"test_mass_attenuation_coefficients.csv"     using 1:7 t "Log-linear interpolation" w l lc rgb "#2222E9" lt -1 lw 2







exit

















set xlabel "Energy in MeV"


set title "Test the value of {/Symbol m}_{water} using two interpolation methods"

set output "test_mu_water.eps"
set xrange [1e-3:20]
set yrange [1e-2:1e4]

plot    "water.csv"                                  using 1:2 t "Reference"                w l lc rgb "#222222" lt -1 lw 4, \
	"test_mu_water_linear_interpolation.csv"     using 1:2 t "XCOM"                     w l lc rgb "#E92222" lt -1 lw 2, \
	"test_mu_water_linear_interpolation.csv"     using 1:3 t "Linear interpolation"     w l lc rgb "#22E922" lt -1 lw 2, \
	"test_mu_water_log_linear_interpolation.csv" using 1:3 t "Log-linear interpolation" w l lc rgb "#2222E9" lt -1 lw 2

set output "test_mu_water_zoom.eps"
set xrange [60:120]
set yrange [1e-1:1e1]

plot    "water.csv"                                  using ($1:2 t "Reference"                w l lc rgb "#222222" lt -1 lw 4, \
	"test_mu_water_linear_interpolation.csv"     using ($1:2 t "XCOM"                     w l lc rgb "#E92222" lt -1 lw 2, \
	"test_mu_water_linear_interpolation.csv"     using ($1:3 t "Linear interpolation"     w l lc rgb "#22E922" lt -1 lw 2, \
	"test_mu_water_log_linear_interpolation.csv" using ($1:3 t "Log-linear interpolation" w l lc rgb "#2222E9" lt -1 lw 2


set title "Test the value of {/Symbol m}_{soft tissue} using two interpolation methods"
set output "test_mu_soft_tissue.eps"
set xrange [1e-3:20]
set yrange [1e-2:1e4]

plot    "tissue.csv"                                 using 1:2 t "Reference"                w l lc rgb "#222222" lt -1 lw 4, \
	"test_mu_water_linear_interpolation.csv"     using 1:4 t "XCOM"                     w l lc rgb "#E92222" lt -1 lw 2, \
	"test_mu_water_linear_interpolation.csv"     using 1:5 t "Linear interpolation"     w l lc rgb "#22E922" lt -1 lw 2, \
	"test_mu_water_log_linear_interpolation.csv" using 1:5 t "Log-linear interpolation" w l lc rgb "#2222E9" lt -1 lw 2

set output "test_mu_soft_tissue_zoom.eps"
set xrange [0.01:0.1]
set yrange [1e-1:1e1]

replot

set title "Test the value of {/Symbol m}_{bone} using two interpolation methods"

set output "test_mu_bone.eps"
set xrange [1e-3:20]
set yrange [1e-2:1e4]

plot    "bone.csv"                                   using 1:2 t "Reference"                w l lc rgb "#222222" lt -1 lw 4, \
	"test_mu_water_linear_interpolation.csv"     using 1:6 t "XCOM"                     w l lc rgb "#E92222" lt -1 lw 2, \
	"test_mu_water_linear_interpolation.csv"     using 1:7 t "Linear interpolation"     w l lc rgb "#22E922" lt -1 lw 2, \
	"test_mu_water_log_linear_interpolation.csv" using 1:7 t "Log-linear interpolation" w l lc rgb "#2222E9" lt -1 lw 2

set output "test_mu_bone_zoom.eps"
set xrange [0.01:0.1]
set yrange [1e-1:1e1]

replot

set output "test_mu.eps"
set xrange [1e-3:20]
set yrange [1e-2:1e4]

plot    "water.csv"                                  using 1:2 t "Reference (Water)"       ps 3 lw 3, \
        "tissue.csv"                                 using 1:2 t "Reference (Soft tissue)" ps 3 lw 3, \
        "bone.csv"                                   using 1:2 t "Reference (Bone)"        ps 3 lw 3, \
	"test_mu_water_log_linear_interpolation.csv" using 1:2 t "Water"       w l lc rgb "#E92222" lt -1 lw 3,\
	"test_mu_water_log_linear_interpolation.csv" using 1:3 t "Soft tissue" w l lc rgb "#22E922" lt -1 lw 3,\
	"test_mu_water_log_linear_interpolation.csv" using 1:4 t "Bone"        w l lc rgb "#2222E9" lt -1 lw 3,\

set output "test_mu_zoom.eps"
set xrange [0.01:0.1]
set yrange [1e-1:1e1]

replot


reset