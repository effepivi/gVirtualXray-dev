#!/usr/bin/sh

twine check wheelhouse/repaired/*
twine upload -r testpypi wheelhouse/repaired/*
