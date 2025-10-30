from __future__ import annotations

from dataclasses import dataclass
from enum import Enum, unique
from typing import Literal

import numpy as np
import pandas as pd

# ======================================================== #


@unique
class PROJECTION(str, Enum):
    """Represents a beam projection type.

    Enums:
        POINT: A projection from a point source
        PARALLEL: A uniform parallel projection
    """

    POINT = "point"
    PARALLEL = "parallel"


@dataclass(kw_only=True)
class TwinFlux:
    """Representes calibrated digital twin flux for representative noise of a beam-detector pair, linked to voltage or energy."""

    # Twin flux curve parameters for use in np.polyval at 1mas per [cm2] at distance [mm]
    curve: list[float]

    # Distance in mm of curve, standard units would be 1000 for flux @ 1m.
    distance: float

    @property
    def is_calibrated(self) -> bool:
        """Denotes flux modelling is calibrated."""
        return self.curve != []

    def to_json(self) -> dict:
        return self.__dict__.copy()

    @staticmethod
    def from_json(json: dict) -> TwinFlux:
        curve = list(json["curve"])
        distance = float(json["distance"])
        return TwinFlux(curve=curve, distance=distance)

    def __str__(self) -> str:
        if self.is_calibrated:
            return (
                f"Calibrated Flux: True ({np.polyval(self.curve, 1000):.2f} photons per cm2 @ 1mAs @ {self.distance}mm)"
            )
        return "Calibrated Flux: False"


# ======================================================== #


@dataclass(kw_only=True)
class TwinBeam:
    """Base class representing an X-ray source, includes projection type and beam filteration."""

    shape: PROJECTION
    filters: list[list[str | float]]
    internal_filters: list[list[str | float]]

    def to_json(self) -> dict:
        return self.__dict__.copy()

    @staticmethod
    def from_json(json: dict) -> TwinBeam:
        shape = PROJECTION(json["shape"])
        filters = list(json["filters"])
        internal_filters = list(json["inherent_filteration"]) if "inherent_filteration" in json else []
        return TwinBeam(shape=shape, filters=filters, internal_filters=internal_filters)

    def __repr__(self) -> str:
        return str(self.to_json())

    def __str__(self) -> str:
        return f"shape: {self.shape.value}, internal_filters: {self.internal_filters}, filters: {self.filters}"


@dataclass(kw_only=True)
class TwinBeamMonochromatic(TwinBeam):
    """A monochromatic (singular energy) beam source, commonly found in synchrotrons.

    May include calibrated flux.
    """

    beam_type: Literal["monochromatic"] = "monochromatic"
    keV: tuple[float, float]
    flux: TwinFlux

    def to_json(self) -> dict:
        return self.__dict__.copy()

    @staticmethod
    def from_json(json: dict) -> TwinBeamMonochromatic:
        flux: TwinFlux = TwinFlux.from_json(json["flux"])
        keV = float(json["keV"][0]), float(json["keV"][1])
        shape = json["shape"]

        # No filtration for monochromatic source
        filters = []
        internal_filters = []

        return TwinBeamMonochromatic(
            keV=keV,
            flux=flux,
            shape=shape,
            filters=filters,
            internal_filters=internal_filters,
        )

    def __str__(self) -> str:
        return f"Monochromatic beam: ({self.keV[0]} - {self.keV[1]}keV), {self.flux}, {TwinBeam.__str__(self)}"

    def __repr__(self) -> str:
        return str(self.to_json())


