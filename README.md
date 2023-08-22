# CollidingCircles
Simple 2D gravity simulator / sandbox using C++ and OpenGL.

![CircleColliderExample](https://github.com/melodiskdur/CollidingCircles/assets/85485015/cc703fda-15a6-4347-9992-96e1b8acaf5f)

Implements the Barnes-Hut approximation for simulating the gravitational forces on each circle. It's an efficient O(n*log(n)) algorithm that
uses an underlying spatial Quadtree structure in order to approximate forces from Quad regions onto a given particle, under the condition that
the center of mass of a Quad is "far enough" from the particle's position. See the image below for a visual example of the Barnes-Hut Quadtree.
For more information, visit [The Barnes-Hut Galaxy Simulator]([https://pages.github.com/](https://beltoforion.de/en/barnes-hut-galaxy-simulator/)https://beltoforion.de/en/barnes-hut-galaxy-simulator/)
