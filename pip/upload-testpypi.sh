#!/usr/bin/sh

python3 -m pip install twine

twine check wheelhouse/repaired/*
twine check gVXR-*.*.*-*-*-win_amd64.whl

twine upload -r testpypi wheelhouse/repaired/*
twine upload -r testpypi gVXR-*.*.*-*-*-win_amd64.whl

