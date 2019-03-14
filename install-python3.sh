#!/bin/sh

#
#
# Copyright (c) 2019, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
# http://www.fpvidal.net/
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
# this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation and/or
# other materials provided with the distribution.
#
# 3. Neither the name of the Bangor University nor the names of its contributors
# may be used to endorse or promote products derived from this software without
# specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


if [[ -d .svn ]]
then
    read -p "Do you want to update the source code from SVN? (Y/N)" -n 1 -r
    echo    # (optional) move to a new line
    if [[ $REPLY =~ ^[Yy]$ ]]
    then
        echo "Update the source code from SVN"
        svn update  > svn_update.out.log 2> svn_update.err.log
    fi
else
    read -p "Do you want to retrieve the source code from SVN? (Y/N)" -n 1 -r
    echo    # (optional) move to a new line
    if [[ $REPLY =~ ^[Yy]$ ]]
    then
        echo "Get the source code from SVN"
        svn checkout https://svn.code.sf.net/p/gvirtualxray/code/trunk .  > svn_checkout.out.log 2> svn_checkout.err.log
    else
        echo "We can't install gvxrPython3 without the source code."
        echo "The install script will terminate."
        exit
    fi
fi


# Compile and install gvxrPython3


# Where the libraries will be installed
# (you may change it if you want)
DESTINATION=$PWD/install


echo "************************"
echo "Temporarily install the libraries in $DESTINATION"
echo "************************"


# Delete previous build if needed
read -p "Do you want to delete the previous builds? (I recommend Y) (Y/N)" -n 1 -r
echo    # (optional) move to a new line
if [[ $REPLY =~ ^[Yy]$ ]]
then
    echo "Delete the previous builds"
    rm -rf gvxr-build/ SimpleGVXR-build/ Wrappers-build/ $DESTINATION/
fi


# Create the build directories if needed
if [[ ! -d gvxr-build ]]
then
    echo "*******************************"
    echo "Create the gvxr-build directory"
    echo "*******************************"
    mkdir gvxr-build
fi

if [[ ! -d SimpleGVXR-build ]]
then
    echo "*************************************"
    echo "Create the SimpleGVXR-build directory"
    echo "*************************************"
    mkdir SimpleGVXR-build
fi

if [[ ! -d Wrappers-build ]]
then
    echo "***********************************"
    echo "Create the Wrappers-build directory"
    echo "***********************************"
    mkdir Wrappers-build
fi



# Build and install the core library
cd gvxr-build

echo "Configure gvxr using CMake"
cmake ../gvxr \
  -DCMAKE_INSTALL_PREFIX=$DESTINATION \
  > ../gvxr_cmake.out.log 2> ../gvxr_cmake.err.log

echo "Build gvxr using make"
make -j6 > ../gvxr_make.out.log 2> ../gvxr_make.err.log

echo "Install gvxr in $DESTINATION"
make install > /dev/null



# Build and install the SimpleGVXR library
cd ../SimpleGVXR-build

echo "Configure SimpleGVXR using CMake"
cmake ../SimpleGVXR \
  -DCMAKE_INSTALL_PREFIX=$DESTINATION \
  -DUSE_SYSTEM_gVirtualXRay=ON \
  -DgVirtualXRay_DIR=$DESTINATION/gVirtualXRay-1.1.0 \
  > ../SimpleGVXR_cmake.out.log 2> ../SimpleGVXR_cmake.err.log

echo "Build SimpleGVXR using make"
make > ../SimpleGVXR_make.out.log 2> ../SimpleGVXR_make.err.log

echo "Install SimpleGVXR in $DESTINATION"
make install > /dev/null



# Build and install the Python3 package
cd ../Wrappers-build

echo "Configure Wrappers using CMake"
cmake ../Wrappers \
    -DCMAKE_INSTALL_PREFIX=$DESTINATION \
    -DUSE_SYSTEM_gVirtualXRay=ON \
    -DgVirtualXRay_DIR=$DESTINATION/gVirtualXRay-1.1.0 \
    -DUSE_SYSTEM_SimpleGVXR=ON \
    -DSimpleGVXR_DIR=$DESTINATION//SimpleGVXR-1.0.0 \
    -DBUILD_PYTHON3=ON \
> ../Wrappers_cmake.out.log 2> ../Wrappers_cmake.err.log

echo "Build Wrappers using make"
make > ../Wrappers_make.out.log 2> ../Wrappers_make.err.log


read -p "Install gvxrPython3 for all users (Y/N)" -n 1 -r
echo    # (optional) move to a new line
if [[ $REPLY =~ ^[Yy]$ ]]
then
    echo "Install gvxrPython3"
    sudo make install-gvxrPython3
fi


# Test the python3 package
cd python3
echo "Test the python3 package"
python3 test.py
