# microscope_scintillation
This repository contains all the software for the treatment of digital signals recorded in the MICROSCOPE Project [^footnote1] by the scintillation detector.

[^footnote1]:  Juan A. Garzón, _Atomic, Nuclear and Molecular Physics, Department of Particle Physics_, IGFAE (USC)<br/>
               John Xuna, _Cosmic Particles and Fundamental Physics researcher_, IGFAE (USC)<br/>
               José Daniel Viqueira Cao, _Cosmic Rays researcher_, IGFAE (USC)

Please **read** the instructions before dowloading and executing the software.

## How to download this repository and prepare it locally?
Press the green button to download the .zip and then unzip it in a local repo, or, copy the link and then type `git clone <link>` in a terminal.

Once cloned locally, an intermediate **step** is necessary to execute any program inside. Create in the main branch two directories: `DATA` and `OUTPUTS`.
With this step the programs are ready to be executed.

> 🚨 The software is ready to be executed in linux. Necessary to have installed `python3` and `ROOT` [^footnote2].
[^footnote2]: See documentation of how to install ROOT [here](https://root.cern/install/).

## Content and how to run each script
The name of each file is self-explanatory. Each one contains its complete description inside.
The main program is `CRoot.C`, which creates a tree from the data and allows the rest of the ROOT scripts to handle the data faster. Each program must read a file with extension `.root`, which is the tree.

The `.C` programs must be loaded inside a ROOT sesion, with the `.L` command. Then the program is executed by typing its name (which is indicated inside the code).

The `.py` programs are ready to be executed in spyder or in a terminal with `python3`.
