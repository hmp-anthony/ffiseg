### ffiseg

A physics engine based on the Cyclone physics engine.

I have improved the code and added tests to increase robustness.

No more make files. CMake is used as the build system.

Just type `make` and then `./build/demos/ballistic/ballistic`, for instance.

I hope to add to the code base by adding:

 - [ ] Fabric simulation
 - [ ] Liquid simulation
 - [ ] Replace C-style pointers with smart pointers
 - [ ] Fix inconsistent use of getters and setters. 
 - [ ] Replace raw arrays with `std::pair`, `std::array` and/or `std::vector`
 - [ ] FEM solvers
 - [ ] Avx/MPI/OpenMP optimisation 
 ...


