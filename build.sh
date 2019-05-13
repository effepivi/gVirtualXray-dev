#!/bin/bash

cmake \
    -DUNIT_TESTS:BOOL=OFF \
    -DBUILD_PYTHON3:BOOL=ON \
    -DBUILD_CSHARP:BOOL=ON \
    -DWRAPPER_TEST:BOOL=ON \
    -DUSE_SYSTEM_XCOM:BOOL=ON \
    -DUSE_SYSTEM_ASSIMP:BOOL=ON \
    -DUSE_SYSTEM_GLEW:BOOL=ON \
    -DUSE_SYSTEM_GLFW:BOOL=ON \
    -DXCOM_PATH:STRING=/Users/eese10/Documents/gvirtualxray-code/XCOM \
    -P build.cmake
