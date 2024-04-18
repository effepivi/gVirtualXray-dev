#!/usr/bin/sh

MANYLINUX=manylinux2014_x86_64
docker build --tag 'build-gvxr' - < $PWD/Dockerfile-$MANYLINUX.txt

# Use nvidia-docker if possible
docker run -it \
            --ipc=host \
            --gpus all \
            -p 5000:5000 \
            --privileged \
            -e NVIDIA_VISIBLE_DEVICES=all -e NVIDIA_DRIVER_CAPABILITIES=display,compute \
            -e EGL_PLATFORM="EGL_PLATFORM_GBM_MESA" \
            -e DISPLAY \
            -e QT_X11_NO_MITSHM \
            -v ~/.Xauthority:/root/.Xauthority:rw \
            -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
            -v $(pwd)/..:/io \
            -w /io/pip build-gvxr \
            /bin/bash
