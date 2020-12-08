# Character animation project

## Overview

This project offers a usage example of the [physim](hhttps://github.com/lluisalemanypuig/physics-simulator/) library. In particular, it provides several example simulations showing how to use the steering algorithms for agents, for the ultimate purpose of simulating a crowd.

These simulations can be found in the subproject
[anim](https://github.com/lluisalemanypuig/character-animation/tree/master/charanim/anim).
These simulations are:
- Simulation 100: example of _seek_ steering.
See [this](https://youtu.be/mrXKAWbpMrg) video.
- Simulation 101: example of _flee_ steering.
See [this](https://youtu.be/3nEgPSVR6Jc) video.
- Simulation 102: example of _arrival_ steering.
See [this](https://youtu.be/q4GoyllWeDo) video.
- Simulation 103: example of _collision avoidance_ steering.
See [this](https://youtu.be/oLG41e-VA8I) video.
- Simulation 104: example (1) of _unaligned collision avoidance_ steering.
See [this](https://youtu.be/Oy_2ld4sNNc) video.
- Simulation 105: example (2) of _unaligned collision avoidance_ steering.
See [this](https://youtu.be/0QkEuy_MnOQ) video.
- Simulation 106: example (1) of _walk with me_ steering.
 See [this](https://youtu.be/2MXQZmuyw5Q) video.
- Simulation 107: example (2) of _walk with me_ steering.
 See [this](https://youtu.be/YGUqM94e4lk) video.
- Simulation 200: A single agent follows a path from a dynamically loaded
map. 'Dyanmically loaded' means that the user can provide their own map
to the simulation. The start and goal points of the path are also given
by the user. See [this](https://youtu.be/fRwBXJwTm2A) video.

The details on how each steering behaviour is implemented can be found in the [documentation](https://github.com/lluisalemanypuig/physics-simulator/tree/master/docs). of the _physim_ library. The full details can be found in the [code](https://github.com/lluisalemanypuig/physics-simulator/blob/master/physim/particles/agent_particle.cpp).

## 3D models

### Characters used

The models used, found in the directory characters/, were copied 'as is' from the _cally_ demo, which can be downloaded in the main page of the [Cal3d](https://mp3butcher.github.io/Cal3D/) library. They are not part of the _charanim_ project. No license was added to the directory as none was found in the file downloaded via that link.

### Other models

For the sake of debugging and of embellishing a scene (as much as a computer scientist can), some simple 3d models were designed for such purpose. These are found in the _models/_ directory and _are_ part of the _charanim_ project, so the license in file [LICENSE]() also applies to them (or at least, it should).

## Dependencies

This project depends on several libraries:
- [Cal3d](https://mp3butcher.github.io/)
- [physim](https://github.com/lluisalemanypuig/physics-simulator)
- OpenGL, GLUT, GLEW.
- [glm](https://glm.g-truc.net/0.9.9/index.html) (v. 0.9.5 or older)

See [this](hhttps://github.com/lluisalemanypuig/physics-simulator/blob/master/README.md) for instructions on how to compile the _physim_ library. If the _physics-simulator_ project is not put in the same directory, and at the same level, as this then the _.pro_ file needs to be modified appropriately.

## Compilation

It is strongly suggested that this project be compiled in release mode. It contains one _.pro_ [file](https://github.com/lluisalemanypuig/character-animation/tree/master/charanim) grouping all these subprojects, which can be opened using the IDE tool Qt Creator. These _.pro_ files need to contain, if necessary:
- The path to include the _physim_ library, and to link against it.
- The paths to include the _glm_ lbrary. The .pro already include the include paths to these libraries assuming a Linux environment in which these are installed in the system.
- The path to include the GL, GLUT, GLEW libraries.

All the _.pro_ already have the paths to include and link againts these libraries assuming  they are installed in the system.

In order to successfully compile the project, one can follow these intructions:

- Create the makefile

	qmake -makefile charanim/charanim.pro -o charanim-release/Makefile

- Compile:
	
	cd charanim-release/ && make && make

The fact that the command _make_ has to be issued twice is that the _charanim_ project has subprojects depending on other subprojects, in which the latter may not be compiled before the former.

### Execution

The simulations listed above can be executed, after compiling, by navigating to the appropriate directory

	cd charanim-release/

and executing

	./anim id

where _id_ is one of {100,101,102,...,107,200}, all the simulations available. Each simulation admits different parameters. They can be checked with:

	./anim id --help

## Context

This repository contains the second [Computer Animation](https://www.fib.upc.edu/en/studies/masters/master-innovation-and-research-informatics/curriculum/syllabus/CA-MIRI) course project, carried out during the first semester of the academic year 2018-2019. The course is part of the [Master in Innovation and Research in Informatics (MIRI)](https://www.fib.upc.edu/en/studies/masters/master-innovation-and-research-informatics) curriculum.
