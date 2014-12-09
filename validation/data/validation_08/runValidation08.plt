#!/usr/bin/gnuplot

set term post eps enhanced color "Helvetica" 32 size 9in,7in

set grid


set ylabel "Normalised Energy"
set xlabel "Pixel Index"

set output "test_profile.eps"
plot \
	"reference_sinogram_profile.txt" t "Reference"    w l lc rgb "#E92222" lt -1 lw 2, \
	"test_sinogram_profile.txt"      t "gVirtualXRay" w l lc rgb "#2222E9" lt -1 lw 2

