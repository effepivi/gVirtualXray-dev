from skbuild import setup
import os

cmake_args = [
    '-DBUILD_TESTING:BOOL=OFF',
    '-DBUILD_SIMPLEGVXR:BOOL=ON',
    '-DBUILD_WRAPPER_PYTHON3:BOOL=ON',
    '-DCMAKE_BUILD_TYPE:STRING=Release',
    '-DUSE_SYSTEM_FREETYPE:BOOL=OFF',
    '-DUSE_SYSTEM_XRAYLIB:BOOL=OFF',
    '-DUSE_SYSTEM_GLFW:BOOL=OFF',
    '-DUSE_SYSTEM_ASSIMP:BOOL=OFF',
    '-DUSE_SYSTEM_GLEW:BOOL=OFF',
    '-DUSE_SYSTEM_ZLIB:BOOL=OFF',
    '-DUSE_SYSTEM_LIBTIFF:BOOL=OFF',
    '-DUSE_SYSTEM_FLTK:BOOL=OFF',
    '-DUSE_SYSTEM_GLUT:BOOL=OFF',
    '-DUSE_SYSTEM_VCG:BOOL=OFF',
    '-DUSE_SYSTEM_FFTW3:BOOL=OFF',
    '-DUSE_SYSTEM_GDCM:BOOL=OFF',
    '-DUSE_assimp:BOOL=ON',
    '-DUSE_FFTW3:BOOL=OFF',
    '-DUSE_FREETYPE:BOOL=OFF',
    '-DUSE_GDCM:BOOL=OFF',
    '-DUSE_LIBTIFF:BOOL=ON',
]

if os.name == 'nt':
    cmake_args.append('-DCMAKE_GENERATOR_PLATFORM:STRING=x64')

setup(name='gVXR',
    version='2.0.0',
    author="Dr Franck P. Vidal",
    #description="gVXR bindings, allowing to use some of the gVirtualXray (gVXR) library in python.",
    description="gVirtualXray (gVXR) Bindings to simulate realist X-ray attenuation images in microseconds from triangle meshes.",
    long_description="The gVXR Bindings project allows to use of the gVirtualXray (gVXR), the Virtual X-Ray Imaging Library on GPU with python. It allows the user to simulate realist X-ray attenuation images in microseconds from triangle meshes. This project is still experimental. For more information, please visit https://gvirtualxray.sourceforge.io/",
    url="https://gvirtualxray.sourceforge.io/",
    python_requires='>=3.6',
    packages=['gvxrPython3'],
    cmake_install_dir='Wrappers/gvxrPython3',
    package_dir={'': 'Wrappers'},
    package_data={"gvxrPython3": ["_gvxrPython3/*"]},
    cmake_install_target="install-python",
    cmake_args=cmake_args
)

    #package_data={'gvxrPython3': ['gvxrPython3/*.so', 'gvxrPython3/*.dll', 'gvxrPython3/*.py']},
    #package_dir={'gvxrPython3': 'gvxrWrapper/python3'},
