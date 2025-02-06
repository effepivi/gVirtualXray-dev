from skbuild import setup
import os


# Use conan
# subprocess.run(["conan", "install", ".", "--output-folder=/tmp/build", "--build=missing"]) 
# '-DCMAKE_TOOLCHAIN_FILE=/tmp/build/conan_toolchain.cmake',

cmake_args = [
    '-DBUILD_TESTING:BOOL=OFF',
    '-DBUILD_SIMPLEGVXR:BOOL=ON',
    '-DBUILD_WRAPPER_PYTHON3:BOOL=ON',
    '-DBUILD_SimpleGVXR_DOCUMENTATION:BOOL=ON',
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
    cmake_args.append('-DCMAKE_BUILD_PARALLEL_LEVEL:INT=20')
    cmake_args.append('-DGSL_INCLUDE_DIR:PATH=C:/Users/res52584/PROGRAMMING/GitHub/vcpkg/installed/x64-windows/include')
    cmake_args.append('-DGSL_CBLAS_LIBRARY:PATH=C:/Users/res52584/PROGRAMMING/GitHub/vcpkg/installed/x64-windows/lib/gslcblas.lib')
    cmake_args.append('-DGSL_CBLAS_LIBRARY_DEBUG:PATH=C:/Users/res52584/PROGRAMMING/GitHub/vcpkg/installed/x64-windows/lib/gslcblas.lib')
    cmake_args.append('-DGSL_LIBRARY:PATH=C:/Users/res52584/PROGRAMMING/GitHub/vcpkg/installed/x64-windows/lib/gsl.lib')
    cmake_args.append('-DGSL_LIBRARY_DEBUG:PATH=C:/Users/res52584/PROGRAMMING/GitHub/vcpkg/installed/x64-windows/lib/gsl.lib')
     
# GNU/Linux or MacOS
elif os.name == 'posix':
    # Not MacOS
    if str(os.uname()).find("Darwin") < 0:
        cmake_args.append('-DCMAKE_EXE_LINKER_FLAGS:STRING=-lrt')
        cmake_args.append('-DCMAKE_MODULE_LINKER_FLAGS:STRING=-lrt')
        cmake_args.append('-DCMAKE_BUILD_PARALLEL_LEVEL:INT=24')


setup(name='gvxr',
    version='2.0.9',
    author="Dr Franck P. Vidal",
    #description="gVXR bindings, allowing to use some of the gVirtualXray (gVXR) library in python.",
    description="gVirtualXray (gVXR) Bindings to simulate realist X-ray attenuation images in microseconds from triangle meshes.",
    long_description="The gVXR Bindings project allows to use of the gVirtualXray (gVXR), the Virtual X-Ray Imaging Library on GPU with python. It allows the user to simulate realist X-ray attenuation images in microseconds from triangle meshes. This project is still experimental. For more information, please visit https://gvirtualxray.sourceforge.io/",
    long_description_content_type="text/x-rst",    
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
