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

    # class-based modelling for setup and stage position
    dthe_twin.beam.kev = 60

    dthe_twin.panel.resolution = 1
    dthe_twin.panel.exposure = 0.2

    dthe_twin.exposure = 2

    dthe_twin.stage.detector.position = [0, 10]
    dthe_twin.source.position = [0, 10]

    # ?
    # dthe_twin.beam = dthe_twin.source

    # move stage params onto beam / source
    dthe_twin.source.filter = ["Al", 1]
    dthe_twin.source.filter = [["Al", 1], ["Cu", 3]]
    dthe_twin.source.position = [0, 0, 10]
    dthe_twin.panel.position = [0, 10]

    # Helper functions to move stage
    # dthe_twin.sdd = ...
    # dthe_twin.sod = ...
    # dthe_twin.odd = ...

    # ------

    # apply twin parameters with .apply()
    dthe_twin.apply()

    # gvxr functions for simulating images, gain to be moved into gvxr
    # gvxr. ...

    diad_twin = DigitalTwin.from_file("./diad.json5")

    print(diad_twin)
