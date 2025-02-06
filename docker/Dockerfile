# set base image (host OS)
#FROM ghcr.io/tomographicimaging/cil:master
FROM ghcr.io/tomographicimaging/cil:24.3.0

# Change the user to install a package
USER root

# EGL
RUN apt-get update
RUN apt-get install -y  libegl1-mesa libnvidia-gl-550-server openscad
RUN apt-get install -y mesa-utils mesa-utils-extra #mesa-demos

# For debugging
# RUN apt-get install -y mesa-utils-extra #xterm

# Change the user
USER "${NB_USER}"

# Change the workdir
WORKDIR /gvxr

# Install Python packages
RUN python3 -m pip install --user gVXR viewscad SimpleITK progressbar

# For debugging
# CMD xterm
