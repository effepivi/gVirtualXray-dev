#!/usr/bin/sh

#set -e -u -x

function repair_wheel {
    wheel="$1"
    if ! auditwheel show "$wheel"; then
        echo "Skipping non-platform wheel $wheel"
    else
        auditwheel repair "$wheel" --plat manylinux2014_x86_64 -w /io/pip/wheelhouse/repaired
    fi
}

# Install RPMs
yum install -y libXi-devel libXrandr-devel libXinerama-devel libXcursor-devel
yum install -y mesa-libEGL-devel libGL-devel libGLU-devel
yum install -y python3-devel

# Install cmake
python3 -m pip install --upgrade pip
python3 -m pip install cmake twine

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
rm -rf /io/_skbuild

# Create all the wheels
#for PYBIN in /opt/python/*/bin; do
for PYBIN in /opt/python/*/bin; do
    "${PYBIN}/pip" install --upgrade pip
    "${PYBIN}/pip" install -r /io/pip/dev-requirements.txt
    "${PYBIN}/pip" wheel  /io/ --no-deps -w /io/pip/wheelhouse/
done

# Bundle external shared libraries into the wheels
for whl in /io/pip/wheelhouse/*.whl; do
    repair_wheel "$whl"
done

twine check /io/pip/wheelhouse/repaired/*.whl
