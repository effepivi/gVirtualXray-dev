#!/usr/bin/sh

MANYLINUX=manylinux2014_x86_64
docker build --tag 'build-gvxr' - < $PWD/Dockerfile-$MANYLINUX.txt

# Use nvidia-docker if possible
docker run -it \
            --gpus all \
            -p 5000:5000 \
            --privileged \
            -e NVIDIA_VISIBLE_DEVICES=all -e NVIDIA_DRIVER_CAPABILITIES=display,compute \
            -e EGL_PLATFORM="EGL_PLATFORM_GBM_MESA" \
            -e DISPLAY=unix$DISPLAY \
            -e QT_X11_NO_MITSHM \
            -v ~/.Xauthority:/root/.Xauthority:ro \
            -v /tmp/.X11-unix:/tmp/.X11-unix \
            -v $(pwd)/..:/io \
            -w /io/pip build-gvxr \
            /bin/bash
