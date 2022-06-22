import os
import pathlib
import glob
import shutil

import sys
# import pysvn

import urllib.request
import shutil

from distutils.core import setup

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext as build_ext_orig
from distutils.command.build_py import build_py as build_py_orig
from distutils.command.install import install
from distutils.command.install_lib import install_lib
from distutils.command.build import build
from wheel.bdist_wheel import bdist_wheel

# URL="svn://svn.code.sf.net/p/gvirtualxray/code/branches/use-xraylib"
FNAME="gvirtualxray-code-r2183-branches-use-xraylib"
URL="https://sourceforge.net/code-snapshots/svn/g/gv/gvirtualxray/code/" + FNAME + ".zip"
TARGET_DIRECTORY="gvirtualxray-src"

#
# dependencies_list = []
#
# #find all dependencies dll in the PATH and put them in a map {dll file : full path to dll path}
# if sys.platform == 'win32'or sys.platform == 'cygwin':
#   env_path_content = os.getenv('PATH')
#   all_dll = {}
#   for p in env_path_content.split(";"):
#     p_rep=p.replace("\"","")
#     files=[]
#     if os.path.isdir(p_rep):
#       files=os.listdir(p_rep)
#     for f in files:
#       if '.dll' in f:
#         #truncate f:
#         for dep in dependencies_list:
#           if dep in f:
#             all_dll[f] = p_rep+"\\"+f
#
#   all_keys=""
#   for key in all_dll:
#     all_keys+=(key+";")
#
#   for dep in dependencies_list:
#     if not dep in all_keys:
#       print("ERROR: Missing dependency: "+dep)
#
#
# gVXR_modules = [
# ]
#
#
# extensions = []
#
#
# #collect gVXR modules
# for mod_name in gVXR_modules:
#     e = Extension('gVXR/_gVXR_'+mod_name,
#                   sources=[])
#     extensions.append(e)
#
#
def get_options():
    return [
        ('gVXR_dir=', None, 'Specify the path to the gVXR installation directory.'),
        ('enable_ASSIMP=', None, 'Use the Open-Asset-Importer-Lib (ASSIMP) to load 3D meshes.'),
        ('enable_OpenSceneGraph=', None, 'Use the OpenSceneGraph to load 3D meshes.'),
        ('enable_ITK=', None, 'Use ITK.'),
        ('enable_FFTW3=', None, 'Use FFTW3.'),
        ('enable_FreeType=', None, 'Use FreeType.'),
        ('enable_GDCM=', None, 'Use GDCM (Grassroots DICOM library) to save DICOM files.'),
        ('enable_LibTIFF=', None, 'Use LibTIFF.'),
        ('cmake_prefix_path=', None, 'Specify the path to a directory that can be used as CMAKE_PREFIX_PATH, that would contain all headers and libraries. '),
        ('generator=', None, 'The generator to use for cmake.'),
        ('python_executable=', None, 'The path to the python executable.'),
        ('python_root=', None, 'The path to the python root directory.'),
        ('cmake=', None, 'Specify the path to the cmake executable.')
    ]


def get_option_pairs():
    values ={
        ('gVXR_dir', 'gVXR_dir'),
        ('enable_ASSIMP', 'enable_ASSIMP'),
        ('enable_OpenSceneGraph', 'enable_OpenSceneGraph'),
        ('enable_ITK', 'enable_ITK'),
        ('enable_FFTW3', 'enable_FFTW3'),
        ('enable_FreeType', 'enable_FreeType'),
        ('enable_GDCM', 'enable_GDCM'),
        ('enable_LibTIFF', 'enable_LibTIFF'),
        ('cmake_prefix_path','cmake_prefix_path'),
        ('generator', 'generator'),
        ('python_root', 'python_root'),
        ('python_executable', 'python_executable'),
        ('cmake', 'cmake')
    }

    return values


def init_values(obj):
    obj.gVXR_dir = None
    obj.enable_ASSIMP = None
    obj.enable_OpenSceneGraph = None
    obj.enable_ITK = None
    obj.enable_FFTW3 = None
    obj.enable_FreeType = None
    obj.enable_GDCM = None
    obj.enable_LibTIFF = None
    obj.cmake_prefix_path = None
    obj.generator = None
    obj.cmake = None
    obj.python_root = None
    obj.python_executable = None


