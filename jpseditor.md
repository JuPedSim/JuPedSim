JPSeditor
====================

Erik Andresen drafted this on 27 Jun 2016

`JPSeditor` is a graphical user interface to create the geometry of a scenario simulated by JuPedSim. It comes with set of CAD- and further tools to simplify the creation of proper xml-files 
incorporating information about the scenario' geometry.


Features
========

- DXF Import / Export
- CAD features
	- Point and line grid
	- object snap
	- Orthomode
	- Zooming 
	- Line editing
	- ...
- Tools for room and door declarations


Tutorial
========

To highlight some features of JuPedSim we have uploaded some videos on our [YouTube channel](https://www.youtube.com/channel/UCKS8w8CUClHEeN4K1SUSMBA) including a tutorial showing how to use the editor.


Installing
==========

This version comes with no installer. 
So just download the archive corresponding to your architecture and unzip them and you are ready to go. 
The binaries are only available for windows at the moment. 
For other architectures (Linux, OSX) you will need to compile the sources. 
See the section [Compiling from sources](#compiling-from-sources).

Uninstalling
============

As JuPedSim comes with no installer, you just need to delete the unziped directory.

Running
=======

Start the application by clicking on the binary file. (For example .exe)

System Requirements
==============

There is no special hardware/software requirements for running JuPedSim. 
The distributed binaries however, are only available for windows at the moment. 
For compiling from sources you need a compiler with c++11 support is needed for the core. 
Qt version >=4.5 and VTK >5.8 are needed for the visualization.



Frequently Asked Questions
===========================

- What is the official page of JuPedSim?
www.jupedsim.org and the contact is info at jupedsim.org. You will find more information on the working group and other tools and experimental pedestrians data we have been collecting over the years.

- Where is the official repository?
JuPedSim is developed at the Forschungszentrum Jülich in Germany and the bleeding edge code is in their intern git repository. At the moment only specific tags are pushed to GitHub.

- Is there a manual?
Of course, the user's guide is found in the downloaded archive.

- Are the models validated?
We are actually setting up verification and validation tests. Some verification tests are included in this version but most of them will be available with the next version. This is an ongoing process.

- How can I contribute to JuPedSim?
Testing and reporting bugs will be great. If you may want to contribute actively to the code, by implementing new models and/or features, you are welcome to do so.