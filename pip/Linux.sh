#!/usr/bin/bash

for VERSION in 3.6 3.7 3.8 3.9 3.10
do

    rm -rf ../_skbuild/linux-x86_64-$VERSION

    if [ -d $HOME/miniconda3/envs/gvxr-build-$VERSION ]; then
        echo Miniconda environment found in  $HOME/miniconda3/envs/gvxr-build-$VERSION
    elif  [ -d $HOME/anaconda3/envs/gvxr-build-$VERSION ]; then
        echo Anaconda environment found in  $HOME/miniconda3/envs/gvxr-build-$VERSION
    else
        echo Anaconda/Miniconda environment gvxr-build-$VERSION not found. It must be created.
        conda create -y --name gvxr-build-$VERSION python=$VERSION
        conda activate gvxr-build-$VERSION; conda install -y -c conda-forge build scikit-build ninja
    fi
done

SOURCE_DIR=$PWD/..

for VERSION in 3.6 3.7 3.8 3.9 3.10
do
    SHORT_VERSION=`echo $VERSION | tr -d '.'`

    if [ -d $HOME/miniconda3/envs/gvxr-build-$VERSION ]; then
        echo Miniconda environment found in  $HOME/miniconda3/envs/gvxr-build-$VERSION
        PYTHON=$HOME/miniconda3/envs/gvxr-build-$VERSION/bin/python3
    elif  [ -d $HOME/anaconda3/envs/gvxr-build-$VERSION ]; then
        echo Anaconda environment found in  $HOME/miniconda3/envs/gvxr-build-$VERSION
        PYTHON=$HOME/anaconda3/envs/gvxr-build-$VERSION/bin/python3
    else
        echo Where is my gvxr-build-$VERSION environment?
        exit
    fi

    echo Python interpretor: $PYTHON

    rm -f $SOURCE_DIR/dist/gVXR-2.0.0-cp$SHORT_VERSION-cp${SHORT_VERSION}m-linux_x86_64.whl
    rm -f $SOURCE_DIR/dist/gVXR-2.0.0-py${VERSION}-linux-x86_64.egg

    if [ ! -f $SOURCE_DIR/dist/gVXR-2.0.0-cp$SHORT_VERSION-cp${SHORT_VERSION}m-linux_x86_64.whl ]; then

        $PYTHON -m pip install --upgrade pip
        $PYTHON -m pip install build scikit-build ninja

        cd $SOURCE_DIR ; $PYTHON setup.py \
            build \
            --build-type Release \
            --install-target install-python \
            -- -- -j16
            # --generator "Unix Makefiles" \

        cd $SOURCE_DIR ; $PYTHON setup.py \
            install \
            --build-type Release \
            --install-target install-python \
            -- -- -j16
            # --generator "Unix Makefiles" \

        cd $SOURCE_DIR ; $PYTHON setup.py \
            bdist_wheel \
            --build-type Release \
            --install-target install-python \
            -- -- -j16
            # --generator "Unix Makefiles" \
    fi


    # Uninstall gVXR if needed
    $PYTHON -m pip  uninstall -y gvxr

    # Install gVXR
    $PYTHON -m pip install $SOURCE_DIR/dist/gVXR-2.0.0-cp$SHORT_VERSION-cp${SHORT_VERSION}m-linux_x86_64.whl
done
