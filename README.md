## JuPedSim
Simulating pedestrians with state of the art models.
The Jülich Pedestrian Simulator consists of four modules which are loosely
coupled and can be used independently at the moment. These are:

1. `JPScore`: the core module computing the trajectories. At the moment only the [Generalized Centrifugal Force Model](http://arxiv.org/abs/1008.4297) is implemented.
2. `JPSvis`: a tool for visualising the simulation trajectories as well as the input geometries.
3. `JPSreport`: a tool for analysing the trajectories and validating the
model. It implements a couple of measurement methods including the [Voronoi-method](http://dx.doi.org/10.1016/j.physa.2009.12.015) for calculating the density.
4. `JPSed`: a GUI for editing the geometry and the model parameters. Not yet implemented.

## Download

The official web page of the project can be found [here](http://www.jupedsim.org).
