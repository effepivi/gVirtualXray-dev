from ._gvxrPython3 import temp_gvxrPython3 as gvxr
from ._version import __version__

# The subpackages are deliberately not imported here. Each of them needs
# dependencies that are not required to simulate X-ray images, and importing
# gvxrPython3 must keep working when they are missing. Import them explicitly:
#
#     from gvxrPython3.notebook import Viewer     # needs anywidget
#     from gvxrPython3.cil import gVXRDataReader  # needs the CIL framework
#
# Note that notebook imports gvxr from here, so the two imports above must stay
# at the top of this file.

__all__ = ["gvxr", "__version__"]
