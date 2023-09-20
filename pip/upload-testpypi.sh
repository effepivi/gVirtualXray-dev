#!/usr/bin/sh

python3 -m pip install twine

python3 -m twine check wheelhouse/repaired/*
python3 -m twine check gVXR-*.*.*-*-*-win_amd64.whl
python3 -m twine check gVXR-*.*.*-*-*-macosx_*_*_x86_64.whl

python3 -m twine upload -r testpypi wheelhouse/repaired/*
python3 -m twine upload -r testpypi gVXR-*.*.*-*-*-win_amd64.whl
python3 -m twine upload -r testpypi gVXR-*.*.*-*-*-macosx_*_*_x86_64.whl
