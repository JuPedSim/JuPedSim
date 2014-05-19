JuPedSim
========

Simulating pedestrians with state of the art models.

![image](./figures/logo.png)\

Jülich Pedestrian Simulator\

Forschungszentrum Jülich GmbH\

\
<span> **User’s Guide Version 0.5**</span>\

\

<span>0.4</span>

*Authors:*\
Ulrich Kemloh\
Mohcine Chraibi\
Jun Zhang\

<span>0.4</span>

JuPedSim
========

The jülich Pedestrian Simulator (JuPedSim) is a tool for simulating and
analysing pedestrians streams. It consists (at the moment) of three
modules which are loosely coupled and can be used independently at the
moment. These are:

1.  **JPScore**: the core module computing the trajectories

2.  **JPSvis**: the tool for visualising the trajectories

3.  **JPSreport**: the tool for analysing the trajectories and
    validating the model

The description and use of each module is presented in the next
chapters. All modules can be downloaded and compiled using CMake. For
convenience binaries for windows are provided.

JPScore
=======

The core module for computing the trajectories can be initialized with a
list of files or by passing all needed parameters via the command line.
But it is encourage to use the initialisation files. In the following
JPScore denotes the executable.

Project file (ini)
------------------

A program call looks like this:

    > JPScore.exe --inifile=ini.xml

The typical content of a project file (ini.xml) is described in
 [lst:inifile].

    <?xml version="1.0" encoding="UTF-8" ?>

    <JuPedSim project="JPS-Project" version="0.5">

        <!-- seed , geometry, output format -->
        <header> </header>
        
        <!-- traffic information: e.g closed doors or smoked rooms -->
        <traffic_constraints>   </traffic_constraints>
        
        <!-- goals outside the geometry-->
        <routing>   <goals/>    </routing>

        <!--persons information and distribution -->
        <agents>
            <agents_distribution></agents_distribution>
        </agents>

        <!-- These parameters may be overwritten -->
        <operational_models>
            <model id="1" description="gcfm">
            </model>
        </operational_models>

        <route_choice_models>
            <router router_id="1" description="global_shortest">
            </router>
        </route_choice_models>
        
    </JuPedSim>

### Header

The header comprises the following elements:

-   <seed>12542</seed> 

    The seed used for the random number generator. If missing the
    current time, time(NULL), is used.

-   <max_sim_time>200</max_sim_time>

    The maximal simulation time in seconds. The default value is 900
    seconds.

