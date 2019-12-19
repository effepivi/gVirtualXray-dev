
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

class wxCanvasPanel(wx.Frame):
    def __init__(self, parent, aParamContainer):
        wx.Frame.__init__(self, parent)

        menu_bar = wx.MenuBar()
        file_menu = wx.Menu()
        menu_bar.Append(file_menu, '&File')

        self.im = None;
        self.colorbar = None;
        '''
        save_item = wx.MenuItem(file_menu, wx.ID_SAVE, text = "&Save\tCtrl+S",kind = wx.ITEM_NORMAL)
        file_menu.Append(save_item)

        save_as_item = wx.MenuItem(file_menu, wx.ID_SAVEAS, text = "&Save as...\tCtrl+Shift+S",kind = wx.ITEM_NORMAL)
        file_menu.Append(save_as_item)

        file_menu.AppendSeparator()

        close_item = wx.MenuItem(file_menu, wx.ID_CLOSE, text = "&Close\tCtrl+W", kind = wx.ITEM_NORMAL)
        file_menu.Append(close_item)
        '''

        self.SetMenuBar(menu_bar)

        self.status_bar = wx.StatusBar(self, -1)
        self.SetStatusBar(self.status_bar)

        #self.Bind(wx.EVT_MENU, self.menuhandler)

        self.figure = Figure()
        self.axes = self.figure.add_subplot(111)
        self.canvas = FigureCanvas(self, -1, self.figure)


        self.canvas.Bind(wx.EVT_ENTER_WINDOW, self.onMouseOver)
        self.canvas.Bind(wx.EVT_LEAVE_WINDOW, self.onMouseLeave)
        self.canvas.mpl_connect('motion_notify_event', self.onMouseMotion)

        self.sizer = wx.BoxSizer(wx.VERTICAL)
        self.sizer.Add(self.canvas, 1, wx.LEFT | wx.TOP | wx.GROW)

        self.rb1 = wx.RadioButton(self, 11, label = 'Linear colour scale')
        self.rb2 = wx.RadioButton(self, 22, label = 'Log colour scale')
        self.rb3 = wx.RadioButton(self, 33, label = 'Power-law colour scale')

        self.sizer.Add(self.rb1, 0, wx.LEFT | wx.TOP | wx.GROW)
        self.sizer.Add(self.rb2, 0, wx.LEFT | wx.TOP | wx.GROW)
        self.sizer.Add(self.rb3, 0, wx.LEFT | wx.TOP | wx.GROW)

        self.color_scale = 0;
        self.Bind(wx.EVT_RADIOBUTTON, self.onRadiogroup);

        self.SetSizer(self.sizer)
        self.Fit()

        self.data_ready = False;
        self.current_energy_unit = aParamContainer;

        #self.draw();
        #self.Show();
    def onRadiogroup(self, event):
        id = event.GetId()
        if id == 11:
            self.color_scale = 0;
        elif id == 22:
            self.color_scale = 1;
        elif id == 33:
            self.color_scale = 2;

        self.draw();
        self.figure.canvas.draw();
        self.Refresh(True);
        self.Update()
        self.Show();

    def onMouseMotion(self, event):
        if event.inaxes:
            x, y = event.xdata, event.ydata

            '''value = self.bins[0];
            count = self.hist[0];

            for temp_value, temp_count in zip(self.bins[:-1], self.hist):

                if value <= x and x <= temp_value:
                    value = temp_value;
                    count = temp_count;

            if x >= self.bins[len(self.hist)-1]:
                value = self.bins[len(self.hist)-1];
                count = self.hist[-1];

            self.status_bar.SetStatusText(("value= " + str(value) + "   count=" + str(count)))'''

    def onMouseOver(self, event):
        self.canvas.SetCursor(wx.Cursor(wx.CURSOR_BULLSEYE));
        event.Skip();

    def onMouseLeave(self, event):
        self.canvas.SetCursor(wx.Cursor(wx.CURSOR_ARROW));
        event.Skip();

    def draw(self):

        if self.data_ready:
            print("DRAW")
            self.axes.clear();

            if isinstance(self.colorbar, NoneType) == False:
                self.colorbar.remove();

            self.x_ray_image = np.divide(gvxr.computeXRayImage(), gvxr.getUnitOfEnergy(self.current_energy_unit));

            wxApp.g_need_refresh_xray_image = False;

            displayed_image = 255 * (self.x_ray_image - np.min(self.x_ray_image)) / (np.max(self.x_ray_image) - np.min(self.x_ray_image));

            print(displayed_image.shape, np.min(displayed_image), np.max(displayed_image))

            cmap = "gray";
            norm = None;
            if self.color_scale == 0:
                self.axes.set_title("Using a linear colour scale");
            elif self.color_scale == 1:
                self.axes.set_title("Using a log colour scale");
                norm=LogNorm();
            elif self.color_scale == 2:
                self.axes.set_title("Using a Power-law colour scale");
                norm=PowerNorm(gamma=1./2.);

            self.im = self.axes.imshow(displayed_image, cmap=cmap, norm=norm);

            divider = make_axes_locatable(self.axes)
            cax = divider.append_axes('bottom', size='25%', pad=0.5)
            self.colorbar = self.figure.colorbar(self.im, cax=cax, orientation='horizontal');


            self.figure.canvas.draw();
            self.Refresh(True);
            self.Update()
            self.Show();

    def updateImage(self, image):
        print("updateImage")
        if self.data_ready:

            if isinstance(self.im, NoneType):
                print("YEAH")
                self.draw();
            else:
                self.draw();
                self.im.set_data(image);
                self.figure.canvas.Refresh(True);
                self.figure.canvas.Update()
                self.Refresh(True);
                self.Update()
