#!/usr/bin/sh

function repair_wheel {
    wheel="$1"
    if ! auditwheel show "$wheel"; then
        echo "Skipping non-platform wheel $wheel"
    else
        auditwheel repair "$wheel" --plat linux_x86_64 -w repaired_dist/
    fi
}


# Bundle external shared libraries into the wheels
for whl in ../dist/*.whl; do
    repair_wheel "$whl"
done



twine check repaired_dist/*

twine upload -r testpypi repaired_dist/*

#https://test.pypi.org/project/gVXR/2.0.0/
