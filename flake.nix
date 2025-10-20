{
	inputs = {
		# Use dependencies from nixos-unstable
		nixpkgs = { url = "github:nixos/nixpkgs/nixos-unstable"; };
		# Use flake-utils for some helper functions
		flake-utils = { url = "github:numtide/flake-utils"; };
	};
	outputs = { nixpkgs, flake-utils, ... }: flake-utils.lib.eachDefaultSystem (system:
		let
			# Use packages from nixpkgs
			pkgs = import nixpkgs { inherit system; };

			gvirtualxray = (with pkgs; stdenv.mkDerivation {
				pname = "gvxr";
				version = "rev2598";

				# src = fetchsvn {
				# 	url = "svn://svn.code.sf.net/p/gvirtualxray/code/trunk";
				# 	rev = "2584";
				# 	sha256 = "4DpqgTK/sFCe0MmAR4n6B0gJWLQ3Mtr4emuxWNEoy8k=";
				# };
				src = ./.;

				# Build-time dependencies
				nativeBuildInputs = [
					# gvxr uses cmake for building
					cmake

					# X11 required for window libraries
					xorg.libX11
					xorg.libX11.dev
					xorg.libXi.dev
					xorg.libXcursor.dev
					xorg.libXxf86vm.dev
					xorg.libXinerama.dev
					xorg.libXrandr.dev

					# boost
					boost.dev

					# OpenGL & GLEW
					libGL
					libGLU
					glfw

					glew.dev
					glew.out

					# GNU Scientific Library
					# Used for multithreaded std::random
					gsl.dev

					# image codecs
					assimp
					libtiff.dev
					libpng.dev
					gdcm # The grassroots cross-platform DICOM implementation

					# itk # Insight Segmentation and Registration Toolkit
					freetype.dev # Font rendering engine

					# Data compression
					zlib # Lossless data-compression library
					bzip2.dev # High-quality data compression
					minizip

					# ----- Wrappers
					swig # interface compiler

					python312
					python312Packages.numpy

				];

				cmakeFlags = [
					# Dependencies
					"-DUSE_SYSTEM_GLEW=ON"
					"-DGLEW_DIR=${glew.dev}/include/GL/"
					"-DGLEW_LIBRARY_RELEASE=${glew}/lib/libGLEW.so"

					"-DUSE_SYSTEM_GLFW=ON"
					"-DGLFW3_INCLUDE_DIR=${glfw}/include/glfw"

					"-DUSE_assimp=ON"
					"-DUSE_SYSTEM_ASSIMP=ON"
					"-DASSIMP_INCLUDE_DIR=${assimp.dev}/include/assimp"

					"-DUSE_SYSTEM_ZLIB=ON"
					"-DUSE_SYSTEM_LIBTIFF=ON"

					"-DUSE_SYSTEM_FREETYPE=ON"
					"-DUSE_SYSTEM_GDCM=OFF"
					#
					"-DUSE_SYSTEM_BOOST=ON"
					"-Dboost_INCLUDE_DIR=${boost.dev}/include/boost/"

					# Wrappers
					"-DBUILD_WRAPPERS=ON"
					"-DBUILD_WRAPPER_PYTHON3=ON"
					"-DPYTHON_LIBRARIES=${python312}/lib/libpython3.10.so"
					"-DPYTHON_INCLUDE_DIR=${python312}/include/python3.10/"
					"-DPYTHON_EXACUTABLE=${python312}/bin/python3.10"
					"-DPython3_NumPy_INCLUDE_DIRS=${python312Packages.numpy}"

					# debug
					# "-DCMAKE_FIND_DEBUG_MODE=ON"
				];

				buildPhase = "make -j $NIX_BUILD_CORES > log.txt";

				installTargets = "install-python";

			});

		in rec {
			defaultPackage = gvirtualxray;
		}
	);
}
