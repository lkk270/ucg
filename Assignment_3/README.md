Basic Raytracing Effects
========================

**Due Date**: TBD.

The goal of this assignment is to implement basic ray tracing effects such as shadows, reflection, refraction and depth of field.

### Using Eigen

In all exercises you will need to do operations with vectors and matrices. To simplify the code, you will use [Eigen](http://eigen.tuxfamily.org/).
Have a look at the [Getting Started](http://eigen.tuxfamily.org/dox/GettingStarted.html) page of Eigen as well as the [Quick Reference](http://eigen.tuxfamily.org/dox/group__QuickRefPage.html}) page for a reference of the basic matrix operations supported.

### Preparing the Environment

Follow instructions the [general rules](https://github.com/nyu-cg-fall-17/computer-graphics/blob/master/RULES.md) to setup what you need for the assignment.

### Submission

Please refer to [this section](https://github.com/nyu-cg-fall-17/computer-graphics/blob/master/RULES.md#what-to-hand-in) of the general rules for more details on what to hand in.


Ex.0: Field of View and Perspective Camera
------------------------------------------

![](img/fov.png?raw=true)

The field of view of a perspective camera represents the angle formed between the center of the camera and the sensor (aka the pixel grid through which rays are shot). The focal length is the distance between the camera center and the sensor, and is called `f` in the figure above.

1. Fill the starter code to compute the correct value of `h` (`scale_y` in the code).
2. Implement the perspective camera similarly to Assignment 1.


Ex.1: Shadow Rays
-----------------

To determine if a point is in the shadow of another or not, one must cast a ray from this point to the different light sources in the scene. If another object lies in between the point and the light source, then this light does not contribute to the color of the point.

### Tasks

1. Fill the starter code to implement shadow rays.
2. Show what happens when the shadow ray is not offset by a small epsilon value.


Ex.2: Reflection & Refraction
-----------------------------

![](img/refraction.png?raw=true)
(Figure: [Wojciech Jarosz](https://canvas.dartmouth.edu/courses/16840))

To render mirrors and transparent materials, shading must also consider objects that could be reflected or seen through the surface hit by the camera (primary) rays. This can be achieved by shooting new rays from the hit position to the scene with a new direction.

The direction of the reflected has been given in class, and can be expressed as `r = 2n(n·v)-v`. The direction of the refracted ray can be derived from the Snell-Descartes law indicated on the figure.

### Tasks

1. Derive the formula for the direction of the refracted ray based on the Snell-Descartes law.
2. Fill the starter code to implement reflected and refracted rays. Don't forget to decrease the counter to limit the maximum number of 'bounce' a ray can make, and be sure to check for total internal reflection when implementing refraction (it may happens that there is no refracted ray if the incident direction is too low).


Ex.3: Depth of Field
--------------------

![](img/dof.png?raw=true)

A basic depth of field effect can be achieved in our simplified camera model. Instead of only one ray per pixel, one can shoot several rays per pixel as illustrated by the figure above. In practice, this means the hole of our camera no longer has an infinitesimal size, a parameter known as the aperture in real-world cameras.

Here, the focal plane of our camera corresponds to our pixel grid (which we have put at a distance `f` from the camera center). This means that object A, which is close to the focal plane, will appear sharper than object B, which is more distant.

To implement a depth of field effect, you will need average the contribution of several rays per pixels, sampled randomly as suggested in the picture.

### Tasks

1. Fill the starter code to implement depth of field.
2. Modify the input scene to move the rightmost sphere more into focus. Experiment with different settings of this effect. 


Starting Code
-------------

After compiling the code following the process described in the [general instructions](https://github.com/nyu-cg-fall-17/computer-graphics/RULES.md), you can launch the program from command-line as follows:

```
mkdir build; cd build; cmake ..; make
./assignment2 ../data/scene.json
```
Once you complete the assignment, you should see result a picture generated in your folder.

After implementing perspective camera, specular highlights, shadows, reflections, refractions and depth of field, the result of your code should like this:

![](img/result.png?raw=true)
