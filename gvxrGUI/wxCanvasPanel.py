
import numpy as np


# wxPython is used to create the GUI
import wx

# Matplotlib is used to display the 2D images an the histogram
import matplotlib

# Matplotlib relies on the wxPython backend
matplotlib.use('WXAgg')
from matplotlib.backends.backend_wxagg import FigureCanvasWxAgg as FigureCanvas
from matplotlib.backends.backend_wx import NavigationToolbar2Wx

from matplotlib.figure import Figure
from mpl_toolkits.axes_grid1 import make_axes_locatable

# Use different scales to display the X-ray image (log and power)
from matplotlib.colors import LogNorm
from matplotlib.colors import PowerNorm

# gVirtualXRay's wrapper
import gvxrPython3 as gvxr;

# The main GUI application
import wxApp;


NoneType = type(None);

class wxCanvasPanel(wx.Panel):
    def __init__(self, parent, aParamContainer):
        wx.Panel.__init__(self, parent)
        self.figure = Figure()
        self.axes1 = self.figure.add_subplot(131)
        self.axes2 = self.figure.add_subplot(132)
        self.axes3 = self.figure.add_subplot(133)
        self.canvas = FigureCanvas(self, -1, self.figure)
        self.sizer = wx.BoxSizer(wx.VERTICAL)
        self.sizer.Add(self.canvas, 1, wx.LEFT | wx.TOP | wx.GROW)
        self.SetSizer(self.sizer)
        self.Fit()
        self.data_ready = False;
        self.current_energy_unit = aParamContainer;
        self.colorbar1 = None;
        self.colorbar2 = None;
        self.colorbar3 = None;

    def draw(self):

        if self.data_ready:
            print("Display images")
            if isinstance(self.colorbar1, NoneType) == False:
                self.colorbar1.remove();
            if isinstance(self.colorbar2, NoneType) == False:
                self.colorbar2.remove();
            if isinstance(self.colorbar3, NoneType) == False:
                self.colorbar3.remove();

            x_ray_image = np.divide(gvxr.computeXRayImage(), gvxr.getUnitOfEnergy(self.current_energy_unit));


            wxApp.g_need_refresh_xray_image = False;

            print(self.current_energy_unit);
            print(np.max(x_ray_image));

            self.im1 = self.axes1.imshow(x_ray_image, cmap="gray");
            self.axes1.set_title("Using a linear colour scale");

            divider = make_axes_locatable(self.axes1)
            cax = divider.append_axes('bottom', size='25%', pad=0.5)
            self.colorbar1 = self.figure.colorbar(self.im1, cax=cax, orientation='horizontal');

            self.im2 = self.axes2.imshow(x_ray_image, norm=LogNorm(), cmap="gray");
            self.axes2.set_title("Using a log colour scale");

            divider = make_axes_locatable(self.axes2)
            cax = divider.append_axes('bottom', size='25%', pad=0.5)
            self.colorbar2 = self.figure.colorbar(self.im2, cax=cax, orientation='horizontal');

            self.im3 = self.axes3.imshow(x_ray_image, norm=PowerNorm(gamma=1./2.), cmap="gray");
            self.axes3.set_title("Using a Power-law colour scale");

            divider = make_axes_locatable(self.axes3)
            cax = divider.append_axes('bottom', size='25%', pad=0.5)
            self.colorbar3 = self.figure.colorbar(self.im3, cax=cax, orientation='horizontal');

    def updateImage(self, image):
        if self.data_ready:
            self.im1.set_data(image);
            self.im2.set_data(image);
            self.im3.set_data(image);
            self.figure.canvas.draw();
            self.Refresh();
