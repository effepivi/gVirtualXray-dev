import os, glob
from pathlib import Path
import json5

from ..twins import DigitalTwin

_twin_list = None

def getDigitalTwinList():
    """Accessor on the list of digital twins currently supported by gVXR.

    If you'd like your device to be listed, please contact the project maintainer.

    Raises
    ------
    IOError
        If two twins share the same name.

    Returns
    -------
    list
        a list of dictionaries
    """

    global _twin_list

    if not _twin_list:

        directory_path = os.path.dirname(__file__)

        _twin_list = []

        for fname in glob.glob(os.path.join(directory_path, "models/*.json5")):
            file_path = Path(fname)

            with file_path.open("r") as f:
                _twin_list.append(json5.load(f))

        # Make sure the name is new
        counter = 0
        for twin in _twin_list:
            if _twin_list[-1]["name"].upper() == twin["name"].upper():
                counter += 1

        # Two twins have the same name
        if counter != 1:
            raise IOError("Two twins have the same name (" + twin["name"].upper() + ")")

    return _twin_list


def createDigitalTwin(id: int=None, name: str=None):
    """Load a given digital twin and initialise gVXR.

    Parameters
    ----------
    id : int
        The index of the digital twin, as specified in the list provided by getDigitalTwinList().

    name : str
        The name of the digital twin, as specified in the list provided by getDigitalTwinList().
        Note that the case will be ignored.

    Raises
    ------
    ValueError
        If neither id and name are used.

    ValueError
        If both id and name are used.

    ValueError
        If the requested twin does not exist.

    Returns
    -------
    DigitalTwin
        a selected digital twin
    """

    # Test the validity of parameters
    if id is None and name is None:
        raise ValueError("Use either id or name to choose your digital twin")
    elif id is not None and name is not None:
        raise ValueError("Use either id or name to choose your digital twin, not both")

    # Make sure the list of twin exists
    getDigitalTwinList();

    # And ID is used
    if id:

        if id < len(_twin_list):
            return DigitalTwin.from_json(_twin_list[id])
        else:
            raise ValueError("The twin that you selected does not exist. Use `getDigitalTwinList()` to identify a suitable twin.")

    # A name is used
    else:

        for twin in _twin_list:
            if name.upper() == twin["name"].upper():
                return DigitalTwin.from_json(twin)

        # The twin was not found
        raise ValueError("The twin that you selected does not exist. Use `getDigitalTwinList()` to identify a suitable twin.")
