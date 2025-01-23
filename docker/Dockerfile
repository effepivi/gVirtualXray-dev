# set base image (host OS)
FROM ghcr.io/tomographicimaging/cil:master

# Change the user to install a package
USER root

# EGL
RUN apt-get update
RUN apt-get install -y  libegl1-mesa

# For debugging
# RUN apt-get install -y mesa-utils-extra #xterm

# Change the user
USER "${NB_USER}"

# Change the workdir
WORKDIR /gvxr

# Install Python packages
RUN python3 -m pip install --user gVXR spekpy k3d

# For debugging
# CMD xterm