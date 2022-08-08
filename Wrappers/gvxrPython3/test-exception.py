#!/usr/bin/env python3

import sys
import numpy as np

from gvxrPython3 import gvxr # Simulate X-ray images

image = np.zeros((40, 40))

try:
    print("Call a function that must generate a C++ exception")
    gvxr.getImageRow(image, 41)

except OSError as err:
    print("OS error: {0}".format(err))
except ValueError:
    print("Could not convert data to an integer.")
except Exception as inst:
    print(inst)
except BaseException as err:
    print(f"Unexpected {err=}, {type(err)=}")

print("Back to Python")
