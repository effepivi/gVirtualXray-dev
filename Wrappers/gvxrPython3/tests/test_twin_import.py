import pytest

from twins import DigitalTwin
from twins.utils import createDigitalTwin


def test_beam_load_tube() -> None:
    DigitalTwin.from_file("twins/models/dthe.json5")


def test_beam_load_synch_multibeam() -> None:
    DigitalTwin.from_file("twins/models/diad.json5")


def test_twin_validate() -> None:
    diad = DigitalTwin.from_file("twins/models/diad.json5")

    error, errors = diad.validate()
    assert error is False


def test_twin_invalid() -> None:
    diad = DigitalTwin.from_file("twins/models/diad.json5")
    diad.beam.kev = 1000
    error, errors = diad.validate()

    assert error is True

def test_twin_diad_sod_sdd() -> None:
    # Simple diad twin test
    diad = DigitalTwin.from_file("twins/models/diad.json5")

    diad.sod = 10
    diad.odd = 20

def test_create_digital_twin_utils() -> None:
    diad1 = createDigitalTwin(name="DIAD")
    diad2 = createDigitalTwin(id=1)
    diad3 = DigitalTwin.from_file("twins/models/diad.json5")

    assert diad1 == diad2
    assert diad1 == diad3

    dthe1 = createDigitalTwin("dThE")
    dthe2 = createDigitalTwin(id=1)
    dthe3 = DigitalTwin.from_file("twins/models/dthe.json5")

    assert dthe1 == dthe2
    assert dthe1 == dthe3

    assert diad1 != dthe1
