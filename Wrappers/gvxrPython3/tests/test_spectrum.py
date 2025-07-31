#!/usr/bin/env python3
from __future__ import annotations

import unittest

import numpy as np
import spekpy as sp
from gvxrPython3 import gvxr  # Simulate X-ray images


def nrmse(ground_truth_set:np.ndarray|list, test_set:np.ndarray|list) -> float:
    denominator = np.max(ground_truth_set) - np.min(ground_truth_set)
    return np.sqrt(np.mean((np.array(ground_truth_set) - np.array(test_set)) ** 2, dtype=np.float64)) / denominator

kvp_set = np.arange(50, 200, 25)
filtration_set = [
    None,
    [["Cu", 1, "mm"]],
    [["Cu", 5, "mm"]],
    [["Cu", 1, "mm"], ["Al", 5, "mm"]],
    [["Cu", 5, "mm"], ["Al", 1, "mm"]],
]


def applyBinCutoff(energy_set:list, count_set:list, anEnergyInElectronVolt:float=5) -> tuple[list, list]:
    temp_energy_bins = []
    temp_photon_counts = []

    for energy, count in zip(energy_set, count_set):

        bin_energy_in_keV = energy * count
        bin_energy_in_eV = bin_energy_in_keV * gvxr.getUnitOfEnergy("keV") / gvxr.getUnitOfEnergy("eV")

        if bin_energy_in_eV > anEnergyInElectronVolt:
            temp_energy_bins.append(energy)
            temp_photon_counts.append(count)

    return temp_energy_bins, temp_photon_counts


def generateSpekpyGroundTruth(kvp:float, filtration:list | None) -> tuple[np.ndarray, np.ndarray]:

    # Instantiate the spectrum object
    s = sp.Spek(kvp=kvp, th=12, z=100)

    # There is at least one filter
    if filtration is not None:
        # Process all the filters
        for f in filtration:
            # Retrieve the material
            material = f[0]

            # Convert the thickness in mm if needed
            thickness_in_mm = f[1] * gvxr.getUnitOfLength(f[2]) / gvxr.getUnitOfLength("mm")

            # Add the filter
            s.filter(material, thickness_in_mm)

    # Get the spectrum. The photon counts is given in cm^2 at a distance of z from the source
    energy_bins:np.ndarray
    energy_bins, photon_counts = s.get_spectrum(edges=False)

    return energy_bins, photon_counts


def loop(addFilterLast:bool) -> None:
    for filtration in filtration_set:

        for kv in kvp_set:

            print("********************************************************************************")
            print("Test filtration:", filtration)
            print("Test kv:", kv)
            print("********************************************************************************")
            print()
            temp_bins, temp_counts = generateSpekpyGroundTruth(kv, filtration)

            ground_truth_energy_bins, ground_truth_photon_counts = applyBinCutoff(temp_bins, temp_counts)

            print("ground_truth", ground_truth_energy_bins[0], ground_truth_energy_bins[0] * ground_truth_photon_counts[0] * gvxr.getUnitOfEnergy("keV") / gvxr.getUnitOfEnergy("eV"))

            # Reset the spectrum
            gvxr.resetBeamSpectrum()
            gvxr.clearFiltration()

            # Add filters
            if not addFilterLast and filtration is not None:
                # Process all the filters
                for f in filtration:
                    # Retrieve the material
                    material = f[0]

                    # Convert the thickness in mm if needed
                    thickness = f[1] * gvxr.getUnitOfLength(f[2]) / gvxr.getUnitOfLength("mm")

                    # Add the filter
                    gvxr.addFilter(f[0], f[1], f[2])

            unfiltered_energy_bins, unfiltered_photon_counts = generateSpekpyGroundTruth(kv, None)

            for energy, count in zip(unfiltered_energy_bins, unfiltered_photon_counts):
                gvxr.addEnergyBinToSpectrumPerCm2At1m(energy, "keV", count)

            # Add filters
            if addFilterLast and filtration is not None:
                # Process all the filters
                for f in filtration:
                    # Retrieve the material
                    material = f[0]

                    # Convert the thickness in mm if needed
                    thickness = f[1] * gvxr.getUnitOfLength(f[2]) / gvxr.getUnitOfLength("mm")

                    # Add the filter
                    gvxr.addFilter(f[0], f[1], f[2])

            photon_energy_set, photon_count_set = applyBinCutoff(gvxr.getEnergyBins("keV"), gvxr.getPhotonCountsPerCm2At1m())

            print("test", photon_energy_set[0], photon_energy_set[0] * photon_count_set[0] * gvxr.getUnitOfEnergy("keV") / gvxr.getUnitOfEnergy("eV"))


            assert len(ground_truth_energy_bins) == len(photon_energy_set)
            assert len(ground_truth_photon_counts) == len(photon_count_set)


            metrics_energy = 100 * nrmse(ground_truth_energy_bins, photon_energy_set)
            metrics_count = 100 * nrmse(ground_truth_photon_counts, photon_count_set)

            # print("ENERGY", kv, filtration, metrics_energy)
            # print("COUNT", kv, filtration, metrics_count)
            assert metrics_energy < 1.5 # metrics_energy is between 0% and 100%
            assert metrics_count < 1.5 # metrics_count is between 0% and 100%


def test_spekpy1() -> None:
    loop(addFilterLast=True)

def test_spekpy2() -> None:
    loop(addFilterLast=False)



if __name__ == "__main__":
    unittest.main()
