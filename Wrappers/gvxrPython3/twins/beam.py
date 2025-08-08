from __future__ import annotations

from dataclasses import dataclass
from enum import Enum, unique
from typing import Literal

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

    def to_json(self) -> dict:
        return self.__dict__.copy()

    @staticmethod
    def from_json(json: dict) -> TwinFlux:
        curve = list(json["curve"])
        distance = float(json["distance"])
        return TwinFlux(curve=curve, distance=distance)


# ======================================================== #


@dataclass(kw_only=True)
class TwinBeam:
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


@dataclass(kw_only=True)
class TwinBeamMonochromatic(TwinBeam):
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
            keV=keV, flux=flux, shape=shape, filters=filters, internal_filters=internal_filters
        )


@dataclass(kw_only=True)
class TwinBeamTube(TwinBeam):
    beam_type: Literal["tube"] = "tube"
    keV: tuple[float, float]
    uA: tuple[float, float]
    flux: TwinFlux
    target_amperage_table: pd.DataFrame

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
            keV=keV, uA=uA, flux=flux, target_amperage_table=target_amperage_table, **twin_beam.to_json()
        )

    def __eq__(self, other:object):
        if not isinstance(other, TwinBeamTube):
            return False
        # Compare in the exported json domain due to internal pandas variables
        # prevent easy array comparisons.
        return self.to_json() == other.to_json()


@dataclass(kw_only=True)
class TwinBeamFixedSpectrum(TwinBeam):
    beam_type: Literal["fixed-spectrum"] = "fixed-spectrum"
    spectrum: list[tuple[float, float]]

    def to_json(self) -> dict:
        return self.__dict__.copy()

    @staticmethod
    def from_json(json: dict) -> TwinBeamFixedSpectrum:
        twin_beam = TwinBeam.from_json(json)
        spectrum = list(json["spectrum"])
        return TwinBeamFixedSpectrum(**twin_beam.to_json(), spectrum=spectrum)


def beam_from_json(json: dict) -> TwinBeam:
    if "beam_type" not in json:
        raise KeyError(
            "Beam requires a 'beam_type' property. Supported types: ['monochromatic', 'tube', 'fixed-spectrum']"
        )

    if json["beam_type"] == "monochromatic":
        return TwinBeamMonochromatic.from_json(json)

    if json["beam_type"] == "fixed-spectrum":
        return TwinBeamFixedSpectrum.from_json(json)

    if json["beam_type"] == "tube":
        return TwinBeamTube.from_json(json)

    raise KeyError(
        f"Unknown beam type '{json['beam_type']}'. Supported types: ['monochromatic', 'tube', 'fixed-spectrum']"
    )
