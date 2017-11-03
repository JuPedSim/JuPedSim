# |---------------------------|
# | - build:                  |
# |   docker build jupedsim . |
# | - run:                    |
# |    docker -it jupedsim    |
# |---------------------------|
    
FROM ubuntu:14.04
 
MAINTAINER Mohcine Chraibi <m.chraibi@gmail.com>

# install required packages
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    g++ \
    git \
    lcov \
    doxygen \
    wget \
    libbz2-dev \ 
    python \
    python3-setuptools \
    python-dev \
    python-pip \
    python3-pip \
    qt5-default \
    qttools5-dev-tools \
    libvtk5-dev \
    libvtk5-qt4-dev \
    && apt-get clean

# set environment
ENV HOME /home/jupedsim

ARG boost_version=1.61.0
ARG boost_dir=boost_1_61_0
ENV boost_version ${boost_version}
ENV BOOST_ROOT /usr
RUN wget http://downloads.sourceforge.net/project/boost/boost/${boost_version}/${boost_dir}.tar.gz \
    && tar xfz ${boost_dir}.tar.gz \
    && rm ${boost_dir}.tar.gz \
    && cd ${boost_dir} \
    && ./bootstrap.sh --with-libraries=filesystem,test,system\
    && ./b2 --without-python --prefix=/usr -j 4 link=shared runtime-link=shared install \
    && cd .. && rm -rf ${boost_dir} && ldconfig

# add user
RUN groupadd -r -g 1000 jupedsim && useradd -r -g jupedsim -u 1000 -m jupedsim

USER jupedsim    

# install JuPedSim
RUN cd /home/jupedsim \
    && mkdir Desktop \
    && mkdir Desktop/JPSvis_Files \
    && mkdir workspace \
    && cd workspace \
    && git clone --depth=1 https://github.com/JuPedSim/JuPedSim.git \
    && cd JuPedSim \
    && git submodule update --init --recursive \
    && make -f Makefile.cmake
    
# cp binaries to /usr/bin
USER root

RUN cp /home/jupedsim/workspace/JuPedSim/bin/jpscore /usr/bin
RUN cp /home/jupedsim/workspace/JuPedSim/bin/jpseditor /usr/bin
RUN cp /home/jupedsim/workspace/JuPedSim/bin/jpsvis /usr/bin
RUN cp /home/jupedsim/workspace/JuPedSim/bin/jpsreport /usr/bin

USER jupedsim

# RUN usermod -p `perl -e "print crypt("qw","Q4")"` root
# install python variant filtering dependencies
RUN pip3 install numpy
RUN pip3 install matplotlib
RUN pip3 install Polygon3

# by default /bin/bash is executed
CMD /bin/bash && echo "Thank you for using JuPedSim!"
