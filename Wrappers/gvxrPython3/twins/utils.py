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
    
    return _twin_list


def createDigitalTwin(id: int):
    """Load a given digital twin and initialise gVXR.

    Parameters
    ----------
    id : int
        The index of the digital twin, as specified in the list provided by getDigitalTwinList()

    """

    return DigitalTwin.from_json(getDigitalTwinList()[id])