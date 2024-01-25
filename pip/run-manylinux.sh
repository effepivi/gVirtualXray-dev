#!/usr/bin/sh

MANYLINUX=manylinux2014_x86_64
docker build --tag 'build-gvxr' - < $PWD/Dockerfile-$MANYLINUX.txt
docker run -it -v $(pwd)/..:/io -w /io/pip build-gvxr  /bin/bash
