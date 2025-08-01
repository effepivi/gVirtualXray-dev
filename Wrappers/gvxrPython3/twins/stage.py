from __future__ import annotations

from dataclasses import dataclass


@dataclass(kw_only=True)
class XYZRange:
    x: tuple[float, float, float | None]
    y: tuple[float, float, float | None]
    z: tuple[float, float, float | None]

    def to_json(self) -> dict:
        return self.__dict__

    @staticmethod
    def from_json(json: dict) -> XYZRange:
        x = list(json["x"])
        y = list(json["y"])
        z = list(json["z"])

        if len(x) > 1 and len(x) < 3:
            x += [None]

        if len(x) > 1 and len(y) < 3:
            y += [None]

        if len(x) > 1 and len(z) < 3:
            z += [None]

        x = tuple(x)
        y = tuple(y)
        z = tuple(z)

        return XYZRange(x=x, y=y, z=z)

    def get_default_position(self) -> tuple[float, float, float]:
        # Position in [mm],
        # Z values are absolute distance in mm away from sample at 0, 0, 0
        default_x = 0
        default_y = 0
        default_z = 0

        if self.x[2] is not None:
            # Use the default range
            default_x = self.x[2]
        elif self.x != [0, 0, None]:
            # Use midpoint between max-min
            default_x = self.x[0] + (self.x[1] - self.x[0]) / 2


        if self.y[2] is not None:
            # Use the default range
            default_y = self.y[2]
        elif self.y != [0, 0, None]:
            # Use midpoint between max-min
            default_y = self.y[0] + (self.y[1] - self.y[0]) / 2


        if self.z[2] is not None:
            # Use the default range
            default_z = self.z[2]
        elif self.z != [0, 0, None]:
            # Use midpoint between max-min
            default_z = self.z[0] + (self.z[1] - self.z[0]) / 2
        else:
            # Use a default Z-Axis value of 500mm if position is unconstrained.
            default_z = 500

        return (default_x, default_y, default_z)


@dataclass(kw_only=True)
class TwinStage:
    source: XYZRange
    detector: XYZRange
    static_sdd: bool

    def to_json(self) -> dict:
        return self.__dict__

    @staticmethod
    def from_json(json: dict) -> TwinStage:
        source = XYZRange.from_json(json["source"])
        detector = XYZRange.from_json(json["detector"])
        static_sdd = bool(json["static_sdd"])

        return TwinStage(
            source=source,
            detector=detector,
            static_sdd=static_sdd,
        )
