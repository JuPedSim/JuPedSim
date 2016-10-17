<a name="top"></a>
<a href="https://raw.githubusercontent.com/JuPedSim/jpscore/master/LICENSE"><img src="https://img.shields.io/badge/license-GPL-blue.svg" alt="LGPL" align="right"></a> <a href="http://www.twitter.com/JuPedSim"><img src="http://i.imgur.com/tXSoThF.png" alt="Twitter" align="right"></a><br>
***
<p align="center"><img src="/doc/jupedsim_small.png" alt="JuPedSim"/></p>
<p align="center">
<b><a href="http://www.jupedsim.org">documentation</a></b>
|
<b><a href="http://jupedsim.github.io/jpscore/">jpscore</a></b>
|
<b><a href="http://jupedsim.github.io/jpsreport/">jpsreport</a></b>
|
<b><a href="https://cst.version.fz-juelich.de/jupedsim/jpseditor">jpseditor</a></b>
|
<b><a href="https://cst.version.fz-juelich.de/jupedsim/jpsvis">jpsvis</a></b>
|
<b><a href="https://hub.docker.com/u/jupedsim/">docker images</a></b>
|
<b><a href="http://jupedsim.github.io/jpscore/contributing/">contribute</a></b>
</p>
***

**Introduction**

Simulating pedestrians with state of the art models.
The Jülich Pedestrian Simulator ([JuPedSim](http://www.jupedsim.org)) consists of three modules which are loosely
coupled and can be used independently at the moment. These are:

1. `JPScore`: the core module computing the trajectories. See [list](http://jupedsim.github.io/jpscore/models/operativ) of implemented models.
2. `JPSvis`: a tool for visualising the input (geometry) and output (trajectories) data.
3. `JPSreport`: a tool for analysing the trajectories and validating the
model. It implements a couple of measurement methods including the [Voronoi-method](http://dx.doi.org/10.1016/j.physa.2009.12.015) for calculating the density.
4.  `JPSeditor`: a tool for creating and editing geometry files with dxf import/export capabilities.


**Quick Install:**

```shell
git clone git@github.com:JuPedSim/JuPedSim.git
cd JuPedSim
git submodule update --init --recursive
make -f Makefile.cmake
```
After a successful compilation, the release binaries will be located in the bin folder.

**Showcase and tutorials**

To highlight some features of `JuPedSim` we have uploaded some videos and tutorials on
our [YouTube channel](https://www.youtube.com/channel/UCKS8w8CUClHEeN4K1SUSMBA).

**Repository and tests**

- [GitLab repository](https://cst.version.fz-juelich.de/public/projects). Please consider cloning the code there.

`JuPedSim` is at the moment validated using the [RiMEA Testcases](http://www.rimea.de). Also it is stillundergoing an extensive unit-testing. 
Validation of the implemented models with respect to experimental data is planned, but only partially implemented.



