# hybridTrace

Application for comparing classical triangle rasterization and ray tracing.
This does NOT use a physically-based lighting model.
Some of the main visual effects ray tracing provides that rasterization doesn't (out of the box) in this case are: shadows and multi-bounce reflections.

Uses my custom math library: [gMath](https://github.com/eqdiag/gMath)

All dependencies are self-contained in this project using git's submodule system.



## Installation

```
git clone https://github.com/eqdiag/hybridTrace
cd hybridTrace
mkdir build
cd build
cmake ..
make
```

## Modes
  *  `Rasterize Mode` Renders reflective spheres as triangle meshes. Colors of meshes are sampled from the environment map.
  *  `Ray Trace Mode`  Renders reflective spheres using exact implicit formulation. Ray traced primary and secondary visibility.
  *  `Hybrid Mode`  Reflective spheres are triangle meshes. Primary visibility is handled via rasterization, lighting is handled via ray tracing.


## Screenshots

![alt text](https://github.com/eqdiag/hybridTrace/blob/main/screenshots/raster_mode.PNG "Rasterization Mode Example")
![alt text](https://github.com/eqdiag/hybridTrace/blob/main/screenshots/trace_mode.PNG "Ray Trace Mode Example")
![alt text](screenshots/hybrid_mode.png "Hybrid Mode Example")

## Keyboard Controls
  * `ESC` Exit program

## Mouse Controls
  * `mouseDrag` Rotates camera
  * `mouseScroll` Zoom camera in/out

## Potential Future Additions
  * Loading in custom skyboxes
                       
## Copyright and Attribution
Copyright (c) 2023 eqdiag. Released under the [MIT License](https://github.com/eqdiag/hybridTrace/blob/main/LICENSE.md).
