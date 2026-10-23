import ipywidgets

from gvxrPython3 import gvxr

from .Viewer import Viewer


# The controls of the viewer, shown in the help panel. gVXR draws the same
# list into the 3D scene when it runs in a window of its own, see
# g_help_message in SimpleGVXR/src/OpenGLWindow.cxx, but that drawing needs
# FreeType and the render loop, neither of which a widget has
HELP = [
    ("Mouse", [
        ("Left drag", "Rotate the camera around the sample"),
        ("Middle drag", "Pan the view"),
        ("Right drag", "Rotate the sample itself"),
        ("Wheel", "Zoom in and out"),
        ("Double click", "Bring the sample back to the middle of the view"),
        ("S or D + drag or wheel", "Change the SOD or the ODD"),
    ]),
    ("Touch", [
        ("One finger", "Rotate the camera"),
        ("Two fingers", "Pan the view"),
        ("Pinch", "Zoom in and out"),
    ]),
    ("Keyboard, display", [
        ("B", "Show or hide the beam"),
        ("W", "Wireframe or solid"),
        ("N", "X-ray image positive or negative"),
        ("L", "Lighting on or off"),
        ("V", "Show or hide the normal vectors"),
        ("R", "Reset the view and the geometry"),
    ]),
    ("Keyboard, geometry", [
        ("S + ↑ or →", "Increase the SOD, ↓ or ← to decrease it"),
        ("D + ↑ or →", "Increase the ODD, ↓ or ← to decrease it"),
        ("1 + →", "Increase the ODD, ← to decrease it"),
        ("2 + ← or →", "Move the sample along the source-detector axis"),
        ("2 + ↑ or ↓", "Move the sample along the detector right vector"),
        ("3 + ↑ or ↓", "Move the sample up and down"),
        ("4 + ← or →", "Rotate the sample"),
        ("4 + ↑ or ↓", "Tilt the sample"),
    ]),
]


def helpAsHTML():
    """Return the list of controls as an HTML table."""
    rows = []

    for section, bindings in HELP:
        rows.append(
            "<tr><th colspan='2' style='text-align:left;padding-top:8px'>"
            f"{section}</th></tr>")

        for gesture, effect in bindings:
            rows.append(
                "<tr>"
                "<td style='padding-right:12px;white-space:nowrap'>"
                f"<code>{gesture}</code></td>"
                f"<td>{effect}</td>"
                "</tr>")

    return "<table style='border-collapse:collapse'>" + "".join(rows) + "</table>"


class ViewerPanel(ipywidgets.VBox):
    """A Viewer with the controls and the list of key bindings around it.

    The viewer on its own is driven by the mouse and the keyboard only, which
    works but leaves no trace on screen of what is possible. This adds the
    same commands as widgets, so that they can be found without knowing them
    already.
    """

    def __init__(self, viewer=None):

        self.viewer = viewer if viewer is not None else Viewer()

        beam = ipywidgets.Checkbox(value=True, description="Beam", indent=False)
        detector = ipywidgets.Checkbox(value=True, description="Detector", indent=False)
        wireframe = ipywidgets.Checkbox(value=False, description="Wireframe", indent=False)
        lighting = ipywidgets.Checkbox(value=True, description="Lighting", indent=False)
        negative = ipywidgets.Checkbox(value=False, description="Negative", indent=False)
        normals = ipywidgets.Checkbox(value=False, description="Normals", indent=False)

        self._bind(beam, gvxr.displayBeam)
        self._bind(detector, gvxr.displayDetector)
        self._bind(wireframe, gvxr.useWireframe)
        self._bind(lighting, gvxr.useLighing)
        self._bind(negative, gvxr.useNegative)
        self._bind(normals, gvxr.displayNormalVectors)

        toggles = ipywidgets.VBox([
            ipywidgets.HBox([beam, detector, wireframe]),
            ipywidgets.HBox([lighting, negative, normals]),
        ])

        sod = gvxr.getSOD("mm")
        odd = gvxr.getODD("mm")

        self._sod = ipywidgets.FloatSlider(
            value=sod, min=0.1 * sod, max=3.0 * sod,
            description="SOD (mm)", continuous_update=False,
            style={"description_width": "initial"})

        self._odd = ipywidgets.FloatSlider(
            value=odd, min=0.1 * odd, max=3.0 * odd,
            description="ODD (mm)", continuous_update=False,
            style={"description_width": "initial"})

        self._sod.observe(lambda change: self._setSOD(change["new"]), names="value")
        self._odd.observe(lambda change: self._setODD(change["new"]), names="value")

        fit = ipywidgets.Button(
            description="Fit to view",
            tooltip="Move the camera so that the whole sample is visible")

        reset = ipywidgets.Button(
            description="Reset",
            tooltip="Go back to the view and the geometry of the beginning")

        fit.on_click(lambda button: self.viewer.fit_to_view())
        reset.on_click(lambda button: self._reset())

        help_panel = ipywidgets.Accordion(
            children=[ipywidgets.HTML(helpAsHTML())])
        help_panel.set_title(0, "Controls")
        help_panel.selected_index = None

        super().__init__([
            self.viewer,
            ipywidgets.HBox([fit, reset]),
            toggles,
            self._sod,
            self._odd,
            help_panel,
        ])

    def _bind(self, checkbox, function):
        """Call function with the state of the checkbox, then redraw."""
        def changed(change):
            function(change["new"])
            self.viewer.refresh()

        checkbox.observe(changed, names="value")

    def _axis(self):
        """Unit vector pointing from the source towards the detector."""
        source = gvxr.getSourcePosition("mm")
        detector = gvxr.getDetectorPosition("mm")

        axis = [detector[i] - source[i] for i in range(3)]
        length = sum(value * value for value in axis) ** 0.5

        if length <= 0.0:
            return None, source, detector

        return [value / length for value in axis], source, detector

    def _setSOD(self, value):
        # Moving the source away from the detector increases the distance
        # between the source and the object, the same way updateSOD does it
        # in SimpleGVXR/src/Window.cxx
        axis, source, _ = self._axis()

        if axis is None:
            return

        step = value - gvxr.getSOD("mm")

        gvxr.setSourcePosition(
            source[0] - axis[0] * step,
            source[1] - axis[1] * step,
            source[2] - axis[2] * step,
            "mm")

        self.viewer.refresh()

    def _setODD(self, value):
        axis, _, detector = self._axis()

        if axis is None:
            return

        step = value - gvxr.getODD("mm")

        gvxr.setDetectorPosition(
            detector[0] + axis[0] * step,
            detector[1] + axis[1] * step,
            detector[2] + axis[2] * step,
            "mm")

        self.viewer.refresh()

    def _reset(self):
        self.viewer.reset_view()

        # The sliders no longer say what the geometry is, and writing to them
        # must not be taken for the user moving them
        self._sod.unobserve_all()
        self._odd.unobserve_all()

        self._sod.value = gvxr.getSOD("mm")
        self._odd.value = gvxr.getODD("mm")

        self._sod.observe(lambda change: self._setSOD(change["new"]), names="value")
        self._odd.observe(lambda change: self._setODD(change["new"]), names="value")