@dataclass(kw_only=True)
class TwinBeamTube(TwinBeam):
    """A polychromatic X-ray tube source, commonly found in Lab-CT setups.

    Controlled with cathode (tube) voltage (kV) and amperage (uA)

    May include calibrated flux.
    May include calibrated cathode (tube) to anode (target) amperage.
    """

    beam_type: Literal["tube"] = "tube"
    keV: tuple[float, float]
    uA: tuple[float, float]
    flux: TwinFlux
    target_amperage_table: pd.DataFrame

    @property
    def kV(self) -> tuple[float, float]:
        return self.keV

    @property
    def amperage_calibrated(self) -> bool:
        """Denotes the anode amperage is calibrated."""
        return self.target_amperage_table.size != 0

    def get_target_amperage(self, voltage: float, tube_amperage: float) -> float:
        """Convert user-requested tube amperage into actual amperage of the target based on a lookup table.

        Args:
            voltage (float): Voltage in kVp
            tube_amperage (float): Amperage in uA

        Returns:
            float: amperage of the target in uA

        """
        if not self.amperage_calibrated:
            return tube_amperage

        def t_amp(voltage: float, tube_amperage: float) -> float:
            return float(
                self.target_amperage_table.loc[
                    (self.target_amperage_table["voltage"] == voltage)
                    & (self.target_amperage_table["amperage"] == tube_amperage),
                    "t_amp",
                ].iloc[0],
            )

        voltages = self.target_amperage_table["voltage"].unique()
        amperages = self.target_amperage_table["amperage"].unique()

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

    def to_json(self) -> dict:
        dictionary = self.__dict__.copy()
        dictionary["target_amperage_table"] = self.target_amperage_table.to_numpy().tolist()
        return dictionary

    @staticmethod
    def from_json(json: dict) -> TwinBeamTube:
        twin_beam: TwinBeam = TwinBeam.from_json(json)

        keV = tuple(json["keV"])
        uA = tuple(json["uA"])
        flux: TwinFlux = TwinFlux.from_json(json["flux"])

        target_amperage_table = pd.DataFrame(
            columns=["voltage", "amperage", "t_amp"],
            data=list(json["target_amperage_table"]),
        )

        return TwinBeamTube(
            keV=keV,
            uA=uA,
            flux=flux,
            target_amperage_table=target_amperage_table,
            **twin_beam.to_json(),
        )

    def __repr__(self) -> str:
        return str(self.to_json())

    def __str__(self) -> str:
        ua_calibration = (
            f" ({self.get_target_amperage(self.keV[1], self.uA[1])}uA @ {self.keV[1]}kV @ {self.uA[1]}uA)"
            if self.amperage_calibrated
            else ""
        )
        return f"Tube beam: {self.keV[0]}kV - {self.keV[1]}kV, {self.uA[0]}uA - {self.uA[1]}uA{ua_calibration}, {self.flux}, {TwinBeam.__str__(self)}"

    def __eq__(self, other: object) -> bool:
        if not isinstance(other, TwinBeamTube):
            return False
        # Compare in the exported json domain due to internal pandas variables
        # prevent easy array comparisons.
        return self.to_json() == other.to_json()


@dataclass(kw_only=True)
class TwinBeamFixedSpectrum(TwinBeam):
    """An X-ray source with a fixed emission spectra. Common in pulse sources and synchrotrons."""

    beam_type: Literal["fixed-spectrum"] = "fixed-spectrum"
    spectrum: list[tuple[float, float]]

    def to_json(self) -> dict:
        return self.__dict__.copy()

    @staticmethod
    def from_json(json: dict) -> TwinBeamFixedSpectrum:
        twin_beam = TwinBeam.from_json(json)
        spectrum = list(json["spectrum"])
        return TwinBeamFixedSpectrum(**twin_beam.to_json(), spectrum=spectrum)

    def __repr__(self) -> str:
        return str(self.to_json())

    def __str__(self) -> str:
        spectra = np.asarray(self.spectrum)
        peak = spectra[spectra.argmax(axis=1)[1]]
        return f"Fixed Spectrum Source: ({self.spectrum[0][0]}keV - {self.spectrum[-1][0]}keV (Peak at {peak[0][0]}keV, {peak[0][1]} photons cm2), {TwinBeam.__str__(self)}"


def beam_from_json(json: dict) -> TwinBeam:
    """Extract a beam object from a digital twin configuration file."""
    if "beam_type" not in json:
        raise KeyError(
            "Beam requires a 'beam_type' property. Supported types: ['monochromatic', 'tube', 'fixed-spectrum']",
        )

    if json["beam_type"] == "monochromatic":
        return TwinBeamMonochromatic.from_json(json)

    if json["beam_type"] == "fixed-spectrum":
        return TwinBeamFixedSpectrum.from_json(json)

    if json["beam_type"] == "tube":
        return TwinBeamTube.from_json(json)

    raise KeyError(
        f"Unknown beam type '{json['beam_type']}'. Supported types: ['monochromatic', 'tube', 'fixed-spectrum']",
    )
