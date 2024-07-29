if [ ! -d  $(pwd)/output ]; then
    mkdir  $(pwd)/output
fi

docker build --platform linux/amd64 -t gvxr .

xhost +

docker run \
    --rm -it \
    --ipc=host \
    --gpus all \
    -e NVIDIA_VISIBLE_DEVICES=all -e NVIDIA_DRIVER_CAPABILITIES=display,compute \
    -e EGL_PLATFORM="EGL_PLATFORM_GBM_MESA" \
    -e DISPLAY \
    -e QT_X11_NO_MITSHM \
    --privileged \
    --user $(id -u) \
    --group-add users \
    -d \
    -p 8888:8888 \
    -v $(pwd)/output:/test-gvxr/output \
    -v ~/.Xauthority:/root/.Xauthority:rw \
    -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
    -v $(pwd)/..:/io \
    gvxr
