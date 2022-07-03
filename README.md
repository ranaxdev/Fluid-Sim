
<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/ranaxdev/Fluid-Sim">
    <img src="https://github.com/ranaxdev/Fluid-Sim/blob/main/Res/logo.png" alt="Logo" width="80" height="80">
  </a>

  <h3 align="center">Fluid-Sim</h3>

  <p align="center">
    An interactive particle-based fluid simulator made with C/C++ and OpenGL
    <br />
    <br />
    <a href="https://github.com/ranaxdev/Fluid-Sim/issues">Report Bug</a>
  </p>
</p>

<hr>



<!-- ABOUT THE PROJECT -->
## Preview


https://user-images.githubusercontent.com/44033302/177021274-d94fa7a6-b5d6-4ce3-b12e-cb8428dea59d.mp4





<br>

This is an interactive 3D particle-based fluid simulator based on smoothed particle hydrodynamics, featuring fluid interaction with a tank container and adjustable parameters such as mass, density, gravity, viscosity and different smoothing kernels. Based on the <a href="https://matthias-research.github.io/pages/publications/sca03.pdf">paper</a> by Matthias Müller et al. at the department of Computer Science, Federal Institute of Technology Zürich (ETHZ), Switzerland, published in 2003 at the Eurographics/SIGGRAPH Symposium on Computer Animation.

## Details
### Computational Fluid Dynamics (CFD)
The solution is a work-around to the computational fluid dynamics problem of performance as producing approximations to the Navier-Stokes equations is computationally expensive and the visualization aspect is generally done as a second step, but this implementation provides live computation of the fluid behavior by approximating the shallow water equations in discretised grid-space (Euler).

The basic principal is to distribute field quantities in a local neighborhood of each particle using radial symmetrical smoothing kernels, which respect the Navier-Stokes principals, conservation of mass and momentum. The below is a simplified version of the equations for incompressible fluids.

<img src="https://github.com/ranaxdev/Fluid-Sim/blob/main/Res/mass.png" alt="Force" style="width:15%;"/>

<img src="https://github.com/ranaxdev/Fluid-Sim/blob/main/Res/momentum.png" alt="Force" style="width:30%;"/>

Where `ρ` is density, `v` is a velocity field, `g` is an external force density field and `μ` is viscosity.

Because the derivative of the velocity field is essentially the derivative of the particle velocity, we don't need to approximate the conservation of mass anymore and can focus on the second equation, which has the components pressure, external force and viscosity. According to SPH, a scalar quantity `A` is interpolated at location `r` by a weighted sum of contributions from all particles as shown below

<img src="https://github.com/ranaxdev/Fluid-Sim/blob/main/Res/sph.png" alt="Force" style="width:20%;"/>

Here, `j` iterates over all particles, `m_j` is the mass of particle `j`, `r_j` its position, `ρ_j` the density and `A_j` the field quantity at `r_j`. The function `W` is a smoothing kernel with radius `h`. Using this, we can approximate the different required forces such as internal pressure

<img src="https://github.com/ranaxdev/Fluid-Sim/blob/main/Res/pressure.png" alt="Force" style="width:30%;"/>

### Pressure

Where `p`, the pressure force is computed using `p=k(ρ-ρ_0)` where `k` is the gas constant and `ρ_0` is the rest density. However, this force is not symmetric and the fluid will behave irregularly.

<img src="https://github.com/ranaxdev/Fluid-Sim/blob/main/Res/sym.png" alt="Force" style="width:30%;"/>

Therefore, the paper suggests a way to symmetrize the force which uses the arithmetic mean of the pressures of the interacting particles.

<img src="https://github.com/ranaxdev/Fluid-Sim/blob/main/Res/sym2.png" alt="Force" style="width:30%;"/>

### Viscosity

Using the same principal to calculate the viscosity force yields more asymmetrical forces which have a suggested correction using velocity differences.

<img src="https://github.com/ranaxdev/Fluid-Sim/blob/main/Res/vis1.png" alt="Force" style="width:30%;"/>

<img src="https://github.com/ranaxdev/Fluid-Sim/blob/main/Res/vis2.png" alt="Force" style="width:30%;"/>

### Smoothing Kernels

This implementation experimented with the `Poly6`, `Spiky` and `Viscosity` kernels which have different effects on the behavior of the fluid in terms of stability, accuracy and speed.

The `W_poly6` and `W_spiky` can be interchanged for internal pressure and are shown below

<img src="https://github.com/ranaxdev/Fluid-Sim/blob/main/Res/poly6.png" alt="Force" style="width:30%;"/>

<img src="https://github.com/ranaxdev/Fluid-Sim/blob/main/Res/poly6_d.png" alt="Force" style="width:30%;"/>

<img src="https://github.com/ranaxdev/Fluid-Sim/blob/main/Res/spiky.png" alt="Force" style="width:30%;"/>

<img src="https://github.com/ranaxdev/Fluid-Sim/blob/main/Res/spiky_d.png" alt="Force" style="width:30%;"/>

And for the viscosity force kernel `W_viscosity`, we need the second laplacian which requires a kernel that doesn't resolve negative forces that increase relative velocities.

<img src="https://github.com/ranaxdev/Fluid-Sim/blob/main/Res/visc_kernel.png" alt="Force" style="width:40%;"/>

<img src="https://github.com/ranaxdev/Fluid-Sim/blob/main/Res/visc_d.png" alt="Force" style="width:30%;"/>

### Integration Scheme

The velocity and position are updated using the leapfrog integration scheme which is describes by the following equations

<img src="https://github.com/ranaxdev/Fluid-Sim/blob/main/Res/leapfrog.png" alt="Force" style="width:25%;"/>



## Installation
### Linux

Dependencies include `OpenGL 4.50`, `libdl`, `glfw3`, `pthread`, `glm`

```bash
git clone https://github.com/ranaxdev/Fluid-Sim/
cd Fluid-Sim
mkdir Release
cd Release
cmake -DCMAKE_BUILD_TYPE=Release ../
make
```

### Windows

I'm currently working on releasing an executable so that you don't have to use `cmake` to build it on windows

## Libraries Used

- [GLAD](https://github.com/Dav1dde/glad)
: OpenGL Loading Library

- [GLM](https://github.com/g-truc/glm)
: OpenGL Mathematics

- [GLFW](https://www.glfw.org/)
: A multi-platform library for OpenGL, OpenGL ES, Vulkan, window and input 

- [Dear ImGui](https://github.com/ocornut/imgui)
: Bloat-free Graphical User interface for C++ with minimal dependencies 

<!-- LICENSE -->
## License

Distributed under the Apache 2.0 License. See `LICENSE` for more information.


<!-- CONTACT -->
## Contact

S.R. Qaiser - [@pitu_dev](https://twitter.com/pitu_dev) - sc21srq@leeds.ac.uk

Project Link: [https://github.com/ranaxdev/Fluid-Sim](https://github.com/ranaxdev/Cloth-Sim)
