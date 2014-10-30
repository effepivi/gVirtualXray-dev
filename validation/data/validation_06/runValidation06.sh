#!/bin/bash

gnuplot < runValidation06.plt

fig2eps --nogv scene.fig

convert test_profile.eps     test_profile.png
convert scene.eps            scene.png

