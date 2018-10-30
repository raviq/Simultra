<p align="center">
  <a href="https://github.com/raviq/Simultra">
    <img alt="simultra" src="https://github.com/raviq/Simultra/blob/master/docs/ico.png" width="200">
  </a>
</p>

<h1 style="text-align:center; display: inline;" align="center">
  Simultra
</h1>

<p align="center">
	A multiagent simulator of road traffic
</p>

<p align="center">
	<a href="https://www.gnu.org/licenses/gpl-3.0" alt="License: GPL v3"><img src="https://img.shields.io/badge/License-GPL%20v3-blue.svg" /></a>
	<!--<a href="https://travis-ci.org/raviq/Simultra" alt="Build Status"><img src="https://travis-ci.org/raviq/Simultra.svg?branch=master" /></a> -->
</p>


## Table of Contents
* [Description](#desc)
* [Prerequisites](#prereq)
* [Building](#build)
* [Usage](#usge)
* [Integration](#integ)
* [Improvements](#improv)
* [Contributions](#contrib)
* [Citation](#cite)
* [License](#licens)

<a  name="desc"></a>
## Description

Simultra is a multiagent simulator of road traffic.

### Features

- Programmable agents that can emulate vehicles, drivers and intelligent traffic signals.
- Realistic map rendering using [OpenStreetMap](https://www.openstreetmap.org).
- Precipitation rendering on top of the traffic simulation layer.

Watch this simulation [video](https://github.com/raviq/Simultra/blob/master/Simultra/resources/video.mp4) from AAMAS 2016. 

### Architecture

<p align="center">
	<img src="https://github.com/raviq/Simultra/blob/master/docs/arch.png" width="600">
</p>

<a  name="prereq"></a>
## Prerequisites

- Qt 5.11.2
- C++11
- [Readosm](https://www.gaia-gis.it/gaia-sins/readosm-sources) (1.0.0e)
- [MapGraphics](https://github.com/raptorswing/MapGraphics) 1.0
	
<a  name="build"></a>
## Building

Project structure

```
Simultra/
│   README.md                    // This file
│   LICENSE.md                   // GNU GENERAL PUBLIC LICENCE (v3)
│   simultra.pro                 // Required by qmake to build the project
│   Makefile                     // Automatically generated makefile
└───kore/                        // Core of the simulator
|       *.h *.c *.cpp
└───gui/                         // UI of roads, vehicles, traffic, etc.
|   |   *.h *.cpp *.ui
│   └───images/                  // Image manipulation routines
|       *.h *.cpp
└───build/
|   |   simultra.app             // The simulator
│   └───obj/
|       *.o moc_* ui_* qrc_*
└───resources/
│   └───config/                  // Configurations and simulation scenarios
│   └───RadarImages/             // Precipitation images
└───utils/
        *.h *.c *.cpp            // Debug, log, geometrical calculations, etc.   
│      
└───MapGraphics/                 // Required to build libMapGraphics
```

To build MapGraphics and generate `libMapGraphics` run

```sh
qmake MapGraphics.pro
make
```

To build simultra run

```sh
qmake simultra.pro
make
```
<a  name="usge"></a>
## Usage

To configure and run a simulation you need to work on a number of files in `config/`.

- Start by downloading the map of the area you are interested in from [osm.org](https://www.openstreetmap.org/export#map=18/35.17126/136.90116&layers=C) and name it `map.osm` .
- Set the map topology in `simx_map.xml`.
- Set the simulation scenario in `simx_simulation.xml` by specifying the vehicles and itineraries:

For each vehicle, we need to specify an XML entry:


```
<Vehicle type="TypeX">
  <Location>450 440</Location>
  <Direction>0</Direction>
  <Driver type="Slow1">
    <Lane>
      <Intersections>A B C D E</Intersections>
    </Lane>
  </Driver>
</Vehicle>
```

which contains the initial position of the vehicle, its orientation, driver type, and the intersections that define the itinerary of the driver.


- Set the types of the vehicles and the divers in `types.xml`.
An example of vehicle type specifies the mass, wheelbase, force, and the look of the vehicle:
```
<VehicleType name="TypeX">
  <Image>:/images/bluecar.png</Image>
  <Mass>1800.0</Mass>
  <Wheelbase>1</Wheelbase>
  <MotForce>5000</MotForce>
</VehicleType>
```

The driver type specifies the velocities of two angles (straight and turn) which will later be used in the behavioural model. Example:
```
<DriverType name="SlowType">
  <TurnVelocity>2</TurnVelocity>
  <StraightVelocity>12</StraightVelocity>
</DriverType>
```

- Set the radar images in `RadarImages/` for the precipitation simulation. Current build comes with examples of `.gif` images from the [Japan Meteorological Agency](https://www.jma.go.jp/jma/indexe.html). 

- It is important to set some of the parameters in `config.h`.

Now, running the simulator should give something like:

<p align="center">
	<img src="https://github.com/raviq/Simultra/blob/master/docs/GUI1.png" width="900">
</p>

with

1. Play/Pause simulation.
2. Stop simulation.
3. Simulation clock.
4. Shows the OSM view of the simulation.
5. Shows the physical view of the simulation.
6. Zoom (OSM Map).
7. Zoom (Physics engine Map).
8. Increase/Decrease vehicle size.
9. Center the map in one particular point (specified in `config.h`).
10. Pedestrian mode.
11. Moving the map.
12. Adding precipitations.
13. About Simultra.
14. Simulation area.
15. Order of the layers and rendering opacity.
16. Simulation components.
17. Vehicle's forces. Works in physical view.
18. Flow parameters.
19. Traces, mobility data, etc.
20. Main view window.

### What you can do
- Zoom in/out, either with the mouse or button 6.
- Drag the map in different directions by clicking on button 11 and using the mouse.
- Start the simulation using button 1.
- Go through the mobility data in area 19.
- Set zoom scale to 6 and press button 12 to see how precipitation clusters change on top of the map.
- Press button 13 to shown some information about Simultra.
- Alternate layers in area 15 using the up and down buttons.

etc.

<a  name="integ"></a>
## Integration in other projects

Simultra was used in other project either directly or after some changes on its design and code.

- As a pipelined distributed architecture ([project](https://github.com/itolab-hayashi-rafik/Simultra))

<p align="center">
	<img src="https://github.com/raviq/Simultra/blob/master/docs/integ.png" width="700">
</p>

- On [StarBED](http://starbed.nict.go.jp/en/index.html)

<p align="center">
	<img src="https://github.com/raviq/Simultra/blob/master/docs/onstarbed.png" width="500">
</p>

<a  name="improv"></a>
## Improvements

- There are currently too many configuration files. They can be either generated automatically in a pre-simulation phase or integrated in one JSON file (`config.h` too). Another way to avoid the heavy configuration is to randomly assign vehicles to the existing (OSM) paths. The assignments could be parametrised using some vehicle-per-lane distribution. Another option is to drag and drop vehicles on the OSM map.
- Integrate traffic lights with OSM maps.
- The precipitation data could in principle be acquired dynamically using some weather API.
- Improvements on the (lat, lon) from/to (x, y) conversions.
- Combining `readosm` and `json.c`.
- More testing..

<a  name="contrib"></a>
## Contribution

Comments, feedback and ideas are welcome.
If you find this project to be of any use, please let me know.
If you find bugs or omissions in the code or documentation, please do not hesitate to submit an issue or open a pull request with a fix. Thank you.

<a  name="cite"></a>
## Citation

> Takayuki Ito, Takanobu Otsuka, Teruyoshi Imaeda, and Rafik Hadfi. "An Implementation of Large-Scale Holonic Multi-agent Society Simulator and Agent Behavior Model." Pacific Rim International Conference on Artificial Intelligence. Springer, Cham, 2018.

> Rafik Hadfi, Masayuki Hayashi, and Takayuki Ito. "A pipeline multiagent architecture for road traffic simulation." Proceedings of ITS World Congress. 2017.

> Rafik Hadfi and Takayuki Ito. "Multilayered Multiagent System for Traffic Simulation." Proceedings of the 2016 International Conference on Autonomous Agents & Multiagent Systems. International Foundation for Autonomous Agents and Multiagent Systems, 2016.

> Rafik Hadfi and Takayuki Ito. "Holonic multiagent simulation of complex adaptive systems." International Conference on Practical Applications of Agents and Multi-Agent Systems. Springer, Cham, 2016.

<a  name="licens"></a>
## License

This software was developed in the hope that it would be of some use to the AI community, 
and is freely available for redistribution and/or modification under the terms of the GNU General Public Licence.
It is distributed WITHOUT WARRANTY; without even the implied warranty of merchantability or fitness for a particular purpose.
See the [GNU General Public License](https://github.com/raviq/Simultra/blob/master/LICENSE) for more details. 


(c) 2015-2018 Rafik Hadfi, rafik.hadfi@gmail.com



