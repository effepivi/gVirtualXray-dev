#!/usr/bin/sh

#set -e -u -x

function repair_wheel {
    wheel="$1"
    if ! auditwheel show "$wheel"; then
        echo "Skipping non-platform wheel $wheel"
    else
        #/opt/_internal/cpython-3.9.13/bin/auditwheel repair "$wheel" --plat manylinux2014_x86_64 -w /io/pip/wheelhouse/repaired
        /opt/_internal/cpython-3.9.13/bin/auditwheel repair "$wheel"  --plat manylinux_2_17_x86_64 --exclude libOpenGL.so,libGLX.so -w /io/pip/wheelhouse/repaired
        #/opt/_internal/cpython-3.9.13/bin/auditwheel repair "$wheel"  --plat manylinux_2_24_x86_64 --exclude libOpenGL.so,libGLX.so -w /io/pip/wheelhouse/repaired
        #/opt/_internal/cpython-3.9.13/bin/auditwheel repair "$wheel"  --plat manylinux_2_27_x86_64 --exclude libOpenGL.so,libGLX.so -w /io/pip/wheelhouse/repaired
    fi
}

# Install RPMs
# yum install -y wget
yum install -y libXi-devel libXrandr-devel libXinerama-devel libXcursor-devel
yum install -y mesa-libEGL-devel libGL-devel libGLU-devel
# yum install -y glfw-devel
#yum install -y python3-devel

# Install cmake
/opt/python/cp39-cp39/bin/python3 -m pip install --upgrade pip
/opt/python/cp39-cp39/bin/python3 -m pip install cmake twine

# Go where the code is
cd /io

# Create the output directory if needed
if [ ! -d /io/pip/wheelhouse/repaired ]
then
    mkdir -p /io/pip/wheelhouse/repaired
fi

# Enable parallel build
export MAKEFLAGS="-j$(nproc)"

# Delete the old builds
rm -rf /io/Wrappers/gVXR.egg-info
rm -rf /io/pip/wheelhouse/gVXR-*.*.*-*-*-linux_x86_64.whl
rm -rf /io/pip/wheelhouse/repaired/gVXR-*.*.*-*-*-manylinux*_x86_64.whl

# Download and build glad
# cd /tmp
# rm -rf glad
# git clone https://github.com/Dav1dde/glad.git
# cd glad
# cmake -S $PWD -B bin \
#     -DCMAKE_BUILD_TYPE:STRING=Release \
#     -DCMAKE_C_FLAGS=-fPIC -DCMAKE_CXX_FLAGS=-fPIC \
#     -DGLAD_ALL_EXTENSIONS:BOOL=ON \
#     -DGLAD_SPEC:STRING=egl \
#     -DGLAD_GENERATOR=c \
#     -DPROFILE=core
#
# cmake --build bin
# cmake --install bin

# Create all the wheels
for PYBIN in /opt/python/*/bin; do
    rm -rf /io/_skbuild
    "${PYBIN}/pip" install --upgrade pip
    "${PYBIN}/pip" uninstall -y auditwheel
    "${PYBIN}/pip" install -r /io/pip/dev-requirements.txt
    "${PYBIN}/pip" wheel  /io/ --no-deps -w /io/pip/wheelhouse/
done

/opt/python/cp39-cp39/bin/python3 -m pip uninstall -y auditwheel
/opt/python/cp39-cp39/bin/python3 -m pip install git+https://github.com/rossant/auditwheel.git@include-exclude



# Bundle external shared libraries into the wheels
for whl in /io/pip/wheelhouse/*.whl; do
    repair_wheel "$whl"
done

/opt/python/cp39-cp39/bin/python3 -m twine check /io/pip/wheelhouse/repaired/*.whl
