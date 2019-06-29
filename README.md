# Towards a universal objective for datacenter transports supporting delay-sensitive applications

## Prerequisites

 * CMake
 * OTcl
 * TclCL
 * Tcl/Tk
 * Boost

Build `delay_transport` and `ns2` using CMake and set enviroment variable `NS_PATH` pointing to ns2.

To run simulations go to the `PIAS-NS2/scripts`, and follow the instructions from `PIAS-NS2/README.md`.
To calculate THD run `analyze_deadlines.py` with two arguments: *pFabric*'s `flow.tr`, and `flow.tr` of the algorithm of interest.





