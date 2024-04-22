#!/usr/bin/env sh

#set -e -u -x

CPYTHON_PATH="/opt/_internal/`ls /opt/_internal/ | grep cpython-3.11`"

function repair_wheel {
    wheel="$1"
    if ! auditwheel show "$wheel"; then
        echo "Skipping non-platform wheel $wheel"
    else
        #$CPYTHON_PATH/bin/auditwheel repair "$wheel" --plat manylinux_2_28_x86_64 -w /io/pip/wheelhouse/repaired
        $CPYTHON_PATH/bin/auditwheel repair "$wheel"  --plat $AUDITWHEEL_PLAT --exclude libOpenGL.so,libGLX.so -w /io/pip/wheelhouse/repaired
    fi
}




# Create the output directory if needed
if [ ! -d /io/pip/wheelhouse/repaired ]
then
    mkdir -p /io/pip/wheelhouse/repaired
fi

# Enable parallel build
export MAKEFLAGS="-j$(nproc)"

# Delete the old builds
rm -rf /io/Wrappers/gVXR.egg-info
rm -rf /io/pip/wheelhouse/gvxr-*.*.*-*-*-linux_x86_64.whl
rm -rf /io/pip/wheelhouse/repaired/gvxr-*.*.*-*-*-manylinux*.manylinux2014_x86_64.whl

# Create all the wheels
#for PYSUBDIR in "cp310-cp310"; do
for PYSUBDIR in `manylinux-interpreters list --installed `; do

    PYDIR=/opt/python/$PYSUBDIR
    rm -rf /io/_skbuild
    "${PYDIR}/bin/pip" install --upgrade pip
    "${PYDIR}/bin/pip" uninstall -y auditwheel
    "${PYDIR}/bin/pip" install -r /io/pip/dev-requirements.txt

    export Python_ROOT_DIR="${PYDIR}"
    export PYTHON_INCLUDE_DIRS="${PYDIR}/include"
    export Python_EXECUTABLE="${PYDIR}/bin/python"
    # export PYTHON_LIBRARIES='/usr/lib64/libpython3.so'
    # export PYTHON_LIBRARIES=`${PYDIR}/bin/python3-config --libs`
    export PYTHON_LIBRARY=$PYTHON_LIBRARIES

    # rm -f /usr/local/bin/cmake
    # ln -s "${PYDIR}/bin/cmake" /usr/local/bin/cmake

    MAKEFLAGS="-j 24" \
    PYTHON_LIBRARY=$PYTHON_LIBRARIES \
    PYTHON_LIBRARIES=$PYTHON_LIBRARIES \
    Python_ROOT_DIR=$Python_ROOT_DIR \
    PYTHON_INCLUDE_DIRS=$PYTHON_INCLUDE_DIRS \
    Python_EXECUTABLE=$Python_EXECUTABLE \
    "${PYDIR}/bin/pip" wheel  /io/ --no-clean --no-deps -w /io/pip/wheelhouse/
done

# $CPYTHON_PATH/bin/python3 -m pip install auditwheel
$CPYTHON_PATH/bin/python3 -m pip uninstall -y auditwheel
$CPYTHON_PATH/bin/python3 -m pip install git+https://github.com/rossant/auditwheel.git@include-exclude

# Bundle external shared libraries into the wheels
for whl in /io/pip/wheelhouse/*.whl; do
    repair_wheel "$whl"
done

$CPYTHON_PATH/bin/python3 -m twine check /io/pip/wheelhouse/repaired/*.whl
