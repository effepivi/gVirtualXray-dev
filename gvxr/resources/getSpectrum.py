has_spekpy = False
has_xpecgen = False

try:
    import spekpy as sp
    has_spekpy = True
except:
    # print("spekpy is not install, you won't be able to load a beam spectrum using spekpy")
    has_spekpy = False

try:
    from xpecgen import xpecgen as xg
    has_xpecgen = True
except:
    # print("xpecgen is not install, you won't be able to load a beam spectrum using xpecgen")
    has_xpecgen = False


def hasSpekpy():
    global has_spekpy
    return has_spekpy

def hasXpecgen():
    global has_xpecgen
    return has_xpecgen
