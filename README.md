# hybridTrace

Real-time GPU based (OpenGL) application for comparing traditional triangle rasterization and ray tracing.
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


## Keyboard Controls
  * `ESC` Exit program

## Mouse Controls
  * `mouseDrag` Rotates camera
  * `mouseScroll` Zoom camera in/out

## Render Modes
  *  `Rasterize Mode` Renders reflective spheres as triangle meshes. Colors of meshes are sampled from the environment map.
  *  `Ray Trace Mode`  Renders reflective spheres using exact implicit formulation. Ray traced primary and secondary visibility.
  *  `Hybrid Mode`  Reflective spheres are triangle meshes. Primary visibility is handled via rasterization, lighting is handled via ray tracing.


# Screenshots
## Live Demo
![Live Demo](/screenshots/demo.gif "Live Demo Example")
## Rasterization Mode
![Rasterization Mode](/screenshots/raster_mode.PNG "Rasterization Mode Example")
## Ray Trace Mode
![Ray Trace Mode](/screenshots/trace_mode.PNG "Ray Trace Mode Example")
## Hybrid Mode
![Hybrid Mode](screenshots/hybrid_mode.PNG "Hybrid Mode Example")

## Potential Future Additions
  * Loading in custom skyboxes
                       
## Copyright and Attribution
Copyright (c) 2023 eqdiag. Released under the [MIT License](https://github.com/eqdiag/hybridTrace/blob/main/LICENSE.md).
