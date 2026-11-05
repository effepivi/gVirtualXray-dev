#!/usr/bin/bash

rm -f windows_test_results.dat

#for PYTHON_VERSION in "310" "311" "312" "313" "314" "315";
for PYTHON_VERSION in "310" "311" "312" "313" "314";
#for PYTHON_VERSION in "313";
do
    
    PYTHON_EXE="C:\Program Files\Python$PYTHON_VERSION\python.exe"
    echo TEST $PYTHON_EXE

    GVXR=`ls gvxr-2.1.1-cp$PYTHON_VERSION-cp$PYTHON_VERSION-win_amd64.whl`
    #"$PYTHON_EXE" -m pip install numpy matplotlib > /dev/null 2> /dev/null
    "$PYTHON_EXE" -m pip install --force-reinstall $GVXR > /dev/null 2> /dev/null
    "$PYTHON_EXE" -m gvxrPython3.tests.test_gvxr > /dev/null 2> /dev/null

    echo "$PYTHON_EXE $GVXR $?" >> windows_test_results.dat
    echo
done
exit
