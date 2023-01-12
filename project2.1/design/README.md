# HeatPlate Simulation Analysis

Jobs are read from the folder inside the project specified as a .txt. The work done and the plates needed are read in the form of data plates by the binary matrix reader and the configuration specified by the job. The plates must always be .bin files.

A job is read with the following data, no parameters are optional:

    plate001.bin 1200 127 1000 2
    plate001.bin 1200 127 1000 1.5
    plate002.bin 60 0.08 450 0.75

There's two versions one in which plates are not distributed and calculated serially or with OpenMP, and a distributed version with OpenMPI and OpenMP.

Plates are distributed by processes dinamically by process 0 and worked as fast as possible with the threads specified inside each process. Simulated generations are handled by these threads to work as evenly as possible, statically, with OpenMP threading.

All matrixes are read from the process 0 and sent back while receiving the data required to make the report per each plate done, after all plates are calculated. Pseudocode of the this design can be found in the pseudocode folder.
