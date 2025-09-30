from __future__ import annotations

import warnings
from collections.abc import Sequence
from dataclasses import dataclass
from pathlib import Path
from typing import Literal, cast

import json5
import numpy as np
from gvxrPython3 import gvxr

from .beam import PROJECTION, TwinBeam, TwinBeamFixedSpectrum, TwinBeamMonochromatic, TwinBeamTube, beam_from_json
from .detector import TwinDetector
from .stage import TwinStage, XYZRange

# ======================================================== #


@dataclass(frozen=True, kw_only=True)
class DigitalTwinSpecification:
    """A Digital Twin Specification contains the definition of an X-ray scanning system.

    This class contains definitions for the X-ray source, detector, and movement stage.
    Including maximum-minimum X-ray tube voltage, stage extents, detector exposures, etc

    This class corresponds with the JSON5 digital twin specification.

    """

    # Static twin definition
    name: str
    description: str
    date: str

    beams: dict[str, TwinBeam]
    detector: TwinDetector
    stage: TwinStage

    # ======================================================== #

    @staticmethod
    def from_json(json: dict) -> DigitalTwinSpecification:
        name = json["name"]
        description = json["description"]
        date = json["date"]
        beams: dict[str, TwinBeam] = {}

        for key, value in dict(json["beams"]).items():
            beams[str(key)] = beam_from_json(value)

        detector = TwinDetector.from_json(json["detector"])
        stage = TwinStage.from_json(json["stage"])

        return DigitalTwinSpecification(
            name=name,
            description=description,
            date=date,
            beams=beams,
            detector=detector,
            stage=stage,
        )

    @staticmethod
    def from_file(path: Path | str) -> DigitalTwinSpecification:
        path = Path(path)
        with path.open("r") as f:
            return DigitalTwinSpecification.from_json(json5.load(f))

    def to_json(self) -> dict:
        return self.__dict__.copy()


# ======================================================== #


@dataclass(kw_only=True)
class Positional:
    _name: str
    _position: tuple[float, float, float]

    # ======================================================== #

    @property
    def position(self) -> tuple[float, float, float]:
        return self._position

    @position.setter
    def position(self, value: float | Sequence[float] | np.ndarray) -> None:
        if isinstance(value, np.ndarray):
            value = value.tolist()

        if isinstance(value, Sequence):
            if len(value) == 1:
                # List with a single item, attempt to convert into a single
                # float value
                value = abs(float(value[0]))
            elif len(value) == 3:
                self._position = (float(value[0]), float(value[1]), abs(float(value[2])))
            else:
                raise ValueError("Position must be a float [z], or list of floats in [x, y, z] format.")

        if isinstance(value, float):
            # Single float value, assume position on beam axis
            self._position = (0, 0, value)

    # ======================================================== #

    def validate(self, stage: XYZRange) -> tuple[bool, list[str]]:
        errors: list[str] = []

        if stage.x is not None:
            # Validate x axis
            if stage.x == (0, 0, None):
                if self.position[0] != 0:
                    errors.append(f"{self._name} does not support x movement. '{self.position[0]}mm' is out of bounds")

            elif stage.x[0] == stage.x[1] and self.position[0] != stage.x[0]:
                errors.append(
                    f"{self._name} does not support x movement, '{self.position[0]}mm' must be {stage.x[0]}mm"
                )

            elif self.position[0] < stage.x[0] or self.position[0] > stage.x[1]:
                errors.append(
                    f"{self._name} X position '{self.position[0]}mm' must be between {stage.x[0]}mm - {stage.x[1]}mm",
                )

        if stage.y is not None:
            # Validate y axis
            if stage.y == (0, 0, None):
                if self.position[1] != 0:
                    errors.append(f"{self._name} does not support y movement. '{self.position[1]}mm' is out of bounds")

            elif stage.y[0] == stage.y[1] and self.position[1] != stage.y[0]:
                errors.append(
                    f"{self._name} does not support y movement, '{self.position[1]}mm' must be {stage.y[0]}mm"
                )

            elif self.position[1] < stage.y[0] or self.position[0] > stage.y[1]:
                errors.append(
                    f"{self._name} Y position '{self.position[1]}'mm must be between {stage.y[0]}mm - {stage.y[1]}mm",
                )

        if stage.z is not None:
            # Validate z axis
            if stage.z == (0, 0, None):
                if self.position[2]:
                    errors.append(f"{self._name} does not support x movement. '{self.position[2]}mm' is out of bounds")

            elif stage.z[0] == stage.z[1] and self.position[2] != stage.z[0]:
                errors.append(
                    f"{self._name} does not support z movement, '{self.position[2]}mm' must be {stage.z[0]}mm"
                )

            elif self.position[2] < stage.z[0] or self.position[2] > stage.z[1]:
                errors.append(
                    f"{self._name} Z position '{self.position[2]}'mm must be between {stage.z[0]}mm - {stage.z[1]}mm",
                )

        return len(errors) != 0, errors


