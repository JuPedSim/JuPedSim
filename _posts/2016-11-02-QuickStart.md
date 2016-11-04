---
layout: post
title: Quick start
subtitle: Compiling the code
---

To install `JuPedSim` there are three steps to follow: 

- [Download the code](#get-the-code) 
- [Check the requirements](#check-the-requirements)
- [Compile](#compile-the-code)




# Get the code

```bash
git clone https://github.com/JuPedSim/JuPedSim.git
cd JuPedSim
git submodule update --init --recursive
```

# Check the requirements 
Some packages and libraries are required to get `JuPedSim` compiled. See also [requirements](/2016-11-03-Requirements).

You can check your system with 

```bash 
make -f Makefile.cmake check
```

# Compile the code

If the two first steps succeed then you can proceed with the compilation 

```bash 
make -f Makefile.cmake
```

In case of successful compilation,  the executables will be located in the directory `bin/`.
