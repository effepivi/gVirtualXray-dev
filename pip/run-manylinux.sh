#!/usr/bin/sh

MANYLINUX=manylinux2014_x86_64

docker pull quay.io/pypa/$MANYLINUX
docker run -ti -v $(pwd)/..:/io -w /io/pip quay.io/pypa/$MANYLINUX /bin/bash
