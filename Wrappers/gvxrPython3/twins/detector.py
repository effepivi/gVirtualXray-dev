from __future__ import annotations

from dataclasses import dataclass


@dataclass(kw_only=True)
class TwinScintillator:
    material: str
    thickness: float
    energy_response: list[tuple[float, float]] | None

    def to_json(self) -> dict:
        return self.__dict__

    @staticmethod
    def from_json(json: dict) -> TwinScintillator:
        material = json["material"]
        thickness = float(json["thickness"])
        energy_response = None
        if "energy_response" in json:
            energy_response = list(json["energy_response"])

        return TwinScintillator(
            material=material, thickness=thickness, energy_response=energy_response,
        )


@dataclass(kw_only=True)
class TwinGain:
    k: float
    gains: list[float]

    def to_json(self) -> dict:
        return self.__dict__

    @staticmethod
    def from_json(json: dict) -> TwinGain:
        k = float(json["k"])
        gains = list(json["gains"])

        return TwinGain(k=k, gains=gains)


@dataclass(kw_only=True)
class TwinDetector:
    exposures: list[float]
    resolutions: list[tuple[int, int]]
    pixel_pitch: float
    lsf: list[float]
    scintillator: TwinScintillator
    gain: TwinGain

    def to_json(self) -> dict:
        return self.__dict__

    @staticmethod
    def from_json(json: dict) -> TwinDetector:
        exposures = list(json["exposures"])
        resolutions = list(json["resolutions"])
        pixel_pitch = float(json["pixel_pitch"])
        lsf = list(json["lsf"])
        scintillator = TwinScintillator.from_json(json["scintillator"])
        gain = TwinGain.from_json(json["gain"])

        return TwinDetector(
            exposures=exposures,
            resolutions=resolutions,
            pixel_pitch=pixel_pitch,
            lsf=lsf,
            scintillator=scintillator,
            gain=gain,
        )
