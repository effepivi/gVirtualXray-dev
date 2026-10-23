import math
from pathlib import Path

import anywidget
import traitlets

from gvxrPython3 import gvxr


class Viewer(anywidget.AnyWidget):

    _esm = Path(__file__).parent / "Viewer.js"

    png = traitlets.Bytes().tag(sync=True)

    key = traitlets.Unicode("").tag(sync=True)
    action = traitlets.Unicode("").tag(sync=True)

    ctrl = traitlets.Bool(False).tag(sync=True)
    shift = traitlets.Bool(False).tag(sync=True)
    alt = traitlets.Bool(False).tag(sync=True)

    key_event_id = traitlets.Int(0).tag(sync=True)

    mouse_x = traitlets.Float(0.0).tag(sync=True)
    mouse_y = traitlets.Float(0.0).tag(sync=True)

    mouse_button = traitlets.Int(-1).tag(sync=True)
    mouse_action = traitlets.Unicode("").tag(sync=True)

    wheel_dx = traitlets.Float(0.0).tag(sync=True)
    wheel_dy = traitlets.Float(0.0).tag(sync=True)

    mouse_event_id = traitlets.Int(0).tag(sync=True)

    def __init__(self):
        super().__init__()
        self._refreshing = False

        self.observe(self._on_key_event,
                     #names=["key", "action", "key_event_id"])
                        names = ["key_event_id"])

        self.observe(
            self._on_mouse_event,
            names=["mouse_event_id"]
        )

        gvxr.useAutoUnbindContext(True)
        self.refresh()

    def refresh(self):
        if self._refreshing:
            return

        self._refreshing = True

        try:
            png = gvxr.getScreenshotAsPNG()
            self.png = png if isinstance(png, bytes) else bytes(png)

        finally:
            self._refreshing = False


    def _on_key_event(self, change):

        # if change["name"] != "action" and change["name"] != "key_event_id":
        #     return

        if not self.key:
            return

        key_map = {
            # Function keys
            "F1": gvxr.KEY_F1,
            "F2": gvxr.KEY_F2,
            "F3": gvxr.KEY_F3,
            "F4": gvxr.KEY_F4,

            # Numeric keys
            "1": gvxr.KEY_1,
            "2": gvxr.KEY_2,
            "3": gvxr.KEY_3,
            "4": gvxr.KEY_4,

            # Letter keys
            "b": gvxr.KEY_B,
            "B": gvxr.KEY_B,

            "c": gvxr.KEY_C,
            "C": gvxr.KEY_C,

            "d": gvxr.KEY_D,
            "D": gvxr.KEY_D,

            "h": gvxr.KEY_H,
            "H": gvxr.KEY_H,

            "l": gvxr.KEY_L,
            "L": gvxr.KEY_L,

            "n": gvxr.KEY_N,
            "N": gvxr.KEY_N,

            "q": gvxr.KEY_Q,
            "Q": gvxr.KEY_Q,

            "r": gvxr.KEY_R,
            "R": gvxr.KEY_R,

            "s": gvxr.KEY_S,
            "S": gvxr.KEY_S,

            "v": gvxr.KEY_V,
            "V": gvxr.KEY_V,

            "w": gvxr.KEY_W,
            "W": gvxr.KEY_W,

            # Navigation keys
            "ArrowUp": gvxr.KEY_UP,
            "ArrowDown": gvxr.KEY_DOWN,
            "ArrowLeft": gvxr.KEY_LEFT,
            "ArrowRight": gvxr.KEY_RIGHT,

            # Special keys
            "Escape": gvxr.KEY_ESCAPE,
        }

        key = key_map.get(self.key)

        if key is None:
            return

        mods = 0

        if self.ctrl:
            mods |= gvxr.MOD_CTRL

        if self.shift:
            mods |= gvxr.MOD_SHIFT

        if self.alt:
            mods |= gvxr.MOD_ALT


        if self.action == "keydown":
            action = gvxr.PRESS
        elif self.action == "keyup":
            action = gvxr.RELEASE
        else:
            action = None

        if action is not None:
            gvxr.keyCallback(
                key,
                0,
                action,
                mods
            )

        self.refresh()

    def _on_mouse_event(self, change):

        mouse_button = -1
        if self.mouse_button == 0:
            mouse_button = gvxr.MOUSE_BUTTON_LEFT
        elif self.mouse_button == 1:
            mouse_button = gvxr.MOUSE_BUTTON_MIDDLE
        elif self.mouse_button == 2:
            mouse_button = gvxr.MOUSE_BUTTON_RIGHT

        if self.mouse_action == "mousedown":
            gvxr.mouseButtonCallback(
                mouse_button,
                gvxr.PRESS,
                0,
                self.mouse_x,
                self.mouse_y,
            )

        elif self.mouse_action == "mouseup":
            gvxr.mouseButtonCallback(
                mouse_button,
                gvxr.RELEASE,
                0,
                self.mouse_x,
                self.mouse_y,
            )

        elif self.mouse_action == "mousemove":
            gvxr.cursorPositionCallback(
                self.mouse_x,
                self.mouse_y
            )

        elif self.mouse_action == "wheel":
            gvxr.scrollCallback(
                self.wheel_dx,
                self.wheel_dy
            )

        elif self.mouse_action == "dblclick":
            self.centre_on_sample()

        self.refresh()

    # ------------------------------------------------------------------
    # View commands
    # ------------------------------------------------------------------

    def centre_on_sample(self):
        """Bring the sample back to the middle of the view.

        The camera orbits and pans about its reference point, so once the
        sample has been moved away from it the rotation feels off centre.
        Both the camera and its reference point are translated by the same
        amount, which slides the view across without turning it.
        """
        centre = gvxr.getNodeAndChildrenBoundingBoxCentre("root", "mm")
        target = gvxr.getCameraReferencePoint("mm")
        position = gvxr.getCameraPosition("mm")

        shift = [centre[i] - target[i] for i in range(3)]

        gvxr.setCameraPosition(
            position[0] + shift[0],
            position[1] + shift[1],
            position[2] + shift[2],
            "mm")

        gvxr.setCameraReferencePoint(centre[0], centre[1], centre[2], "mm")

    def fit_to_view(self):
        """Frame the whole sample, keeping the direction the camera looks in.

        Unlike the R key, which goes back to the view the window started
        with, this works out where the camera has to sit for the sample in
        front of it now to fill the screen.
        """
        bbox = gvxr.getNodeAndChildrenBoundingBox("root", "mm")

        centre = [bbox[i] + (bbox[i + 3] - bbox[i]) / 2.0 for i in range(3)]
        diagonal = math.dist(bbox[0:3], bbox[3:6])

        if diagonal <= 0.0:
            return

        position = gvxr.getCameraPosition("mm")

        # Unit vector from the sample towards the camera, so that the camera
        # only moves closer or further away, never around
        back = [position[i] - centre[i] for i in range(3)]
        length = math.sqrt(sum(value * value for value in back))

        if length <= 0.0:
            back, length = [0.0, 0.0, 1.0], 1.0

        back = [value / length for value in back]

        # Half the diagonal is the radius of the sphere around the sample.
        # Sitting at radius / sin(half the field of view) away puts that
        # sphere exactly inside the frustum, and a little margin is added so
        # that the sample does not touch the edges
        FOVY_IN_DEGREES = 45.0
        distance = 1.1 * (diagonal / 2.0) / math.sin(math.radians(FOVY_IN_DEGREES) / 2.0)

        gvxr.setCameraPosition(
            centre[0] + back[0] * distance,
            centre[1] + back[1] * distance,
            centre[2] + back[2] * distance,
            "mm")

        gvxr.setCameraReferencePoint(centre[0], centre[1], centre[2], "mm")

        self.refresh()

    def reset_view(self):
        """Go back to the view and the geometry the window started with."""
        gvxr.keyCallback(gvxr.KEY_R, 0, gvxr.PRESS, 0)
        gvxr.keyCallback(gvxr.KEY_R, 0, gvxr.RELEASE, 0)
        self.refresh()