#!/usr/bin/sh

MANYLINUX=manylinux2014_x86_64
docker build --tag 'build-gvxr' - < $PWD/Dockerfile-$MANYLINUX.txt

docker run -it \
    -p 5000:5000 \
    -e EGL_PLATFORM="EGL_PLATFORM_GBM_MESA" \
    -e DISPLAY=$DISPLAY \
    -v ~/.Xauthority:/root/.Xauthority:ro \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    -v $(pwd)/..:/io \
    -w /io/pip build-gvxr \
    /bin/bash
