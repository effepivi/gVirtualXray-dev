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


# Compile and install gVirtualXRay


# Where the libraries will be installed
# (you may change it if you want)
DESTINATION=$PWD/install

#####PROMPT HERE TO ASK WHERE THE INSTALL PATH IS

echo "************************"
echo "Temporarily install the libraries in $DESTINATION"
echo "************************"


# Retrieve the code from the trunk on SVN
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


# Delete previous build if needed
read -p "Do you want to delete the previous builds? (Y/N)  " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]
then
    echo "Delete the previous builds"
    rm -rf gvxr-build/ SimpleGVXR-build/ Wrappers-build/ $DESTINATION/
fi
echo


BUILD_WRAPPER="0"
read -p "Build C# wrapper? (Y/N)  " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]
then
    BUILD_CSHARP="ON"
    BUILD_WRAPPER="1"
else
    BUILD_CSHARP="OFF"
fi


read -p "Build Java wrapper? (Y/N)  " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]
then
    BUILD_JAVA="ON"
    BUILD_WRAPPER="1"
else
    BUILD_JAVA="OFF"
fi


read -p "Build GNU Octave wrapper? (Y/N)  " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]
then
    BUILD_OCTAVE="ON"
    BUILD_WRAPPER="1"
else
    BUILD_OCTAVE="OFF"
fi


read -p "Build Perl wrapper? (Y/N)  " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]
then
    BUILD_PERL="ON"
    BUILD_WRAPPER="1"
else
    BUILD_PERL="OFF"
fi


read -p "Build Python 3 wrapper? (Y/N)  " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]
then
    BUILD_WRAPPER="1"
    BUILD_PYTHON3="ON"
    BUILD_PYTHON2="OFF"
else
    BUILD_PYTHON3="OFF"

    read -p "Build Python 2 wrapper? (Y/N)  " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]
    then
        BUILD_WRAPPER="1"
        BUILD_PYTHON2="ON"
    else
        BUILD_PYTHON2="OFF"
    fi
fi


read -p "Build R wrapper? (Y/N)  " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]
then
    BUILD_WRAPPER="1"
    BUILD_R="ON"
else
    BUILD_R="OFF"
fi


read -p "Build Ruby wrapper? (Y/N)  " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]
then
    BUILD_WRAPPER="1"
    BUILD_RUBY="ON"
else
    BUILD_RUBY="OFF"
fi


read -p "Build Tcl wrapper? (Y/N)  " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]
then
    BUILD_WRAPPER="1"
    BUILD_TCL="ON"
else
    BUILD_TCL="OFF"
fi
echo

echo "***************************"
echo "* Build wrappers (summary):"
echo "***************************"
echo Build C# wrapper: $BUILD_CSHARP
echo Build Java wrapper: $BUILD_JAVA
echo Build GNU Octave wrapper: $BUILD_OCTAVE
echo Build Perl wrapper: $BUILD_PERL
echo Build Python 2 wrapper: $BUILD_PYTHON2
echo Build Python 3 wrapper: $BUILD_PYTHON3
echo Build R wrapper: $BUILD_R
echo Build Ruby wrapper: $BUILD_RUBY
echo Build Tcl wrapper: $BUILD_TCL
echo

# Create the build directories if needed
if [[ ! -d gvxr-build ]]
then
    echo "*******************************"
    echo "Create the gvxr-build directory"
    echo "*******************************"
    echo
    mkdir gvxr-build
fi

if [[ ! -d SimpleGVXR-build ]]
then
    echo "*************************************"
    echo "Create the SimpleGVXR-build directory"
    echo "*************************************"
    echo
    mkdir SimpleGVXR-build
fi

if [[ ! -d Wrappers-build ]]
then
    echo "***********************************"
    echo "Create the Wrappers-build directory"
    echo "***********************************"
    mkdir Wrappers-build
fi


echo "***********************************"
echo "* Unit testing                    *"
echo "***********************************"
read -p "Will you want to run the unit tests? (Y/N)  " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]
then
    UNIT_TEST="ON"
fi


if [[ $BUILD_WRAPPER == "1" ]]
then
    read -p "Will you want to run the wrapper tests? (Y/N)  " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]
    then
        WRAPPER_TEST="ON"
    fi
fi


# Build and install the core library
cd gvxr-build

echo "*************************************"
echo "* Configure, build and install gvxr *"
echo "* (run the unit test, optional)     *"
echo "*************************************"
echo
echo "Configure gvxr using CMake"
echo "Please wait"
echo

