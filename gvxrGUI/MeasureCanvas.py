# The main GUI application
import wxApp;


import numpy as np

# Matplotlib is used to display the 2D images an the histogram
import matplotlib

# wxPython is used to create the GUI
import wx
import wx.grid

# Matplotlib relies on the wxPython backend
matplotlib.use('WXAgg')
from matplotlib.backends.backend_wxagg import FigureCanvasWxAgg as FigureCanvas
from matplotlib.backends.backend_wx import NavigationToolbar2Wx

# gVirtualXRay's wrapper
import gvxrPython3 as gvxr;

import scipy;
from scipy import stats;

NoneType = type(None);
measure_grid_frame = None;

class MeasureGridFrame(wx.Frame):
    def __init__(self, parent):
        measure_grid_frame = self;
        wx.Frame.__init__(self, parent)

        # Create a wxGrid object
        self.grid = wx.grid.Grid(self, -1)

        self.column_titles = [];

        self.column_titles.append("Width");
        self.column_titles.append("Height");
        self.column_titles.append("Area");
        self.column_titles.append("Perimetre");

        self.column_titles.append("Units of energy");
        self.column_titles.append("Min");
        self.column_titles.append("Max");
        self.column_titles.append("Median");
        self.column_titles.append("Mean");
        self.column_titles.append("Std dev");

        self.column_titles.append("Skewness");
        self.column_titles.append("Kurtosis");
        self.column_titles.append("Shannon entropy");


        # Then we call CreateGrid to set the dimensions of the grid
        # (1 rows and 13 columns in this example)
        self.grid.CreateGrid(1, len(self.column_titles))

        # We can set the sizes of individual rows and columns
        # in pixels
        #self.grid.SetRowSize(0, 60)
        #self.grid.SetColSize(0, 120)

        # Add title in grid
        for title, i in zip(self.column_titles, range(len(self.column_titles))):
            self.grid.SetCellValue(0, i, title);
            self.grid.SetReadOnly(0, i);
            self.grid.SetCellBackgroundColour(0, i, wx.LIGHT_GREY);
            self.grid.SetCellTextColour(0, i, wx.BLACK);

            if i >= 5:
                self.grid.SetColFormatFloat(i, 6, 3);



        # Colours can be specified for grid cell contents
        #self.grid.SetCellValue(3, 3, 'green on grey')
        #self.grid.SetCellTextColour(3, 3, wx.GREEN)

        # We can specify the some cells will store numeric
        # values rather than strings. Here we set grid column 5
        # to hold floating point values displayed with width of 6
        # and precision of 2
        #self.grid.SetColFormatFloat(5, 6, 2)
        #self.grid.SetCellValue(0, 6, '3.1415')

        self.addRow();
        self.Show();

        self.Bind(wx.EVT_CLOSE, self.processCloseEvent);

    def processCloseEvent(self, event):
        global measure_grid_frame;
        measure_grid_frame = None;
        event.Skip()

    def addRow(self):

        xray_image = wxApp.g_wxapp.updateXRayImage();

        self.grid.AppendRows();
        number_of_rows = self.grid.GetNumberRows();

        i = 0;

        self.grid.SetReadOnly(number_of_rows - 1, i);
        self.grid.SetCellValue(number_of_rows - 1, i, str(xray_image.shape[1]));
        i += 1;

        self.grid.SetReadOnly(number_of_rows - 1, i);
        self.grid.SetCellValue(number_of_rows - 1, i, str(xray_image.shape[0]));
        i += 1;

        self.grid.SetReadOnly(number_of_rows - 1, i);
        self.grid.SetCellValue(number_of_rows - 1, i, str(xray_image.shape[0] * xray_image.shape[1]));
        i += 1;

        self.grid.SetReadOnly(number_of_rows - 1, i);
        self.grid.SetCellValue(number_of_rows - 1, i, str(2 * (xray_image.shape[0] + xray_image.shape[1])));
        i += 1;

        self.grid.SetReadOnly(number_of_rows - 1, i);
        self.grid.SetCellValue(number_of_rows - 1, i, wxApp.g_wxapp.current_energy_unit);
        i += 1;

        self.grid.SetReadOnly(number_of_rows - 1, i);
        self.grid.SetCellValue(number_of_rows - 1, i, str(np.min(xray_image)));
        i += 1;

        self.grid.SetReadOnly(number_of_rows - 1, i);
        self.grid.SetCellValue(number_of_rows - 1, i, str(np.max(xray_image)));
        i += 1;

        self.grid.SetReadOnly(number_of_rows - 1, i);
        self.grid.SetCellValue(number_of_rows - 1, i, str(np.median(xray_image)));
        i += 1;

        self.grid.SetReadOnly(number_of_rows - 1, i);
        self.grid.SetCellValue(number_of_rows - 1, i, str(np.mean(xray_image)));
        i += 1;

        self.grid.SetReadOnly(number_of_rows - 1, i);
        self.grid.SetCellValue(number_of_rows - 1, i, str(np.std(xray_image)));
        i += 1;


        self.grid.SetReadOnly(number_of_rows - 1, i);
        self.grid.SetCellValue(number_of_rows - 1, i, str(stats.skew(xray_image.reshape(-1))));
        i += 1;

        self.grid.SetReadOnly(number_of_rows - 1, i);
        self.grid.SetCellValue(number_of_rows - 1, i, str(stats.kurtosis(xray_image.reshape(-1))));
        i += 1;

        self.grid.SetReadOnly(number_of_rows - 1, i);
        self.grid.SetCellValue(number_of_rows - 1, i, str(stats.entropy(xray_image.reshape(-1))));
        i += 1;

def createMeasureWindow():
    global measure_grid_frame;

    if isinstance(measure_grid_frame, NoneType):
        measure_grid_frame = MeasureGridFrame(None);
    else:
        measure_grid_frame.addRow();
