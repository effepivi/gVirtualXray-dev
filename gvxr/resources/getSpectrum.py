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


def getSpectrumSpekpy(kvp_in_kV, filters=None, th_in_deg=12):
    import numpy as np
    import spekpy as sp

    global has_spekpy

    k = []
    f = []


    # Spekpy is installed
    if has_spekpy:

        # Generate a spectrum
        s = sp.Spek(kvp=kvp_in_kV, th=th_in_deg)

        #Inherent filtration: 1.2mm Al + 100cm Air
        s.filter("Al", 1.2)
        # s.filter("air", 1000)

        # Additional filters
        # if filters is not None:
        #     for beam_filter in filters:
        #         filter_material = beam_filter[0]
        #         filter_thickness_in_mm = beam_filter[1]

        #         s.filter(filter_material, filter_thickness_in_mm)

        # Get the spectrum
        k, f = s.get_spectrum(edges=True)

        # Normalise the area under the curve
        area = 0.0
        prev_bin = None
        for energy, count in zip(k, f):
            if prev_bin is not None:
                area += count * (energy - prev_bin)
            prev_bin = energy

        f = np.array(f) / area


    return k.tolist(), f.tolist()


# def getSpekpySpectrum(kvp_in_kV, filters=None, th_in_deg=12):
#     return getSpectrumSpekpy(kvp_in_kV, filters, th_in_deg)


def getSpectrumXpecgen(kvp_in_kV, filters=None, th_in_deg=12):

    import numpy as np
    from xpecgen import xpecgen as xg

    global has_xpecgen

    k = []
    f = []

    # xpecgen is installed
    if has_xpecgen:

        # Generate a spectrum
        xrs = xg.calculate_spectrum(kvp_in_kV, th_in_deg, 1, 100, epsrel=0.5, monitor=None, z=74)

        #Inherent filtration: 1.2mm Al + 100cm Air
        mu_Al = xg.get_mu(13)
        xrs.attenuate(0.12,mu_Al)
        # xrs.attenuate(100,xg.get_mu("air"))

        # Additional filters
        if filters is not None:
            for beam_filter in filters:
                filter_material = beam_filter[0]
                filter_thickness_in_mm = beam_filter[1]

                if type(filter_material) == str:
                    mu = xg.get_mu(gvxr.getElementAtomicNumber(filter_material))
                else:
                    mu = xg.get_mu(filter_material)

                xrs.attenuate(filter_thickness_in_mm / 10, mu)

        # Get the spectrum
        (k,f) = xrs.get_points()

        # Normalise the area under the curve
        area = 0.0
        prev_bin = None
        for energy, count in zip(k, f):
            if prev_bin is not None:
                area += count * (energy - prev_bin)
            prev_bin = energy

        f = np.array(f) / area

    return k.tolist(), f.tolist()


def getXpecgenSpectrum(kvp_in_kV, filters=None, th_in_deg=12):
    return getSpectrumXpecgen(kvp_in_kV, filters, th_in_deg)
