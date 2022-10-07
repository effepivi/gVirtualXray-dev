#!/usr/bin/sh

python3 -m pip install twine

twine check wheelhouse/repaired/*
twine check gVXR-*.*.*-*-*-win_amd64.whl
twine check gVXR-*.*.*-*-*-macosx_*_*_x86_64.whl

twine upload -r testpypi wheelhouse/repaired/*
twine upload -r testpypi gVXR-*.*.*-*-*-win_amd64.whl
twine upload -r testpypi gVXR-*.*.*-*-*-macosx_*_*_x86_64.whl