def getSourcePath():
    return str(pathlib.Path.cwd()) + "/build/" + TARGET_DIRECTORY + "/" + FNAME

def getBinaryPath():
    return str(pathlib.Path.cwd()) + "/build/bin"

def checkoutSource():
    print("Get the source code from SourceForge (" + URL + ")") #svn checkout --username=zedbluffer svn+ssh://zedbluffer@svn.code.sf.net/p/gvirtualxray/code/ gvirtualxray-code

    # client = pysvn.Client()
    # client.checkout(URL, "build/" + TARGET_DIRECTORY)

    urllib.request.urlretrieve(URL, "temp.zip")

    # os.path.join(os.environ['BACKUP_DIR'], f'Backup_{self.batch_id}.tar.gz')
    # print("CWD " + pathlib.Path.cwd())

    print("The source is in: " + getSourcePath())
    # print("Extract the archive in ", pathlib.Path().absolute(), "/build/")
    shutil.unpack_archive(filename="temp.zip",
        extract_dir="build/" + TARGET_DIRECTORY,
        format="zip")


# class BuildWheelCommand(bdist_wheel):
#     user_options = bdist_wheel.user_options + get_options()
#
#
#     def initialize_options(self):
#         bdist_wheel.initialize_options(self)
#         init_values(self)
#
#
#     def run(self):
#         build_ext = self.reinitialize_command('build_ext',
#             reinit_subcommands=True)
#         path=self.bdist_dir
#         path=path.replace("build/", "")
#         path=path.replace("build\\", "")
#         setattr(build_ext, 'install_dir', path)
#         bdist_wheel.run(self)
#
#
# class InstallCommand(install):
#     user_options = install.user_options + get_options()
#     def initialize_options(self):
#         install.initialize_options(self)
#         init_values(self)




#
#
#
# class InstallLibCommand(install_lib):
#   user_options = install_lib.user_options + get_options()
#
#   def initialize_options(self):
#     install_lib.initialize_options(self)
#     init_values(self)
#
#   def finalize_options(self):
#     install_lib.finalize_options(self)
#     self.set_undefined_options('install', *get_option_pairs())
#     self.set_undefined_options('bdist_wheel', *get_option_pairs())
#
#
#   def build(self):
#     if not self.skip_build:
#       if self.distribution.has_ext_modules():
#           self.run_command('build_ext')
#       if self.distribution.has_pure_modules():
#           self.run_command('build_py')
#
#
#   def run(self):
#     self.cmake_cmd= 'cmake'
#
#     if self.cmake is not None:
#       self.cmake_cmd=self.cmake
#
#     path=self.install_dir
#     path=path.replace("build/", "")
#     path=path.replace("build\\", "")
#     path=os.path.join(path, 'gVXR')
#     cwd = pathlib.Path().absolute()
#     build_temp = pathlib.Path("build")
#     os.chdir(str(build_temp))
#     self.spawn([self.cmake_cmd, '-DBUILD_TYPE=Release', '-P', 'cmake_install.cmake'] )
#
#     if sys.platform == 'win32'or sys.platform == 'cygwin':  #if windows, fetch the dlls in build/Release/lib
#       for dll in all_dll:
#         print("Copying "+all_dll[dll]+" to "+os.path.join(path,dll))
#         shutil.copyfile(all_dll[dll], os.path.join(path,dll))
#       for dll in glob.glob("lib/Release/*.dll"):
#         shutil.copyfile(dll, os.path.join(path,os.path.basename(dll)))
#     os.chdir(cwd)
#
class BuildCommand(build):

    user_options = build.user_options + get_options()

    def __init__(self, *args):
        super().__init__(*args)

        # Initialise the options
        init_values(self)

        # Download the code source
        checkoutSource()

        # Run CMake
        cwd = pathlib.Path().absolute()
        build_temp = pathlib.Path(getBinaryPath())
        build_temp.mkdir(parents=True, exist_ok=True)
        config = 'Release'
        cmake_args = [
            '-S' + getSourcePath(),
            '-B' + getBinaryPath(),
            '-DBUILD_TESTING=OFF',
            '-DBUILD_SIMPLEGVXR:BOOL=ON',
            '-DBUILD_WRAPPER_PYTHON3:BOOL=ON',
            '-DCMAKE_BUILD_TYPE:STRING=Release',
        ]

        if sys.platform == 'win32' or sys.platform == 'cygwin':
            build_args = [
                '--config', 'Release'
            ]
        else:
            build_args = [
                '--config', 'Release',
                '-j16'
            ]

        self.cmake_cmd= 'cmake'

        if self.cmake is not None:
            self.cmake_cmd=self.cmake

        os.chdir(str(build_temp))
        self.dry_run=None

        if self.generator:
            self.spawn([self.cmake_cmd, '-G', self.generator, "."] + cmake_args)
        else:
            self.spawn([self.cmake_cmd, "."] + cmake_args)

        # Not sure why I have to do it twice
        # If ran once, glfw can't find the compiler
        self.spawn([self.cmake_cmd, "."] + cmake_args)
        self.spawn([self.cmake_cmd, '--build', "." ] + build_args)
        # os.chdir(str(cwd))
        # build_directory = pathlib.Path(os.path.join(self.build_lib, 'gVXR'))
        # build_directory.mkdir(parents=True, exist_ok=True)
        # build_ext_orig.build_extensions(self)


      # self.sub_commands = [('build_ext',     build.has_ext_modules),
      #                 ('build_clib',    build.has_c_libraries),
      #                 ('build_py',      build.has_pure_modules),
      #                 ('build_scripts', build.has_scripts),
      #                ]


    # def initialize_options(self):
    #   build.initialize_options(self)
    #   init_values(self)

    # def finalize_options(self):
    #     build.finalize_options(self)
    #     self.set_undefined_options('install', *get_option_pairs())
    #     self.set_undefined_options('bdist_wheel', *get_option_pairs())


    # def set_undefined_options(self, src_cmd, *option_pairs):
    #    src_cmd_obj = self.distribution.get_command_obj(src_cmd)
    #   # Remove "src_cmd_obj.ensure_finalized()" to keep from calling install.finalize_options()
    #   # several times, as it will fail the second time.
    #    for (src_option, dst_option) in option_pairs:
    #      print(dst_option)
    #      if getattr(self, dst_option) is None:
    #          setattr(self, dst_option, getattr(src_cmd_obj, src_option))
