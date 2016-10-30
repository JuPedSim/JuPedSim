 [![LGPL](https://img.shields.io/badge/license-GPL-blue.svg)](https://raw.githubusercontent.com/JuPedSim/jpscore/master/LICENSE)[![twitter](http://i.imgur.com/tXSoThF.png)](http://www.twitter.com/JuPedSim)
***
![logo](https://github.com/JuPedSim/JuPedSim/blob/master/doc/jupedsim_small.png?raw=true)

[**documentation**](http://www.jupedsim.org) | [**jpscore**](http://jupedsim.github.io/jpscore) | [**jpsreport**](http://jupedsim.github.io/jpsreport) | [**jpseditor**](https://cst.version.fz-juelich.de/jupedsim/jpseditor) | [**jpsvis**](https://cst.version.fz-juelich.de/jupedsim/jpsvis) | [**docker images**](https://hub.docker.com/u/jupedsim/) | [**contribute**](http://jupedsim.github.io/jpscore/contributing/)
***

## Introduction

Simulating pedestrians with state of the art models.
The Jülich Pedestrian Simulator ([JuPedSim](http://www.jupedsim.org)) consists of three modules which are loosely
coupled and can be used independently at the moment. These are:

1. `jpscore`: the core module computing the trajectories. See [list](http://jupedsim.github.io/jpscore/models/operativ) of implemented models.
2. `jpsreport`: a tool for analyzing the trajectories and validating the
model. It implements a couple of measurement methods including the [Voronoi-method](http://dx.doi.org/10.1016/j.physa.2009.12.015) for calculating the density.
3. `jpsvis`: a tool for visualizing the input (geometry) and output (trajectories) data.
4.  `jpseditor`: a tool for creating and editing geometry files with dxf import/export capabilities.


## Quick Install:

```shell
git clone git@github.com:JuPedSim/JuPedSim.git
cd JuPedSim
git submodule update --init --recursive # this is VERY important, otherwise the submodules will be empty
make -f Makefile.cmake
```
After a successful compilation, the release binaries will be located in the `bin` folder.


## How to use JuPedSim's Docker images

`JuPedSim` offers also up to date Docker images. For these who wonder what the Docker is, here is what Docker.io says:

> Docker is an open-source project to easily create lightweight, portable, self-sufficient containers 
> from any application. 
> The same container that a developer builds and tests on a laptop can run at scale, in production, 
> on VMs, bare metal, OpenStack clusters, public clouds and more.

Basically, we package `JuPedSim` and all its dependencies in a lightweight image, which can just used without any hassle. 

### Linux

For Linux systems please check this excellent blog: 
[Running GUI apps with Docker](http://fabiorehm.com/blog/2014/09/11/running-gui-apps-with-docker/)

### Mac OSX

start `socat` to expose local xquartz socket on a TCP port:

    socat TCP-LISTEN:6000,reuseaddr,fork UNIX-CLIENT:\"$DISPLAY\"

Then in a new window pass the `DISPLAY` with your own IP-address to the container by running

    docker run -it -e DISPLAY=YOUR_IP:0 -v /some/host/folder/:/tmp jupedsim/jupedsim

See also this [issue](https://github.com/docker/docker/issues/8710)

This will mount `/some/host/folder` in docker's `/tmp`, which is useful to exchange data between your machine and the docker container.

Issue: There is an issue after updating XQuartz: `libGL error: failed to load driver: swrast `

It seems that downgrading to 2.7.8 solved the problem. 

 Note: You can get you IP by typing in the terminal `ifconfig`.

### Windows

Works only with Windows 10.

@todo: How? need someone with a windows machine to check this.

## Showcase and tutorials

To highlight some features of `JuPedSim` we have uploaded some videos and tutorials on
our [YouTube channel](https://www.youtube.com/channel/UCKS8w8CUClHEeN4K1SUSMBA).

## Repository and tests

- [GitLab repository](https://cst.version.fz-juelich.de/public/projects). Please consider cloning the code there.

`JuPedSim` is at the moment validated using the [RiMEA Testcases](http://www.rimea.de). 
Also it is still undergoing an extensive unit-testing. 
Validation of the implemented models with respect to experimental data is planned, but only partially implemented.



