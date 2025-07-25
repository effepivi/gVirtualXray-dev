from __future__ import annotations

from collections.abc import Sequence
from dataclasses import dataclass
from pathlib import Path
from typing import cast

import json5
import numpy as np

from .beam import TwinBeam, TwinBeamMonochromatic, TwinBeamTube, beam_from_json
from .detector import TwinDetector
from .stage import TwinStage

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
        return self.__dict__


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
                value = float(value[0])
            elif len(value) == 3:
                self._position = (float(value[0]), float(value[1]), float(value[2]))
            else:
                raise ValueError("Position must be a float [z], or list of floats in [x, y, z] format.")

        if isinstance(value, float):
            # Single float value, assume position on beam axis
            self._position = (0, 0, value)

    # ======================================================== #

    def validate(self, stage: TwinStage) -> tuple[bool, list[str]]:
        errors = []

        if stage.source.x != []:
            # Validate x axis
            if stage.source.x == [0, 0]:
                if self.position[0] != 0:
                    errors.append(f"{self._name} does not support x movement. '{self.position[0]}'mm is out of bounds")

            elif self.position[0] < stage.source.x[0] or self.position[0] > stage.source.x[1]:
                errors.append(
                    f"{self._name} X position '{self.position[0]}'mm must be between {stage.source.x[0]}mm - {stage.source.x[1]}mm",
                )

        if stage.source.y != []:
            # Validate y axis
            if stage.source.y == [0, 0]:
                if self.position[1] != 0:
                    errors.append(f"{self._name} does not support y movement. '{self.position[1]}'mm is out of bounds")

            elif self.position[1] < stage.source.y[0] or self.position[0] > stage.source.y[1]:
                errors.append(
                    f"{self._name} Y position '{self.position[1]}'mm must be between {stage.source.y[0]}mm - {stage.source.y[1]}mm",
                )

        if stage.source.z != []:
            # Validate z axis
            if stage.source.x == [0, 0]:
                if self.position[2]:
                    errors.append(f"{self._name} does not support x movement. '{self.position[2]}'mm is out of bounds")

            elif self.position[2] < stage.source.z[0] or self.position[2] > stage.source.z[1]:
                errors.append(
                    f"{self._name} Z position '{self.position[2]}'mm must be between {stage.source.z[0]}mm - {stage.source.z[1]}mm",
                )

        return len(errors) == 0, errors


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

    def validate(self, beam: TwinBeam, stage: TwinStage) -> tuple[bool, list[str]]:
        errors = []

        if beam.filters != []:
            # Validate filters
            raise RuntimeWarning("Beam filter validation is not implemented")

        if isinstance(beam, (TwinBeamTube, TwinBeamMonochromatic)):
            if self.kev < beam.keV[0] or self.kev > beam.keV[1]:
                errors.append(f"keV of '{self.kev}' must be between {beam.keV[0]}kV - {beam.keV[1]}kV")

        if isinstance(beam, TwinBeamTube):
            if self.ua < beam.uA[0] or self.ua > beam.uA[1]:
                errors.append(f"Beam uA of '{self.ua}'μA must be between {beam.uA[0]}μA - {beam.uA[1]}μA")

        # Validate beam positon
        _, stage_errors = super().validate(stage)
        errors += stage_errors

        return len(errors) == 0, errors


@dataclass(kw_only=True)
class TwinDetectorSettings(Positional):
    _exposure: float
    _fov: int

    # ======================================================== #

    # ======================================================== #

    def validate(self, detector: TwinDetector, stage: TwinStage) -> tuple[bool, list[str]]:
        errors = []

        # Validate stage positon
        _, stage_errors = super().validate(stage)
        errors += stage_errors

        return len(errors) == 0, errors


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

    def setup(self) -> None:
        # gvxr init functions....
        ...

    def validate(self) -> tuple[bool, list[str]]:
        _, beam_errors = self.beam.validate(self.specification.beams[self._selectedbeam], self.specification.stage)
        _, detector_errors = self.detector.validate(self.specification.detector, self.specification.stage)

        errors = beam_errors + detector_errors

        return len(errors) == 0, errors

    def apply(self) -> None:
        # validate
        error, errors = self.validate()
        if error:
            # Validation errors, throw a value error with the full list.
            raise ValueError("\n".join(errors))
