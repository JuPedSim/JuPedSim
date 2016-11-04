---
layout: post
title: Compiling from source
subtitle: Dependencies for compiling
#bigimg: /img/jupedsim_small.png
---

In order to compile `JuPedSim` it is necessary to first install the required libraries. 

## Supported compilers

Any compiler with support for C++11. 

Tested with   

- `g++` (linux/osx)
- `clang` (osx)
- Visual Studio 2013 (Windows)

## Required tools 

- cmake: see this [tutorial](https://www.johnlamp.net/cmake-tutorial-1-getting-started.html) for a brief overview.  
- Python (highly recommended): needed to plot results of analysis. 

## Required libraries 

- [Boost library](http://www.boost.org/)
- [Qt](https://www.qt.io/): necessary for `jpseditor` and `jpsvis`
- [VTK](http://www.vtk.org/): necessary for `jpsvis`

***

Note:
`jpseditor` and `jpsvis` come with Windows executables and  `dmg`-files for Mac users. 
Therefore, a compilation of these two packages may not be needed.


## Install packages for Mac OS X  (El Capitan)

Assuming you have [Homebrew](http://brew.sh/). Otherwise, these commands 
can be slightly adapted to [MacPorts](https://www.macports.org/)

```
brew update
# qt
brew install qt
# vtk
brew install vtk --with-qt --without-python --with-python3 --without-boost  --build-from-source
```

### Install packages for Debian 7.7 (Ubuntu)

```
sudo apt-get install libmgl-qt5
sudo apt-get install libvtk5-dev
sudo apt-get install libvtk5.8-qt4
```

Alternatively, you can download the latest version of QT [here](https://www.qt.io/download/)
and the latest version of VTK [here](http://www.vtk.org/download/) and compile them manually.

## Boost at least 1.57

You can compile boost as follows: 

```bash
boost_version=1.61.0
boost_dir=boost_1_61_0

wget http://downloads.sourceforge.net/project/boost/boost/${boost_version}/${boost_dir}.tar.gz \
    && tar xfz ${boost_dir}.tar.gz \
    && rm ${boost_dir}.tar.gz \
    && cd ${boost_dir} \
    && ./bootstrap.sh --with-libraries=filesystem,test,system\
    && ./b2 --without-python --prefix=/usr -j 4 link=shared runtime-link=shared install \
    && cd .. && rm -rf ${boost_dir} && ldconfig
```

or if you are a `brew` user :

````
brew install boost
```

***

Note: Debian and Ubuntu have an old version of Boost, which is not supported by `JuPedSIm`. Therefore, you need compile it. 
