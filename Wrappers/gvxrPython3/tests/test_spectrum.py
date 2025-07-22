from math import isclose
import unittest
import numpy as np
import spekpy as sp

from gvxrPython3 import gvxr # Simulate X-ray images
# from gvxrPython3.utils import getTextFileSpectrum
# from gvxrPython3.utils import getSpectrumSpekpy
# from gvxrPython3.utils import getSpectrumXpecgen
# from gvxrPython3.utils import has_spekpy
# from gvxrPython3.utils import has_xpecgen
# from gvxrPython3.utils import has_skimage
# from gvxrPython3.utils import applyFiltration

import matplotlib.pyplot as plt


def nrmse(ground_truth_set, test_set):
    denominator = ground_truth_set.max() - ground_truth_set.min()
    return np.sqrt(np.mean((ground_truth_set - test_set) ** 2, dtype=np.float64)) / denominator


class TestingClass(unittest.TestCase):

    def __init__(self, *args, **kwargs):
        super(TestingClass, self).__init__(*args, **kwargs)
        
        self.kvp_set = np.arange(50, 200, 25)
        self.filtration_set = [
            None,
            [["Cu", 1, "mm"]],
            [["Cu", 5, "mm"]],
            [["Cu", 1, "mm"], ["Al", 5, "mm"]],
            [["Cu", 5, "mm"], ["Al", 1, "mm"]]
        ]

    def generateSpekpyGroundTruth(self, kvp, filtration):

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
        energy_bins, photon_counts = s.get_spectrum(edges=False)

        return energy_bins, photon_counts


    def test_spekpy1(self):


        for filtration in self.filtration_set:

            for kv in self.kvp_set:

                print("********************************************************************************")
                print("Test filtration:", filtration)
                print("Test kv:", kv)
                print("********************************************************************************")
                print()
                ground_truth_energy_bins, ground_truth_photon_counts = self.generateSpekpyGroundTruth(kv, filtration)

                # Reset the spectrum
                gvxr.resetBeamSpectrum()
                gvxr.clearFiltration()

                unfiltered_energy_bins, unfiltered_photon_counts = self.generateSpekpyGroundTruth(kv, None)
                for energy, count in zip(unfiltered_energy_bins, unfiltered_photon_counts):
                    gvxr.addEnergyBinToSpectrumPerCm2At1m(energy, "keV", count)
                
                # There is at least one filter
                if filtration is not None:
                    # Process all the filters
                    for f in filtration:
                        # Retrieve the material
                        material = f[0]

                        # Convert the thickness in mm if needed
                        thickness = f[1] * gvxr.getUnitOfLength(f[2]) / gvxr.getUnitOfLength("mm")

                        # Add the filter
                        gvxr.addFilter(f[0], f[1], f[2])

                photon_energy_set = gvxr.getEnergyBins("keV")
                photon_count_set = gvxr.getPhotonCountsPerCm2At1m()

                metrics_energy = 100 * nrmse(ground_truth_energy_bins, photon_energy_set)
                metrics_count = 100 * nrmse(ground_truth_photon_counts, photon_count_set)

                # print("ENERGY", kv, filtration, metrics_energy)
                # print("COUNT", kv, filtration, metrics_count)

                # plt.figure()
                # plt.title(str(kv) + " kv")
                # plt.plot(ground_truth_energy_bins, ground_truth_photon_counts, label="ground truth")
                # plt.plot(photon_energy_set, photon_count_set, label="test")
                # plt.legend()
                # plt.show()

                self.assertEqual(metrics_energy < 1.0, True) # metrics_energy is between 0 and 100
                self.assertEqual(metrics_count < 1.0, True) # metrics_energy is between 0 and 100


    def test_spekpy2(self):


        for filtration in self.filtration_set:

            gvxr.clearFiltration()

            # There is at least one filter
            if filtration is not None:
                # Process all the filters
                for f in filtration:
                    # Retrieve the material
                    material = f[0]

                    # Convert the thickness in mm if needed
                    thickness = f[1] * gvxr.getUnitOfLength(f[2]) / gvxr.getUnitOfLength("mm")

                    # Add the filter
                    gvxr.addFilter(f[0], f[1], f[2])

            for kv in self.kvp_set:

                print("********************************************************************************")
                print("Test filtration:", filtration)
                print("Test kv:", kv)
                print("********************************************************************************")
                print()
                ground_truth_energy_bins, ground_truth_photon_counts = self.generateSpekpyGroundTruth(kv, filtration)

                # Reset the spectrum
                gvxr.resetBeamSpectrum()

                unfiltered_energy_bins, unfiltered_photon_counts = self.generateSpekpyGroundTruth(kv, None)
                for energy, count in zip(unfiltered_energy_bins, unfiltered_photon_counts):
                    gvxr.addEnergyBinToSpectrumPerCm2At1m(energy, "keV", count)
                


                photon_energy_set = gvxr.getEnergyBins("keV")
                photon_count_set = gvxr.getPhotonCountsPerCm2At1m()


                metrics_energy = 100 * nrmse(ground_truth_energy_bins, photon_energy_set)
                metrics_count = 100 * nrmse(ground_truth_photon_counts, photon_count_set)

                print("ENERGY", kv, filtration, metrics_energy)
                print("COUNT", kv, filtration, metrics_count)


                # plt.figure()
                # plt.title(str(kv) + " kv")
                # plt.plot(ground_truth_energy_bins, ground_truth_photon_counts, label="ground truth")
                # plt.plot(photon_energy_set, photon_count_set, label="test")
                # plt.legend()
                # plt.show()

                self.assertEqual(metrics_energy < 1.0, True) # metrics_energy is between 0 and 100
                self.assertEqual(metrics_count < 1.0, True) # metrics_energy is between 0 and 100


if __name__ == '__main__':
    unittest.main()