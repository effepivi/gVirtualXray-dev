# The main GUI application
import wxApp;


import numpy as np

# Matplotlib is used to display the 2D images an the histogram
import matplotlib

# wxPython is used to create the GUI
import wx

# Matplotlib relies on the wxPython backend
matplotlib.use('WXAgg')
from matplotlib.backends.backend_wxagg import FigureCanvasWxAgg as FigureCanvas
from matplotlib.backends.backend_wx import NavigationToolbar2Wx

# gVirtualXRay's wrapper
import gvxrPython3 as gvxr;

import scipy;
from scipy import stats;

def createMeasureWindow():
    xray_image = wxApp.g_wxapp.updateXRayImage();

    print("Units of energy = ", gvxr.getUnitOfEnergy(wxApp.g_wxapp.param_box.current_energy_unit));)
    print("Area = ",      xray_image.shape[0] * xray_image.shape[1])
    print("Mean = ",      np.mean(xray_image))
    print("Std dev = ",   np.std(xray_image))
    print("Min = ",       np.min(xray_image))
    print("Max = ",       np.max(xray_image))
    print("Median = ",    np.median(xray_image))
    print("Perimetre = ", 2 * (xray_image.shape[0] + xray_image.shape[1]))
    print("Width = ",     xray_image.shape[1])
    print("Height = ",    xray_image.shape[0])
    print("Skew = ",      stats.skew(xray_image.reshape(-1)))
    print("Kurt = ",      stats.kurtosis(xray_image.reshape(-1)))
    print("Shannon entropy = ",  stats.entropy(xray_image.reshape(-1)))