cmake ../gvxr \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=$DESTINATION \
    > ../gvxr_cmake.out.log 2> ../gvxr_cmake.err.log

read -p "Do you want to review (and tweak) gvxr build settings using ccmake (for advanced users)? (Y/N)  " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]
then
    ccmake .
fi

echo "Build gvxr using make"
echo "Please wait (or go and make some coffee, it'll take a while...)"
echo

make -j6 > ../gvxr_make.out.log 2> ../gvxr_make.err.log

echo "Install gvxr in $DESTINATION"
echo
make install > ../install.log 2> ../install.err.log

if [[ $UNIT_TEST == "ON" ]]
then
    echo "Run the unit tests"
    echo
    make test > ../unit_test.log 2> ../unit_test.err
fi


# Build and install the SimpleGVXR library
cd ../SimpleGVXR-build

echo "*******************************************"
echo "* Configure, build and install SimpleGVXR *"
echo "*******************************************"
echo
echo "Configure SimpleGVXR using CMake"
echo
cmake ../SimpleGVXR \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX=$DESTINATION \
      -DUSE_SYSTEM_gVirtualXRay=ON \
      -DgVirtualXRay_DIR=$DESTINATION/gVirtualXRay-1.1.0 \
      > ../SimpleGVXR_cmake.out.log 2> ../SimpleGVXR_cmake.err.log

read -p "Do you want to review (and tweak) SimpleGVXR build settings using ccmake (for advanced users)? (Y/N)  " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]
then
    ccmake .
fi

echo "Build SimpleGVXR using make"
echo
make > ../SimpleGVXR_make.out.log 2> ../SimpleGVXR_make.err.log

echo "Install SimpleGVXR in $DESTINATION"
echo
make install >> ../install.log 2>> ../install.err.log



# Build and install the Python3 package
cd ../Wrappers-build



if [[ $BUILD_WRAPPER == "0" ]]
then
    echo "No wrapper to build."
    echo
else
    echo "*****************************************"
    echo "* Configure, build and install Wrappers *"
    echo "*****************************************"
    echo
    echo "Configure Wrappers using CMake"
    echo
    cmake ../Wrappers \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=$DESTINATION \
        -DUSE_SYSTEM_gVirtualXRay=ON \
        -DgVirtualXRay_DIR=$DESTINATION/gVirtualXRay-1.1.0 \
        -DUSE_SYSTEM_SimpleGVXR=ON \
        -DSimpleGVXR_DIR=$DESTINATION//SimpleGVXR-1.0.0 \
        -DBUILD_CSHARP=$BUILD_CSHARP \
        -DBUILD_JAVA=$BUILD_JAVA \
        -DBUILD_OCTAVE=$BUILD_OCTAVE \
        -DBUILD_PERL=$BUILD_PERL \
        -DBUILD_PYTHON2=$BUILD_PYTHON2 \
        -DBUILD_PYTHON3=$BUILD_PYTHON3 \
        -DBUILD_R=$BUILD_R \
        -DBUILD_RUBY=$BUILD_RUBY \
        -DBUILD_TCL=$BUILD_TCL \
    > ../Wrappers_cmake.out.log 2> ../Wrappers_cmake.err.log


    read -p "Do you want to review (and tweak) Wrappers build settings using ccmake (for advanced users)? (Y/N)  " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]
    then
        ccmake .
    fi


    echo "Build Wrappers using make"
    echo
    make > ../Wrappers_make.out.log 2> ../Wrappers_make.err.log

    echo "Install Wrappers in $DESTINATION"
    echo
    make install >> ../install.log 2>> ../install.err.log

    if [[ $WRAPPER_TEST == "ON" ]]
    then
        echo "Test the wrappers"
        echo
        ./runAll.sh > ../wrapper_test.log 2> ../wrapper_test.err.log
    fi

    if [[ $BUILD_PYTHON2 == "ON" ]]
    then
        read -p "Install gvxrPython2 for all users? (Y/N)  " -n 1 -r
        echo
        if [[ $REPLY =~ ^[Yy]$ ]]
        then
            echo "Install gvxrPython2"
            sudo make install-gvxrPython2
        fi
    fi

    if [[ $BUILD_PYTHON3 == "ON" ]]
    then
        read -p "Install gvxrPython3 for all users? (Y/N)  " -n 1 -r
        echo
        if [[ $REPLY =~ ^[Yy]$ ]]
        then
            echo "Install gvxrPython3"
            sudo make install-gvxrPython3
        fi
    fi

fi



# Run wrappers' test
