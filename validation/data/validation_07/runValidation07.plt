#!/usr/bin/gnuplot

set term post eps enhanced color "Helvetica" 32 size 9in,7in

set grid


set ylabel "Normalised Energy"
set xlabel "Pixel Index"

set key top center box

set output "test_profile.eps"
plot \
	"gate_profile.txt" t "Gate"         w p lw 2, \
	"gpu_profile.txt"  t "gVirtualXRay" w l lc rgb "#2222E9" lt -1 lw 2


set output "test_point_vs_cube.eps"
plot \
	"../validation_06/gate_profile.txt" t "Point source (Gate)"         w p lc rgb "#2222E9" lw 2, \
	"../validation_06/gpu_profile.txt"  t "Point source (gVirtualXRay)" w l lc rgb "#2222E9" lt -1 lw 2, \
	"gate_profile.txt"                  t "Cubic source (Gate)"         w p lc rgb "#E92222" lw 2, \
	"gpu_profile.txt"                   t "Cubic source (gVirtualXRay)" w l lc rgb "#E92222" lt -1 lw 2

