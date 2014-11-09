## JuPedSim
Simulating pedestrians with state of the art models.
The Jülich Pedestrian Simulator ([JuPedSim](http://www.jupedsim.org)) consists of three modules which are loosely
coupled and can be used independently at the moment. These are:

1. `JPScore`: the core module computing the trajectories. At the moment the [Generalized Centrifugal Force Model](http://arxiv.org/abs/1008.4297) is implemented. Another (yet unpublished) model is implemented.
2. `JPSvis`: a tool for visualising the input (geometry) and output (trajectories) data.
3. `JPSreport`: a tool for analysing the trajectories and validating the
model. It implements a couple of measurement methods including the [Voronoi-method](http://dx.doi.org/10.1016/j.physa.2009.12.015) for calculating the density.

## Repository

The code is *actively developed* in a [GitLab repository](https://cst.version.fz-juelich.de/public/projects). Please consider cloning the code their.  To be **notified** of releases and updates please follow us [@JuPedSim](https://twitter.com/JuPedSim).
