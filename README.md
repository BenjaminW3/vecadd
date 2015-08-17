vecadd - Generalized Vector Addition (AXPY) wrapper library
===============================================================

This repository contains multiple sequential and parallel AXPY implementations.
Single and double precision values are supported (compile-time decision).


Software License
----------------

**vecadd** is licensed under **LGPLv3** or later.


Available Implementations
-------------------------

* Sequential:
  * Basic

* Parallel:
  * OpenMP 2.0 guided schedule
  * CUDA Native Implementation
  * [alpaka](https://github.com/ComputationalRadiationPhysics/alpaka)
    * Sequential
    * CUDA
    * OpenMP 2.0 (Host only)