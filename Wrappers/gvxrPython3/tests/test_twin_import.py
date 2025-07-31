import pytest

from twins import DigitalTwin


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


def test_twin_diad_apply() -> None:
    # Simple diad twin test
    diad = DigitalTwin.from_file("twins/models/diad.json5")

    diad.beam.kev = 14
    diad.detector.exposure = 0.5

    diad.apply()


def test_twin_diad_apply_invalid() -> None:
    # Simple diad twin test
    diad = DigitalTwin.from_file("twins/models/diad.json5")

    diad.beam.kev = 180
    diad.detector.exposure = -10

    with pytest.raises(ValueError, match="Incorrect Twin Settings"):
        diad.apply()

def test_twin_diad_sod_sdd() -> None:
    # Simple diad twin test
    diad = DigitalTwin.from_file("twins/models/diad.json5")

    diad.sod = 10
    diad.odd = 20
