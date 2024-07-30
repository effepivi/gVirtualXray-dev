#!/usr/bin/sh

xhost +

ARCH=x86_64
MANYLINUX=manylinux2014_$ARCH
docker build --platform linux/amd64 --tag build-gvxr-$ARCH - < $PWD/Dockerfile-$MANYLINUX.txt

docker run \
    --platform linux/amd64 \
     --rm -it \
    --ipc=host \
    --gpus all \
    -e NVIDIA_VISIBLE_DEVICES=all -e NVIDIA_DRIVER_CAPABILITIES=display,compute \
    -e EGL_PLATFORM="EGL_PLATFORM_GBM_MESA" \
    -e DISPLAY \
    -e QT_X11_NO_MITSHM \
    --privileged \
    -d \
    -p 5000:5000 \
    -v ~/.Xauthority:/root/.Xauthority:rw \
    -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
    -v $(pwd)/..:/io \
    -w /io/pip \
    build-gvxr-$ARCH \
    /bin/bash



# ARCH=aarch64
# MANYLINUX=manylinux_2_28_$ARCH
# docker build --platform linux/arm64 --tag build-gvxr-$ARCH - < $PWD/Dockerfile-$MANYLINUX.txt

# docker run \
#     --platform linux/arm64 \
#      --rm -it \
#     --ipc=host \
#     -e EGL_PLATFORM="EGL_PLATFORM_GBM_MESA" \
#     -e DISPLAY \
#     -e QT_X11_NO_MITSHM \
#     -d \
#     -p 5000:5000 \
#     -v ~/.Xauthority:/root/.Xauthority:rw \
#     -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
#     -v $(pwd)/..:/io \
#     -w /io/pip \
#     build-gvxr-$ARCH \
#     /bin/bash