#
#
#
#
#
# class my_build_ext(build_ext_orig):
#     user_options = build_ext_orig.user_options + get_options()

    # def initialize_options(self):
    #   build_ext_orig.initialize_options(self)
    #   self.install_userbase= None
    #   init_values(self)
    #   self.install_dir= None
    #
    # def finalize_options(self):
    #   build_ext_orig.finalize_options(self)
    #   self.set_undefined_options('build', *get_option_pairs())
    #   self.set_undefined_options('install', ('install_platlib', 'install_dir'))
    #
    #
    # def build_extension(self, ext):
    #   if sys.platform == 'win32'or sys.platform == 'cygwin':
    #     lib_ext='.pyd'
    #   else:
    #     lib_ext='.so'
    #   import os.path
    #   shutil.copyfile(os.path.join('.', 'build','build-python',ext.name+lib_ext), self.get_ext_fullpath(ext.name))


    # def run(self):
    #
    #     cwd = pathlib.Path().absolute()
    #     build_temp = pathlib.Path("build")
    #     build_temp.mkdir(parents=True, exist_ok=True)
    #     config = 'Release'
    #     cmake_args = [
    #         '-DBUILD_TESTING=OFF',
    #         '-DBUILD_SIMPLEGVXR:BOOL=ON',
    #         '-DBUILD_WRAPPER_PYTHON3:BOOL=ON',
    #         '-DCMAKE_BUILD_TYPE:STRING=Release',
    #     ]
    #     # if sys.platform == 'win32' or sys.platform == 'cygwin':
    #     #     cmake_args.append('-DCMAKE_DISABLE_FIND_PACKAGE_boost_serialization=TRUE ')
    #     #     cmake_args.append('-DCMAKE_DISABLE_FIND_PACKAGE_boost_iostreams=TRUE ')
    #     #     cmake_args.append('-DBoost_LIB_DIAGNOSTIC_DEFINITIONS=TRUE')
    #
    #     if self.install_dir is not None:
    #         cmake_args.append('-DCMAKE_INSTALL_PREFIX='+self.install_dir)
    #
    #     # if self.gVXR_dir is not None:
    #     #     cmake_args.append('-DgVXR_DIR='+self.gVXR_dir)
    #
    #     if self.enable_ASSIMP is not None:
    #         cmake_args.append('-DUSE_assimp:BOOL='+self.enable_ASSIMP)
    #
    #     if self.enable_OpenSceneGraph is not None:
    #         cmake_args.append('-DUSE_OPENSCENEGRAPH:BOOL='+self.enable_OpenSceneGraph)
    #
    #     if self.enable_ITK is not None:
    #         cmake_args.append('-DUSE_ITK:BOOL='+self.enable_ITK)
    #
    #     if self.enable_FFTW3 is not None:
    #         cmake_args.append('-DUSE_FFTW3:BOOL='+self.enable_FFTW3)
    #
    #     if self.enable_FreeType is not None:
    #         cmake_args.append('-DUSE_FREETYPE:BOOL='+self.enable_FreeType)
    #
    #     if self.enable_GDCM is not None:
    #         cmake_args.append('-DUSE_GDCM:BOOL='+self.enable_GDCM)
    #
    #     if self.enable_LibTIFF is not None:
    #         cmake_args.append('-DUSE_LIBTIFF:BOOL='+self.enable_LibTIFF)
    #
    #     if self.cmake_prefix_path is not None:
    #       cmake_args.append('-DCMAKE_PREFIX_PATH='+self.cmake_prefix_path)
    #
    #     if self.python_executable is not None:
    #       cmake_args.append('-DPython_EXECUTABLE='+self.python_executable)
    #
    #     if self.python_root is not None:
    #       cmake_args.append('-DPython_ROOT_DIR='+self.python_root)
    #
    #       if sys.platform != 'win32'or sys.platform == 'cygwin':
    #         cmake_args.append('-DPython_EXECUTABLE='+os.path.join(self.python_root, 'bin','python'))
    #
    #     cmake_args.append('-DINSTALL_FROM_SETUP=ON')
    #     # cmake_args.append('-DBoost_DEBUG=TRUE')
    #
    #
    #     if sys.platform == 'win32' or sys.platform == 'cygwin':
    #       build_args = [
    #       '--config', 'Release'
    #       ]
    #     else:
    #       build_args = [
    #       '--', '-j4'
    #       ]
    #     self.cmake_cmd= 'cmake'
    #     if self.cmake is not None:
    #       self.cmake_cmd=self.cmake
    #     os.chdir(str(build_temp))
    #     self.dry_run=None
    #     if self.generator:
    #       self.spawn([self.cmake_cmd, '-G', self.generator, str(cwd)] + cmake_args)
    #     else:
    #       self.spawn([self.cmake_cmd, str(cwd)] + cmake_args)
    #     self.spawn([self.cmake_cmd, '--build', '.'] + build_args)
    #     os.chdir(str(cwd))
    #     build_directory = pathlib.Path(os.path.join(self.build_lib, 'gVXR'))
    #     build_directory.mkdir(parents=True, exist_ok=True)
    #     build_ext_orig.build_extensions(self)
    #


