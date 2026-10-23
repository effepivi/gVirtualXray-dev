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
            self.png = bytes(
                gvxr.getScreenshotAsPNG()
            )

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

        self.refresh()