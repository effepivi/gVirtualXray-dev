#!/usr/bin/gnuplot

set term post eps enhanced color "Helvetica" 32 size 9in,7in

set grid


set ylabel "Density\n({/Symbol r}) in g/cm^3"
set xlabel "Hounsfield Unit"

set xrange[-1000:1600]
set yrange[0:2]

set xtics 200
set ytics 0.2

set output "test_density.eps"
plot    "test_density.csv" using 1:2 notitle w l lc rgb "#222222" lt -1 lw 4