@dataclass(kw_only=True)
class TwinBeamSettings(Positional):
    _kev: float
    _ua: float
    _filter: list[tuple[str, float]]

    # ======================================================== #

    @property
    def kev(self) -> float:
        return self._kev

    @kev.setter
    def kev(self, value: float) -> None:
        self._kev = value

    @property
    def ua(self) -> float:
        return self._ua

    @ua.setter
    def ua(self, value: float) -> None:
        self._ua = value

    @property
    def filter(self) -> list[tuple[str, float]]:
        return self._filter

    @filter.setter
    def filter(self, value: Sequence[str | float] | Sequence[Sequence]) -> None:
        if len(value) < 2 or (isinstance(value[0], str) and not isinstance(value[1], float)):
            raise ValueError("Filter must be a list in the format [str, mm], or [[str, mm], [str, mm]]")

        if isinstance(value[0], str) and isinstance(value[1], float):
            # Single tuple in format [str, mm]
            self._filter = [(value[0], value[1])]

        elif isinstance(value[0], Sequence):
            # Nested lists in format [[str, mm], [str, mm]]
            self._filter = cast("list[tuple[str, float]]", value)

    # ======================================================== #

    def validate(self, beam: TwinBeam, stage: XYZRange) -> tuple[bool, list[str]]:
        errors = []

        if beam.filters != []:
            # Validate filters
            warnings.warn(RuntimeWarning("Beam filter validation is not implemented"), stacklevel=1)

        if isinstance(beam, (TwinBeamTube, TwinBeamMonochromatic)):  # noqa: SIM102
            if self.kev < beam.keV[0] or self.kev > beam.keV[1]:
                errors.append(f"keV of '{self.kev}' must be between {beam.keV[0]}kV - {beam.keV[1]}kV")

        if isinstance(beam, TwinBeamTube):  # noqa: SIM102
            if self.ua < beam.uA[0] or self.ua > beam.uA[1]:
                errors.append(f"Beam uA of '{self.ua}'μA must be between {beam.uA[0]}μA - {beam.uA[1]}μA")

        # Validate beam positon
        _, stage_errors = super().validate(stage)
        errors += stage_errors

        return len(errors) != 0, errors


@dataclass(kw_only=True)
class TwinDetectorSettings(Positional):
    exposure: float
    fov: int
    gain: int

    # ======================================================== #

    def resolution(self, detector: TwinDetector) -> tuple[int, int]:
        # I'm unhappy with the requirement to store a detector specification to
        # work with fov settings.
        return detector.resolutions[self.fov]

    # ======================================================== #

    def validate(self, detector: TwinDetector, stage: XYZRange) -> tuple[bool, list[str]]:
        errors: list[str] = []

        # Validate stage positon
        _, stage_errors = super().validate(stage)
        errors += stage_errors

        # Validate detector fov
        if len(detector.resolutions) == 1 and self.fov != 0:
            errors.append(f"Detector only supports one resolution. FoV of '{self.fov}' must be 0.")

        elif self.fov < 0 or self.fov >= len(detector.resolutions):
            errors.append(f"Selected FoV of '{self.fov}' must be between 0 - {len(detector.resolutions)} inclusive")

        if len(detector.gain.gains) == 1 and self.gain != 0:
            errors.append(f"Detector does not support variable gain. Gain must be 0, not '{self.gain}'")

        elif self.gain < 0 or self.gain > len(detector.gain.gains):
            errors.append(f"Selected Gain of '{self.gain}' must be between 0 - {len(detector.gain.gains)} inclusive")

        if detector.exposures == []:
            if self.exposure <= 0:
                errors.append(f"Detector exposure of '{self.exposure}s' must be a positive number")
            else:
                pass

        elif self.exposure not in detector.exposures:
            errors.append(f"Selected exposure of '{self.exposure}' must be one of {detector.exposures}.")

        return len(errors) != 0, errors


# ======================================================== #


