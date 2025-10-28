#!/usr/bin/bash

# 1. Install git https://git-scm.com/download/win
# 2. Install various versions of cpython from https://www.python.org/


# for PYTHON_EXE in "C:\Program Files\Python38\python.exe" "C:\Program Files\Python39\python.exe" "C:\Program Files\Python310\python.exe" "C:\Program Files\Python311\python.exe" "C:\Program Files\Python312\python.exe" "C:\Python313\python.exe";
for PYTHON_EXE in "C:\Program Files\Python312\python.exe";
do
    echo BUILD $PYTHON_EXE

    "$PYTHON_EXE"  -m pip install --upgrade pip #> /dev/null 2> /dev/null
    "$PYTHON_EXE"  -m pip install numpy build scikit-build ninja #> /dev/null 2> /dev/null
    "$PYTHON_EXE"  -m pip wheel .. #> /dev/null 2> /dev/null
done
