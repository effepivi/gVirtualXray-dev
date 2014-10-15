#!/bin/bash

gnuplot < test_mu_water.plt

convert test_bone.eps        test_bone.png
convert test_soft_tissue.eps test_soft_tissue.png
convert test_water.eps       test_water.png
#convert test_mu.eps test_mu.png

convert test_bone_zoom.eps        test_bone_zoom.png
convert test_soft_tissue_zoom.eps test_soft_tissue_zoom.png
convert test_water_zoom.eps       test_water_zoom.png
#convert test_mu_zoom.eps test_mu_zoom.png




