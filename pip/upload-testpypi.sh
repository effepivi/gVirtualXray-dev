#!/usr/bin/sh

python3 -m pip install twine

twine check wheelhouse/repaired/*
twine upload -r testpypi wheelhouse/repaired/*