-   <trajectories format="xml-plain"  fps="8">
                <file location="corner_traj.xml" />
            <!--    <socket hostname="127.0.0.1" port="8989"/> -->
            </trajectories> 

    ##### format

    The options for the format are

    -   xml-plain: the default xml format. It can lead to large files.

    -   plain: simple text format

    -   vtk: a format compatible with paraview
        (<http://www.paraview.org>)

    A detailed description of the formats are presented in the next
    section.

    ##### fps

    defines the frame rate per second for the trajectories. Note that
    the step size for the simulation usually varies from 0.001 to 0.1

    ##### file location

    defines the location of the trajectories. All paths are relative to
    the location of the project file.

    ##### Streaming

    Optional to a file location a network address (ip/hostname and port)
    for streaming the results can also been provided.

-   <geometry>corner_geo.xml</geometry>

    The location of the geometry file. All files locations are relative
    to the actual location of the project file.

### Traffic constraints

This section defines constraints related to the traffic. At the moment
doors can be open or close.

``` {caption="Sample" traffic="" constraints="" section=""}
<traffic_constraints>
        <!-- doors states are: close or open -->
        <doors>
            <door trans_id="4" caption="Main-gate" state="open" />
            <door trans_id="6" caption="Rear-gate" state="close" />
        </doors>
    </traffic_constraints>
```

##### trans\_id

The unique ID of that door as defined in the geometry file (see section
 [sec:geometry]).

##### caption

optional parameter defining the caption of the door.

##### state

defines the state of the door. Options are **close** or **open**.

### Goal definitions (routing) {#ssec:goals}

Additional goals might be defined *outside* the geometry. They should
not overlap with any walls or be in a rooms. The best practice is to
position them near to exits. Goals are defined with close polygons, i.e
the last vertex is equal to the first one.

``` {caption="Sample" goals="" definition=""}
<routing>
        <goals>
            <goal id="0" final="false" caption="goal 1">
                <polygon>
                    <vertex px="-5.0" py="-5.0" />
                    <vertex px="-5.0" py="-2.0" />
                    <vertex px="-3.0" py="-2.0" />
                    <vertex px="-3.0" py="-5.0" />
                    <vertex px="-5.0" py="-5.0" />
                </polygon>
            </goal>
            <goal id="1" final="false" caption="goal 2">
                <polygon>
                    <vertex px="15.0" py="-5.0" />
                    <vertex px="17.0" py="-5.0" />
                    <vertex px="17.0" py="-7.0" />
                    <vertex px="15.0" py="-7.0" />
                    <vertex px="15.0" py="-5.0" />
                </polygon>
            </goal>
        </goals>
    </routing>
    
```

### Agents

An example how to define agents characteristics with different number of
attributes are given in  [lst:agents].

    <agents>
            <agents_distribution>
                
                <group group_id="1" room_id="0" number="10" />
                
                <group group_id="2" room_id="0" subroom_id="0" number="10" 
                       goal_id="" router_id="1" />
                
                <group group_id="3" room_id="0" subroom_id="1" number="20" 
                       goal_id="1" router_id="2" patience="5"/>
                
                <group group_id="4" room_id="0" number="200" goal_id="-1" 
                       router_id="2" patience="50"/>
                            
                <group group_id="5" room_id="0" number="1" startX="25.3" startY="365.90"/>

            </agents_distribution>
        </agents>

##### group\_id

mandatory parameter defining the unique id of that group.

##### room\_id

mandatory parameter defining the room where the agents should be
randomly distributed.

##### number

mandatory parameter defining the number of agents to distribute.

##### subroom\_id

defines the id of the subroom where the agents should be distributed. If
omitted then the agents are homogeneously distributed in the room. See
the geometry section.

##### goal\_id

one of the *id* defined in the section  [ssec:goals]. If omitted or “-1”
then the shortest to the outside is chosen by the agent.

##### router\_id

defines the route choice model to used. Those models are defined in the
section  [ssec:route~c~hoice~m~odels]

##### patience

This parameter influences the route choice behavior when using the
quickest path router. It basically defines how longs a pedestrian stays
in jams before attempting a rerouting.

##### startX, startY

define the initial coordinate of the agents. This might be useful for
testings. Note that this coordinate is ignored if *number* is different
from 1.

### Operative models

#### GCFM

You will find a description of this model with the calibration
parameters in: M. Chraibi und A. Seyfried, Generalized centrifugal-force
model for pedestrian dynamic, Physical Review E 82, 046111 (2010).\

     [caption=]
        <operational_models>
            <model id="1" description="gcfm">
                <parameters>
                    <solver>euler</solver>
                    <stepsize>0.01</stepsize>
                    <exitCrossingStrategy>4</exitCrossingStrategy>
                    <linkedcells enabled="true" cell_size="2.2" />
                    <v0 mu="1.24" sigma="0.001" />
                    <bmax mu="0.25" sigma="0.001" />
                    <bmin mu="0.20" sigma="0.001" />
                    <amin mu="0.18" sigma="0.001" />
                    <tau mu="0.5" sigma="0.001" />
                    <atau mu="0.5" sigma="0.001" />
                    <force_ped nu="0.3" dist_max="3" disteff_max="2" interpolation_width="0.1" />
                    <force_wall nu="0.2" dist_max="3" disteff_max="2" interpolation_width="0.1" />
                </parameters>
            </model>
        </operational_models>

    <solver>euler</solver>

    <stepsize>0.001</stepsize>

At the moment the Euler scheme is used to solve the differential
equation resulting from the agents motion. Be careful not to used a
larger step size, the system might become unstable. Recommended values
ranges [0.001 0.1]. The step size is in second.

    <exitCrossingStrategy>4</exitCrossingStrategy>

This defines how a pedestrian crosses a line. The options are:

-   1:

-   2:

-   3:

-   4:

-   5:

An attempt is being done to combine all the strategies into one general.

    <linkedcells enabled="true" cell_size="2.2" />

The linked-cells enhance the runtime of the simulation. Choose false or
a large cell\_size if you want to disable this feature.

$ \sim  \mathcal{N}(\mu,\sigma)$.\
The desired velocity $v_0$ is gaussian distributed between
$[\mu-\sigma, \mu +\sigma]$ with the parameters $\mu$ and $\sigma$.
$\mu$ defaults to 1.24

$ \sim  \mathcal{N}(\mu,\sigma)$.\
the length of the longitudinal axis is defined by:
$b_{max} - v * (b_{max} - b_{min}) / v_0$. The maximal value is
$b_{max}$. is gaussian distributed between $[\mu-\sigma, \mu +\sigma]$
with the parameters $\mu$ and $\sigma$. $\mu$ defaults to 0.25

*Default: 0.2*\
Länge der zweiten Halbachse der Ellipse ist:
$b_{max} - v * (b_{max} - b_{min}) / v_0$. Die minimale Ausdehnung
dieser Halbachse ist $b_{min}$. Diese Größe ist gleichverteilt auf
$[\mu-\sigma, \mu +\sigma]$ mit den Parametern $\mu$ und $\sigma$.
$b_{min} \sim  \mathcal{N}(\mu,\sigma)$. Durch diese Option wird $\mu$
gesetzt.

*Default: 0.001*\
Länge der zweiten Halbachse der Ellipse ist:
$b_{max} - v * (b_{max} - b_{min}) / v_0$. Die minimale Ausdehnung
dieser Halbachse ist $b_{min}$. Diese Größe ist gleichverteilt auf
$[\mu-\sigma, \mu +\sigma]$ mit den Parametern $\mu$ und $\sigma$.
$b_{min} \sim  \mathcal{N}(\mu,\sigma)$. Durch diese Option wird
$\sigma$ gesetzt.

*Default: 0.5*\
Länge der ersten Halbachse der Ellipse ist: $a_{min} + v * a_{\tau}$.
Die Faktor mit der Geschwindigkeit ist $a_{\tau}$. Diese Größe ist
gleichverteilt auf $[\mu-\sigma, \mu +\sigma]$ mit den Parametern $\mu$
und $\sigma$. $a_{\tau} \sim  \mathcal{N}(\mu,\sigma)$. Durch diese
Option wird $\mu$ gesetzt.

*Default: 0.001*\
Länge der ersten Halbachse der Ellipse ist: $a_{min} + v * a_{\tau}$.
Die Faktor mit der Geschwindigkeit ist $a_{\tau}$. Diese Größe ist
gleichverteilt auf $[\mu-\sigma, \mu +\sigma]$ mit den Parametern $\mu$
und $\sigma$. $a_{\tau} \sim  \mathcal{N}(\mu,\sigma)$. Durch diese
Option wird $\sigma$ gesetzt.

*Default: 0.18*\
Länge der ersten Halbachse der Ellipse ist: $a_{min} + v * a_{\tau}$.
Die minimale Ausdehnung dieser Halbachse ist $a_{min}$. Diese Größe ist
gleichverteilt auf $[\mu-\sigma, \mu +\sigma]$ mit den Parametern $\mu$
und $\sigma$. $a_{min} \sim  \mathcal{N}(\mu,\sigma)$. Durch diese
Option wird $\mu$ gesetzt.

*Default: 0.001*\
Länge der ersten Halbachse der Ellipse ist: $a_{min} + v * a_{\tau}$.
Die minimale Ausdehnung dieser Halbachse ist $a_{min}$. Diese Größe ist
gleichverteilt auf $[\mu-\sigma, \mu +\sigma]$ mit den Parametern $\mu$
und $\sigma$. $a_{min} \sim  \mathcal{N}(\mu,\sigma)$. Durch diese
Option wird $\sigma$ gesetzt.

*Default: 0.5*\
Die Reaktionszeit eines Fußgängers ist $\tau$. Diese Größe ist
gleichverteilt auf $[\mu-\sigma, \mu +\sigma]$ mit den Parametern $\mu$
und $\sigma$. $\tau \sim  \mathcal{N}(\mu,\sigma)$. Durch diese Option
wird $\mu$ gesetzt.

*Default: 0.001*\
Die Reaktionszeit eines Fußgängers ist $\tau$. Diese Größe ist
gleichverteilt auf $[\mu-\sigma, \mu +\sigma]$ mit den Parametern $\mu$
und $\sigma$. $\tau \sim  \mathcal{N}(\mu,\sigma)$. Durch diese Option
wird $\sigma$ gesetzt

### Route choice models {#ssec:route_choice_models}

#### Global shortest path

At the beginning of the simulation, the Dijkstra algorithm is used to
build a network which is then cached and used through the simulation
life time.

#### Quickest path

In the quickest path, the peds are rerouted based on actual traffic
conditions. This defaults to the global shortest path when the patience
time of the agents are very large. Detailed information about the models
are presented in:\
A. U. Kemloh Wagoum, A. Seyfried, and S. Holl, “Modelling dynamic route
choice of pedestrians to assess the criticality of building evacuation,”
Advances in Complex Systems, vol. 15, no. 3, 2012.

### Sample project file

A concrete example is given in:

    <?xml version="1.0" encoding="UTF-8" ?>

    <JuPedSim project="JPS-Project" version="0.5" 
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" >

        <!-- seed used for initialising random generator -->
        <seed>12542</seed>
        <max_sim_time>200</max_sim_time>
        <!-- geometry file -->
        <geometry>bottleneck_geo.xml</geometry>
        <!-- traectories file and format -->
        <trajectories format="xml-plain"  fps="8">
            <file location="bottleneck_traj.xml" />
        </trajectories>
        <!-- where to store the logs -->
        <logfile>log</logfile>

        <!-- traffic information: e.g closed doors or smoked rooms -->
        <traffic_constraints>
            <!-- doors states are: close or open -->
            <doors>
                <door trans_id="2" caption="" state="open" />
            </doors>
        </traffic_constraints>
        
        <routing>
            <goals>
                <goal id="0" final="true" caption="goal 1">
                    <polygon>
                        <vertex px="70" py="101" />
                        <vertex px="70" py="103" />
                        <vertex px="75" py="103" />
                        <vertex px="75" py="101" />
                        <vertex px="70" py="101" />
                    </polygon>
                </goal>
            </goals>
        </routing>

        <!--persons information and distribution -->
        <agents>
            <agents_distribution>
                <group group_id="1" room_id="1" number="40" goal_id="0" router_id="1" />
            </agents_distribution>
        </agents>

        <!-- These parameters may be overwritten -->
        <operational_models>
            <model id="1" description="gcfm">
                <parameters>
                    <solver>euler</solver>
                    <stepsize>0.01</stepsize>
                    <exitCrossingStrategy>4</exitCrossingStrategy>
                    <linkedcells enabled="true" cell_size="2.2" />
                    <v0 mu="1.24" sigma="0.001" />
                    <bmax mu="0.25" sigma="0.001" />
                    <bmin mu="0.20" sigma="0.001" />
                    <amin mu="0.18" sigma="0.001" />
                    <tau mu="0.5" sigma="0.001" />
                    <atau mu="0.5" sigma="0.001" />
                    <force_ped nu="0.3" dist_max="3" disteff_max="2" interpolation_width="0.1" />
                    <force_wall nu="0.2" dist_max="3" disteff_max="2" interpolation_width="0.1" />
                </parameters>
            </model>
        </operational_models>

        <route_choice_models>
            <router router_id="1" description="global_shortest">
                <parameters>
             <navigation_lines file="routing.xml" />
                </parameters>
            </router>
        </route_choice_models>
    </JuPedSim>

Geometry {#sec:geometry}
--------

Trajectories
------------

Possible formats are:

-   **xml-plain**: the default xml format

-   **plain** : a flat format (just numbers)

-   **vtk**: a vtk compatible format to be used for example with
    paraview.

A detailed description of the different formats is found in the last
chapter of this manual. Either a file name or a socket muss be
specified, otherwise no trajectories will be written.

JPSreport
=========

This module focuses on the analysis of pedestrian characteristics based
on trajectories obtained from video recordings as well as simulations.

Configuration
-------------

An open source library (Boost C++) is necessary in this module, which is
used to obtain Voronoi diagram from positions of pedestrians and has
been already included in the source file!

~~Version: Boost C++ Libraries Version 1.53.0~~

~~Download: <http://www.boost.org/users/history/version_1_53_0.html>~~

Initial files
-------------

 Input.xml: 
:   defines the measurement method, variations to be analyzed,
    measurement area etc.

 geo.xml:
:   define geometry polygon to determine the boundary for analysis

 Trajectory.xml:
:   trajectories will be analyzed.

A small program for transferring trajectory data from format ’.txt’ to
’.xml’:

* Command: run txt2xml.py -s 0 -f 16*

Command line arguments
----------------------

The content of input.xml typically looks like this:

``` {mathescape=""}

<?xml version="1.0" encoding="UTF-8"?>

<JPSreport project="JPS-Project" version="0.4">
    <!-- geometry file -->
    <geometry file = "./Testing/geo_BOT360.xml" />
    <!-- trajectories file and format -->
    <!-- either a file name or a path location. In the latter case all files in the directory will be used-->
    <trajectories>
        <file name="bot-360-160-160.xml" />
        <path location="../JPSdata/data/BO/BOT360/" />
    </trajectories>     <!-- all files in the directories will be used -->
    <measurementAreas unit="cm">
        <area_B id="1" type="BoundingBox">
            <p1 x="-400" y="0" /> <!-- CCW -->
            <p2 x="-400" y="360" />
            <p3 x="400" y="360" />
            <p4 x="400" y="0" />
            <movingDirection start="p1" end="p4" />
        </area_B>
        <area_L id="2" type="Line">
            <start x="400" y="0" />
            <end x="400" y="360" />
        </area_L>
    </measurementAreas>
    <velocity>
        <useXComponent>true</useXComponent>
        <useYComponent>false</useYComponent>

        <!-- half of the time interval that used to calculate instantaneous velocity of ped i [fr] here v_i = (X(t+deltaF) - X(t+deltaF))/(2*deltaF). X is location. -->
        <halfFrameNumberToUse>5</halfFrameNumberToUse>
    </velocity>
    <!-- Method A (Zhang2011a) Flow and Vel -->
    <method_A enabled="false">
        <!-- Time interval used to count the flow [fr] -->
        <timeInterval unit="frame">
            70
        </timeInterval>
        <!-- The coordinate of the line used to calculate the flow and velocity -->
        <measurementArea id="2" />
    </method_A>
    <!-- Method B (Zhang2011a) Vel and Dens based on Tin and Tout -->
    <method_B enabled="false">
        <measurementArea id="1" />
    </method_B>
    <!-- Method C (Zhang2011a) Classical density and Vel -->
    <method_C enabled="false">
        <measurementArea id="1" />
    </method_C>
    <!-- Method D (Zhang2011a) Voronoi density and Vel -->
    <method_D enabled="true" cutByCircle="false" outputGraph="false"
        individualFDdata="true">
        <measurementArea id="1" />
        <steadyState start="500" end="1500" /> <!-- //the begin of stationary state //the end of stationary state -->
        <getProfile enabled="false" scale_x="10" scale_y="10"/>
    </method_D>
</JPSreport>
```

-   ***Measurement areas:***\
    Two different measurement areas can be chosen corresponding to
    different measurement methods:

    $\langle$<span>**BoundingBox**</span>$\rangle$
    :   \
        Bounding box can be either rectangle or polygon. The vertexes of
        the box should be given one by one in clockwise.

    $\langle$<span>**Line**</span>$\rangle$
    :   \
        The line is only used in Method A (see below).

-   ***Velocity:***\

    1.  If only $\langle$<span>**useXComponent**</span>$\rangle$ is “”,
        the velocity in x direction is used in analysis.

    2.  If only $\langle$<span>**useYComponent**</span>$\rangle$ is “”,
        the velocity in y direction is used in analysis.

    3.  If both $\langle$<span>**useXComponent**</span>$\rangle$ and
        $\langle$<span>**useYComponent**</span>$\rangle$ are “”, the
        velocity in 2D space is used in analysis.

    4.  $\langle$<span>**halfFrameNumberToUse**</span>$\rangle$is half
        of the number of frames used to calculate the instantaneous
        velocity. Since the instantaneous velocity is calculated
        according to

        $$\label{eq2}
                v_i(t)=\frac{{x_i}(t+\Delta t^\prime/2)-{x_i}(t-\Delta
                t^\prime/2))}{\Delta t^\prime}$$

        !!!Note that!!! at least one of the above variables should be
        “true” during the analysis.

-   ***Measurement Methods (A, B, C, D): refer to reference
    [zhang2011]***\

    1.  $\langle$<span>**Method A**</span>$\rangle$

        <span>**Input parameters:**</span>

        a\) Location of reference line (Start point and End point)

        b\) Value of time interval (frame)

        <span>**Output data:**</span>

        File 1: N-t (the accumulative number of pedestrians passing the
        reference line and the corresponding time in frame)

        File 2: the mean flow and velocity over the given time interval.

        Note that: All the obtained data is based on the same reference
        line.

    2.  $\langle$<span>**Method B**</span>$\rangle$

        <span>**Input parameters:**</span>

        a\) Rectangle measurement area (p1 (x, y), p2 (x, y), p3 (x, y), p4 (x,
        y))

        b\) Moving direction (e.g. from start point (p1) to end (p4))

        <span>**Output data:**</span> mean density and velocity of each
        pedestrian ($\rho_i$ and $v_i$).

    3.  $\langle$<span>**Method C**</span>$\rangle$

        <span>**Input parameters:**</span> measurement area

        <span>**Output data:**</span> mean density and velocity over
        time ($\rho_C(t)$ and $v_C(t)$).

    4.  $\langle$<span>**Method D**</span>$\rangle$

        <span>**Input parameters:**</span>

        $\langle$<span>**measurement area$\rangle$**</span>

        $\langle$<span>**IsOutputGraph**</span>$\rangle$:

        to determine whether output data for visualizing the Voronoi
        diagram or not. If it is true, files including Voronoi cells,
        speed and the coordinates of pedestrian corresponding to each
        cell will be output.

        $\langle$<span>**cutbycircle**</span>$\rangle$:

        to determine whether cut each cell by circle. (this takes long
        time!!!!)

        $\langle$<span>**IndividualFDdata**</span>$\rangle$:

        to determine whether output the data for individual fundamental
        diagram, which is based on the Voronoi density and velocity and
        each pedestrian but not averaged value over space

        $\langle$<span>**GetProfile**</span>$\rangle$:

        to determine whether calculate the profiles over time and space.
        If yes, the resolution which is decided by the parameters
        ’$\langle$<span>**scale\_x**</span>$\rangle$’ and
        ’$\langle$<span>**scale\_y**</span>$\rangle$’ should be set.

        <span>**Output data:**</span>

        mean density and velocity over time ($\rho_V(t)$ and $v_V(t)$).

        Data for plotting Voronoi cells

        Data for plotting profiles

        Data of Individual Fundamental diagram

JPSvis
======

JPSvis is available on Linux, Windows and OS X platforms. The Graphical
User Interface (GUI) is built with Qt 4.6 available at
<http://qt-project.org/>. For rendering images and videos the
Visualisation ToolKit (VTK 5.4.2) libraries are used. VTK can be
downloaded from <http://www.vtk.org/VTK/resources/software.html>.

End User Installation
---------------------

Fiigures [fig:installStart] through [fig:installFinish] show the
installation process on windows platform using the Windows installer.
The installation of the QT and VTK libraires are optional. Their
location should be available at the runtime. This can be done by setting
the corresponding path variable(s). Linux users may download a zip file
containing the pre-compiled binaries. Note that QT 4.6 and VTK 5.4 are
required. Linux users also require the FFMPG libraries in order to make
videos.

Developers Installation
-----------------------

The compilers used are gcc version 4.3.3 for Linux and mingw gcc 4.4.1
for Windows. The directives are the same for other compiler versions.
the packages needed are:

1.  VTK: Visualization Toolkit, version 5.4 or newer.

2.  Qt Toolkit, version 4.5.3 or newer

3.  FFMPEG: Needed for video support under Linux.

JPSed
=====
