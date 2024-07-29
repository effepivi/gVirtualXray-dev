# set base image (host OS)
FROM nvidia/opengl:1.2-glvnd-devel-ubuntu20.04

# Set timezone:
RUN ln -snf /usr/share/zoneinfo/$CONTAINER_TIMEZONE /etc/localtime && echo $CONTAINER_TIMEZONE > /etc/timezone

# Update the installation
RUN apt-get update
RUN apt-get install -y apt-utils

# Install dependencies:
RUN apt-get install -y tzdata
RUN apt-get install -y vim vim-gtk3
RUN apt-get install -y xterm
RUN apt-get update
RUN apt-get install -y wget curl

RUN apt-get update
RUN apt-get install -y build-essential \
    g++ \
    swig \
    libglu1-mesa-dev \
    freeglut3-dev \
    mesa-common-dev \
    libgsl-dev \
    doxygen \
    libgbm-dev \
    libxrandr-dev libxcursor-dev libxxf86vm-dev libxinerama-dev libxi-dev \
    zlib1g-dev

RUN apt-get update

# Install cmake
RUN mkdir /tmp/cmake
WORKDIR /tmp/cmake
RUN wget --quiet https://github.com/Kitware/CMake/releases/download/v3.30.1/cmake-3.30.1-linux-x86_64.tar.gz
RUN tar zxvfp cmake-3.30.1-linux-x86_64.tar.gz
ENV PATH="/tmp/cmake/cmake-3.30.1-linux-x86_64/bin:${PATH}"



# Install Miniconda on x86 or ARM platforms
RUN arch=$(uname -m) && \
    if [ "$arch" = "x86_64" ]; then \
        MINICONDA_URL="https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh"; \
    elif [ "$arch" = "aarch64" ]; then \
        MINICONDA_URL="https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-aarch64.sh"; \
    else \
        echo "Unsupported architecture: $arch"; \
    exit 1; \
    fi && \
    wget $MINICONDA_URL -O miniconda.sh && \
    mkdir -p /root/.conda && \
    bash miniconda.sh -b -p /opt/conda && \
    rm -f miniconda.sh && \
    echo "export PATH=/opt/conda/bin:$PATH" > /etc/profile.d/conda.sh

RUN apt-get install -y git

ENV PATH /opt/conda/bin:$PATH

RUN conda --version

RUN mkdir /workdir
COPY environment.yml /workdir
RUN conda env create -f /workdir/environment.yml

#RUN git clone git://git.code.sf.net/p/gvirtualxray/gitroot gvirtualxray-gitroot






RUN echo "source activate gVXR" > ~/.bashrc
ENV PATH /opt/conda/envs/gVXR/bin:$PATH

# RUN pip install /gvirtualxray-gitroot
# RUN rm -rf /io/_skbuild ; pip install /io

# RUN git clone git://git.code.sf.net/p/gvirtualxray/gitroot /tmp/gvirtualxray-gitroot
# RUN pip install /tmp/gvirtualxray-gitroot



RUN useradd -rm -d /home/gvxr -s /bin/bash -g users -u 1000 gvxr
# RUN chown gvxr:users -R  /gvirtualxray-gitroot
USER gvxr
WORKDIR /home/gvxr
RUN mkdir /home/gvxr/code
RUN mkdir /home/gvxr/data
RUN mkdir /home/gvxr/results

COPY code/dragon-with-JSON.ipynb /home/gvxr/code/
COPY code/dragon-without-JSON.ipynb /home/gvxr/code/   
COPY results/dragon-comparison.png  /home/gvxr/results/
COPY results/dragon-detector-energy_response.png /home/gvxr/results/
COPY results/dragon-detector-LSF.png /home/gvxr/results/
COPY results/dragon-beam-spectrum.png /home/gvxr/results/
COPY results/dragon-wireframe.png /home/gvxr/results/



RUN echo "source activate gVXR" > ~/.bashrc
ENV PATH /opt/conda/envs/gVXR/bin:$PATH

# RUN pip install /gvirtualxray-gitroot
# RUN rm -rf /io/_skbuild ; pip install /io

EXPOSE 8888

# CMD  xterm #bash python3 -m gvxrPython3.test
CMD ["jupyter", "lab", "--port=8888", "--ip=0.0.0.0", "--NotebookApp.token='gvxr'", "--preferred-dir=/home/gvxr/code"]