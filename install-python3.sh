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




# Compile and install gvxrPython3


# Where the libraries will be installed
# (you may change it if you want)
DESTINATION=$PWD/install

echo ************************
echo Install the libraries in $DESTINATION
echo ************************


# Delete previous build if needed
rm -rf gvxr-build/ SimpleGVXR-build/ Wrappers-build/ $DESTINATION/

# Create the build directories if needed
if [ ! -d gvxr-build ]
then
    echo *******************************
    echo Create the gvxr-build directory
    echo *******************************
    mkdir gvxr-build
fi

if [ ! -d SimpleGVXR-build ]
then
    echo *************************************
    echo Create the SimpleGVXR-build directory
    echo *************************************
    mkdir SimpleGVXR-build
fi

if [ ! -d Wrappers-build ]
then
    echo ***********************************
    echo Create the Wrappers-build directory
    echo ***********************************
    mkdir Wrappers-build
fi

# Build and install the core library
cd gvxr-build
cmake ../gvxr -DCMAKE_INSTALL_PREFIX=$DESTINATION
make -j6
make install

# Build and install the SimpleGVXR library
cd ../SimpleGVXR-build
cmake ../SimpleGVXR -DCMAKE_INSTALL_PREFIX=$DESTINATION -DUSE_SYSTEM_gVirtualXRay=ON
make -j6
make install

# Build and install the Python3 package
cd ../Wrappers-build
cmake ../Wrappers -DCMAKE_INSTALL_PREFIX=$DESTINATION -DUSE_SYSTEM_SimpleGVXR=ON -DBUILD_PYTHON3=ON
make -j6
sudo make install-gvxrPython3

# Test the python3 package
python3 python3/test.py
