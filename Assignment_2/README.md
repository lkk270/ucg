Introduction to Raytracing and Shading
======================================

**Due Date**: Oct 1, 2019.

The goal of this assignment is to implement your first ray tracer and get familiar with different shading.
In the first exercise, you will implement a raytracer and use it to render spheres and planes with different projections.
In the second exercise, you will extend the ray tracer to support different shading.

### Using Eigen

In all exercises you will need to do operations with vectors and matrices. To simplify the code, you will use [Eigen](http://eigen.tuxfamily.org/).
Have a look at the [Getting Started](http://eigen.tuxfamily.org/dox/GettingStarted.html) page of Eigen as well as the [Quick Reference](http://eigen.tuxfamily.org/dox/group__QuickRefPage.html) page for a reference of the basic matrix operations supported.

### Preparing the Environment

Follow instructions the [general rules](https://github.com/nyu-cg-fall-17/computer-graphics/blob/master/RULES.md) to setup what you need for the assignment.

### Submission

Please refer to [this section](https://github.com/nyu-cg-fall-17/computer-graphics/blob/master/RULES.md#what-to-hand-in) of the general rules for more details on what to hand in.


Ex.1: Basic Ray Tracing
-----------------------

### Description

In this exercise, you will modify the provided code to launch primary rays from the camera following a perspective projection model, and implement the intersection with a parallelogram.

### Tasks

Each `TODO` in the provided code corresponds to a part that you need to fill to complete the assignment.

#### Ray Tracing a Parallelogram
1. Set up the parameters of the parallelogram (position of the corner, plus one vector for each side of the parallelogram)
2. Create your own function to check if a ray intersects with an arbitrary parallelogram.
3. Calculate the intersection between your ray and the parallelogram using the function defined above.
4. Compute the normal of that intersection point.

#### Ray Tracing with Perspective Projection
5. Modify the ray equation for perspective projection.
6. Compare the difference in the result for a sphere, for a parallelogram (you can also create a scene with multiple objects).


Ex.2: Shading
-------------

### Description

In this second exercise, you will implement the different shading equations shown in class.

### Tasks

1. Implement the basic shading components discussed in class: ambient, specular and diffuse.
2. Add RGB components instead of the current greyscale one.
3. Experiment with the different parameters and observe their effect on the raytraced shaped. You can also compare what happens on the sphere vs what happens on the parallelogram.


Starting Code
-------------

After compiling the code following the process described in the [general instructions](https://github.com/nyu-cg-fall-17/computer-graphics/RULES.md), you can launch the program from command-line as follows:

```
mkdir build; cd build; cmake ..; make
./assignment2
```
Once you complete the assignment, you should see result pictures generated in your folder.

If you run the code provided in this assignment, it should produce the following image in your current working directory:

![](img/sphere.png?raw=true)
