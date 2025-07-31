#!/usr/bin/env python3

import unittest

import sys
import numpy as np
import os

from gvxrPython3 import gvxr # Simulate X-ray images

class TestingClass(unittest.TestCase):

    def __init__(self, *args, **kwargs):
        super(TestingClass, self).__init__(*args, **kwargs)

    def test_out_of_bound_access(self):

        with self.assertRaises(Exception) as context:

            image = np.zeros((40, 40))
            gvxr.getImageRow(image, 41)

        self.assertTrue("Error: Out of bounds Memory Access." in str(context.exception))

    def test_invalid_rotation_axis(self):

        with self.assertRaises(Exception) as context:

            gvxr.rotateModelView(90, 0, 0, 0)

        self.assertTrue("Error: Invalid rotation axis," in str(context.exception) and "'s length is zero." in str(context.exception))

    def test_invalid_scaling_factors(self):

        with self.assertRaises(Exception) as context:

            gvxr.scaleScene(0, 0, 0)

        self.assertTrue("Invalid scaling factors, all of them are null: 0, 0, 0" in str(context.exception))


    def test_destroy(self):
        gvxr.destroy()




if __name__ == "__main__":
    unittest.main()
