from __future__ import annotations

from pathlib import Path

from .digital_twin import DigitalTwin

# def twin_from_file(file: Path) -> DigitalTwin:
#     twin = None
#     with file.open("r") as f:
#         twin = DigitalTwin.from_json(json5.load(f))
#     return twin

if __name__ == "__main__":
    # dthe_twin = twin_from_file(Path("./dthe.json5"))
    dthe_twin = DigitalTwin.from_file("./dthe.json5")
    print(dthe_twin)

    # Get beams as a list, and change beams
    beams = dthe_twin.get_beams()

    dthe_twin.set_beam(beams[1])
    dthe_twin.set_beam("X-ray 1")
    dthe_twin.set_beam("X-ray 2")
    dthe_twin.set_beam(beams[0])

    # class-based modelling for setup and stage position
    dthe_twin.beam.kev = 60
    dthe_twin.beam.ua = 130
    dthe_twin.beam.filter = ["Al", 1.5]
    dthe_twin.beam.filter = [["Al", 1], ["Cu", 1]]

    # or use beam as "source"
    dthe_twin.source.kev = 80
    dthe_twin.source.position = [0, 0, 10, "mm"] #mm
    dthe_twin.source.position = [0, 10, 10, "mm"] #mm
    dthe_twin.source.position = [10, "mm"] # sod #mm


    # unfortunately contains twin detector specifications#
    # e.g scintillator, etc
    dthe_twin.detector.exposure = 1
    dthe_twin.detector.resolutions
    dthe_twin.detector.scintillator

    dthe_twin.detector.exposure = 1

    # dthe_twin.beams => twin beams definition
    # dthe_twin.detector => twin detector definition -> in config

    # dthe_twin.beam => user-set settings
    # dthe_twin.source => user-set settings
    # dthe_twin.detector => user-set settings !!!
    # dthe_twin.panel => user-set settings !!!


    # move stage params onto beam / source
    dthe_twin.source.filter = ["Al", 1]
    dthe_twin.source.filter = [["Al", 1], ["Cu", 3]]
    dthe_twin.source.position = [0, 0, 10]
    dthe_twin.detector.position = [10, "mm"]

    # Helper functions to move stage
    dthe_twin.sdd = [1000, "mm"]
    dthe_twin.sod = [400, "mm"]
    dthe_twin.odd = 600

    # ------

    # apply twin parameters with .apply()
    dthe_twin.apply()

    # gvxr functions for simulating images, gain to be moved into gvxr
    # gvxr. ...

    diad_twin = DigitalTwin.from_file("./diad.json5")

    print(diad_twin)

def getDigitalTwinList() -> list[str]: ...

def getDigitalTwin(str:list) -> list[str]:
    return DigitalTwin().from_file("...")

from gvxrPython3 import twins



# -----
from gvxrPython3.twins import DigtalTwin, getDigitalTwin

twin = DigtalTwin.from_file("dthe.json")
twin = getDigitalTwin("dthe")
twin.beam.kev = 90
