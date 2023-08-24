# CollidingCircles
Simple 2D gravity simulator / sandbox developed in C++ 17 and OpenGL 3.3.

![CircleColliderExample](https://github.com/melodiskdur/CollidingCircles/assets/85485015/ed0d9c5d-624f-4673-a881-08cccc9c601a)

https://github.com/melodiskdur/CollidingCircles/assets/85485015/374ce2dc-925a-43ce-a092-cc43a0adca99

## How To Build
TBA.

## Dependencies
TBA.

## Force Calculations
This project implements the Barnes-Hut approximation for simulating the gravitational forces on each circle. It's an efficient O(n*log(n)) algorithm that
uses an underlying spatial Quadtree structure in order to approximate forces from Quad regions onto a given particle, under the condition that
the center of mass of a Quad is "far enough" from the particle's position. See the image below for a visual example of the Barnes-Hut Quadtree.
For more information, visit [Barnes-Hut](https://beltoforion.de/en/barnes-hut-galaxy-simulator/).

![BarnesHutQuadTree](https://github.com/melodiskdur/CollidingCircles/assets/85485015/5f35d5ab-6bfe-4867-ab85-e6e63487a729)
