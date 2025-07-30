import pytest

from ..twins import DigitalTwin


def test_beam_load() -> None:
    DigitalTwin.from_file("twins/models/diad.dthe")


def test_twin_validate() -> None:
    diad = DigitalTwin.from_file("twins/models/diad.dthe")

    error, errors = diad.validate()
    assert error is False


def test_twin_invalid() -> None:
    diad = DigitalTwin.from_file("twins/models/diad.dthe")
    diad.beam.kev = 1000
    error, errors = diad.validate()

    assert error is True


def test_twin_diad_apply() -> None:
    # Simple diad twin test
    diad = DigitalTwin.from_file("twins/models/diad.dthe")

    diad.beam.kev = 14
    diad.detector.exposure = 0.5

    diad.apply()


def test_twin_diad_apply_invalid() -> None:
    # Simple diad twin test
    diad = DigitalTwin.from_file("twins/models/diad.dthe")

    diad.beam.kev = 180
    diad.detector.exposure = -10

    with pytest.raises(ValueError, match="Incorrect Twin Settings"):
        diad.apply()
