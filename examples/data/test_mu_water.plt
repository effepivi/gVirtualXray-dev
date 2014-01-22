set term post eps enhanced color "Helvetica" 32 size 7in,4.5in

#set key off

set title "Test the value of {/Symbol m}_{water} using two interpolation methods"
set grid

set log xy

set xlabel "Energy in MeV"
set ylabel "Mass attenuation coefficient\n({/Symbol m}) of water in cm^2/g"

set output "test_mu_water.eps"

plot    "test_mu_water1.csv"                         using 1:3 t "Reference", \
	"test_mu_water_linear_interpolation.csv"     using 1:3 t "Linear interpolation"     w l lc rgb "#22E922" lt -1, \
	"test_mu_water_log_linear_interpolation.csv" using 1:3 t "Log-linear interpolation" w l lc rgb "#2222E9" lt -1

set output "test_mu_water_zoom.eps"

set xrange [0.01:0.1]

replot

reset