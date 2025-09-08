### ffiseg

A physics engine based on the Cyclone physics engine. Image taken from my old github: https://github.com/llyr-who/fabrik

My current laptop is not powerful enough to recreate that image.

I have the bear to prove it :)

![Screenshot](/image.png)

I have improved the code and added tests to increase robustness.
I have implemented a cloth simulator using this engine.
See the demos for more on this.

No more make files. CMake is used as the build system.

Just type `make` and then `./build/demos/ballistic/ballistic`, for instance.

I hope to add to the code base by adding:

 - [ ] Liquid simulation
 - [ ] Replace C-style pointers with smart pointers
 - [ ] Fix inconsistent use of getters and setters. 
 - [ ] Replace raw arrays with `std::pair`, `std::array` and/or `std::vector`
 - [ ] FEM solvers
 - [ ] Avx/MPI/OpenMP optimisation 
 ...


