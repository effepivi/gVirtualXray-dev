#!/bin/bash

rm -f filelist.txt

FILES=`ls ../../include/gVirtualXRay/*.h \
          ../../include/gVirtualXRay/*.inl \
          ../../src/*/*.cxx \
          ../../examples/*/*.h \
          ../../examples/*/*.inl \
          ../../examples/*/*.cxx \
          ../../resources/*.vert \
          ../../resources/*.frag \
          ../../test/*.cxx \
          ../../tutorials/*/*.h \
          ../../tutorials/*/*.inl \
          ../../tutorials/*/*.cxx \
          ../../validation/src/*.cxx \
          ../../validation/shaders/*.vert \
          ../../validation/shaders/*.frag `

for i in $FILES
do
    echo $i >>filelist.txt
done

KWStyle -xml gVirtualXRay.kws.xml -v -D filelist.txt -o overwrite.txt  -html results
