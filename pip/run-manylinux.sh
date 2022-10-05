#!/usr/bin/sh

docker pull quay.io/pypa/manylinux2014_x86_64
docker run -ti -v $(pwd)/..:/io -w /io/pip quay.io/pypa/manylinux2014_x86_64 /bin/bash
