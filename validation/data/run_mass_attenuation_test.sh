#!/bin/bash

gnuplot < plot_mass_attenuation_test.plt

convert test_bone.eps        test_bone.png
convert test_brain.eps       test_brain.png
convert test_gas.eps         test_gas.png
convert test_soft_tissue.eps test_soft_tissue.png
convert test_water.eps       test_water.png

convert test_bone_zoom.eps        test_bone_zoom.png
convert test_brain_zoom.eps       test_brain_zoom.png
convert test_gas_zoom.eps         test_gas_zoom.png
convert test_soft_tissue_zoom.eps test_soft_tissue_zoom.png
convert test_water_zoom.eps       test_water_zoom.png