class DigitalTwin:
    specification: DigitalTwinSpecification

    _selectedbeam: str

    # Mutable settings
    _beam: TwinBeamSettings
    _detector: TwinDetectorSettings

    def __init__(self, specification: DigitalTwinSpecification) -> None:
        self.specification = specification

        default_beam_name = self.specification.beams.keys().__iter__().__next__()
        self.set_beam(default_beam_name)

        exposure = 0.5
        if len(specification.detector.exposures) > 0:
            exposure = specification.detector.exposures[0]

        self._detector = TwinDetectorSettings(
            _name="Detector",
            _position=specification.stage.detector.get_default_position(),
            fov=0,
            gain=0,
            exposure=exposure,
        )

    def __eq__(self, value:object) -> bool:
        if not isinstance(value, DigitalTwin):
            return False
        return self.__dict__ == value.__dict__

    @property
    def beam(self) -> TwinBeamSettings:
        return self._beam

    @property
    def source(self) -> TwinBeamSettings:
        return self._beam

    @property
    def detector(self) -> TwinDetectorSettings:
        return self._detector

    def set_beam(self, beam_name: str) -> bool:
        if beam_name not in self.specification.beams:
            return False

        self._selectedbeam = beam_name
        default_beam = self.specification.beams[self._selectedbeam]

        # Construct settings using default, middle of the range values
        default_kev = 0
        if isinstance(default_beam, (TwinBeamMonochromatic, TwinBeamTube)):
            default_kev = default_beam.keV[0] + ((default_beam.keV[1] - default_beam.keV[0]) / 2)

        default_ua = 0
        if isinstance(default_beam, TwinBeamTube):
            default_ua = default_beam.uA[0] + ((default_beam.uA[1] - default_beam.uA[0]) / 2)

        self._beam = TwinBeamSettings(
            _name="Beam",
            _kev=default_kev,
            _ua=default_ua,
            _position=self.specification.stage.source.get_default_position(),
            _filter=[],
        )

        return True

    def get_beams(self) -> list[str]:
        return list(self.specification.beams.keys())

    @property
    def sod(self) -> float:
        x, z, y = self.source.position
        return np.sqrt(x**2 + z**2 + y**2)

    @sod.setter
    def sod(self, value: float) -> None:
        x, y, z = self.source.position
        z = np.sqrt(value**2 - x**2 - y**2)

        self.source.position = [x, y, z]

    @property
    def odd(self) -> float:
        x, z, y = self.detector.position
        return np.sqrt(x**2 + z**2 + y**2)

    @odd.setter
    def odd(self, value: float) -> None:
        x, y, z = self.detector.position
        z = np.sqrt(value**2 - x**2 - y**2)

        self.detector.position = [x, y, z]

    @property
    def sdd(self) -> float:
        s = self.source.position
        d = self.detector.position
        return np.sqrt(((s[0] - d[0]) ** 2) + ((s[1] - d[1])**2) + ((-s[2] - d[2])**2))

    # ======================================================== #

    @staticmethod
    def from_file(path: Path | str) -> DigitalTwin:
        path = Path(path)
        with path.open("r") as f:
            return DigitalTwin(DigitalTwinSpecification.from_json(json5.load(f)))

    @staticmethod
    def from_json(json: dict) -> DigitalTwin:
        return DigitalTwin(DigitalTwinSpecification.from_json(json))

    # ======================================================== #

    def get_target_amperage(self, voltage: float, tube_amperage: float) -> float:
        """Convert user-requested tube amperage into actual amperage of the target based on a lookup table.

        Args:
            voltage (float): Voltage in kVp
            tube_amperage (float): Amperage in uA

        Returns:
            float: amperage of the target

        """
        beam = self.specification.beams[self._selectedbeam]

        if not isinstance(beam, TwinBeamTube) or beam.target_amperage_table.size == 0:
            return tube_amperage

        def t_amp(voltage: float, tube_amperage: float) -> float:
            return float(
                beam.target_amperage_table.loc[
                    (beam.target_amperage_table["voltage"] == voltage)
                    & (beam.target_amperage_table["amperage"] == tube_amperage),
                    "t_amp",
                ].iloc[0],
            )

        voltages = beam.target_amperage_table["voltage"].unique()
        amperages = beam.target_amperage_table["amperage"].unique()

        x0 = np.abs(voltages - voltage).argmin()
        y0 = np.abs(amperages - tube_amperage).argmin()

        x1 = x0 - 1 if voltages[x0] < voltage else x0 + 1 if x0 != len(voltages) - 1 else x0 - 1
        y1 = y0 - 1 if amperages[y0] < tube_amperage else y0 + 1 if y0 != len(amperages) - 1 else y0 - 1

        if x1 < x0:
            x0, x1 = x1, x0
        if y1 < y0:
            y0, y1 = y1, y0

        x0 = float(voltages[x0])
        x1 = float(voltages[x1])
        y0 = float(amperages[y0])
        y1 = float(amperages[y1])
        x = float(voltage)
        y = float(tube_amperage)

        # Blinear interpolation
        R1 = t_amp(x0, y0) * (x1 - x) / (x1 - x0) + (t_amp(x1, y0) * (x - x0) / (x1 - x0))
        R2 = t_amp(x0, y1) * (x1 - x) / (x1 - x0) + (t_amp(x1, y1) * (x - x0) / (x1 - x0))

        return (R1 * (y1 - y) / (y1 - y0)) + (R2 * (y - y0) / (y1 - y0))

    def _flux_cm2_1m(self) -> float:
        beam = self.specification.beams[self._selectedbeam]
        if isinstance(beam, TwinBeamFixedSpectrum):
            return sum(x[1] for x in beam.spectrum)

        if isinstance(beam, (TwinBeamMonochromatic, TwinBeamTube)):
            # Monochromatic source, only energy affects flux
            flux = float(np.polyval(beam.flux.curve, self.beam.kev) * (beam.flux.distance / 1000) ** 2)

            if isinstance(beam, TwinBeamTube):
                # Tube sources also take target amperage into account
                target_amperage = self.get_target_amperage(self.beam.kev, self.beam.ua)
                mA = target_amperage * 0.001
                flux = flux * mA

            return flux * self.detector.exposure

        # Unknown beam type and flux, default to 10000 per cm2
        return 10000

    # ======================================================== #

    def setup(self, renderer: Literal["OPENGL", "EGL"] = "OPENGL", major: int = 3, minor: int = 2) -> None:
        # gvxr init functions....
        gvxr.createWindow(-1, renderer, major, minor)

    def validate(self) -> tuple[bool, list[str]]:
        _, beam_errors = self.beam.validate(
            self.specification.beams[self._selectedbeam], self.specification.stage.source
        )
        _, detector_errors = self.detector.validate(self.specification.detector, self.specification.stage.detector)

        errors = beam_errors + detector_errors

        return len(errors) != 0, errors

    def apply(self) -> None:
        # validate
        error, errors = self.validate()
        errors = ["Incorrect Twin Settings:", *errors]
        if error:
            # Validation errors, throw a value error with the full list.
            raise ValueError("\n".join(errors))

        # Detector
        detector = self.specification.detector

        # Panel size
        gvxr.setDetectorPixelPitch(detector.pixel_pitch, detector.pixel_pitch, "um")
        gvxr.setDetectorNumberOfPixels(*self.detector.resolution(detector))

        # Scintillator
        if detector.scintillator.energy_response != []:
            gvxr.setDetectorEnergyResponse(detector.scintillator.energy_response, "keV")
        else:
            gvxr.setScintillator(detector.scintillator.material, detector.scintillator.thickness, "um")

        # Position
        gvxr.setDetectorUpVector(0, 1, 0)
        gvxr.setDetectorPosition(*self.detector.position, "mm")

        # Optical behaviour
        if detector.lsf != []:
            gvxr.setLSF(detector.lsf)

        # TODO gain <<<---------

        # Beam
        beam = self.specification.beams[self._selectedbeam]

        # Beam position
        gvxr.setSourcePosition(self.beam.position[0], self.beam.position[1], -self.beam.position[2], "mm")

        # Beam shape
        if beam.shape == PROJECTION.PARALLEL:
            gvxr.useParallelSource()
        elif beam.shape == PROJECTION.POINT:
            gvxr.usePointSource()
        else:
            raise NotImplementedError(f"Beam shape of {beam.shape} is unsupported.")

        # Beam spectrum
        gvxr.resetBeamSpectrum()

        if isinstance(beam, TwinBeamTube):
            gvxr.setmAs(self.detector.exposure * (self.beam.ua / 1000))
            gvxr.setVoltage(self.beam.kev, "keV")

        elif isinstance(beam, TwinBeamMonochromatic):
            gvxr.setMonoChromaticPerCm2At1m(self.beam.kev, "keV", 10000)

        elif isinstance(beam, TwinBeamFixedSpectrum):
            for energy, photons in beam.spectrum:
                gvxr.addEnergyBinToSpectrumPerCm2At1m(energy, "keV", photons)

        # Internal filtration (system characteristic).
        # Internal filtration affects the photon distribution, however
        # calibrated flux is for the adjusted distribution, therefore flux needs
        # to be set *after* we apply the filters.
        for f in beam.internal_filters:
            gvxr.addFilter(f[0], f[1], "cm")

        # Apply calibrated flux
        if isinstance(beam, (TwinBeamTube, TwinBeamMonochromatic)) and beam.flux.curve != []:
            # change to cm2 at 1m
            gvxr.setNumberOfPhotonsPerCm2At1m(self._flux_cm2_1m(), False)

        # External filtration (added by user)
        for f in self.beam.filter:
            print("ADD FILTERS")
            gvxr.addFilter(f[0], f[1], "cm")