setup(name='gVXR',
      version='5.1.0',
      author="gVXR Project",
      description="gVXR bindings, allowing to use some of the gVXR library in python.",
      long_description="The gVXR Bindings project allows to use some packages of gVXR, the Computational Algorithms Library with python. This project is still experimental and more packages will be added. For more information, please visit https://github.com/gVXR/gVXR-swig-bindings/wiki.",
      packages=['gVXR'],
      package_dir = {'gVXR': "build/" + TARGET_DIRECTORY},
      url="https://github.com/gVXR/gVXR-swig-bindings",
      python_requires='>=3.6',
      classifiers = [
        'Development Status :: 5 - Production/Stable',
        'Environment :: Console',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: GNU General Public License v3 or later (GPLv3+)',
        'Natural Language :: English',
        'Operating System :: MacOS :: MacOS X',
        'Operating System :: Microsoft :: Windows',
        'Operating System :: Unix',
        'Programming Language :: C++',
        'Programming Language :: Python :: 3',
        'Topic :: Scientific/Engineering',
        'Topic :: Scientific/Engineering :: Mathematics',
        'Topic :: Software Development :: Libraries'
    ],
      install_requires=['numpy'],
      package_data={'gVXR': ['*.dll']},
      cmdclass={
        # 'install' : InstallCommand,
        # 'install_lib' : InstallLibCommand,
        'build' : BuildCommand,
        # 'build_ext': my_build_ext,
        # 'bdist_wheel' : BuildWheelCommand,
    }
)
