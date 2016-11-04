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

