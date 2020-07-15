# VIsRenderer
This is a basic OpenGL renderer designed to draw some simple vis objects such as particles, isosurfaces and flow lines (to be done). 

This library builds on top of OpenGL/C++ that allows easy creation and rendering of various types of VBOs. Currently the following geometry types are supported: (1) Points 
(2) Lines (3) Triangles (4) Quads (5) Mesh. All the geometries are defined in containers.h and containers, which include a base class Container and multiple subclasses. 
To render the geometries, a class Renderer is defined in renderer.h and renderer.C. 

An example that uses Containers and Renderer can be found in cube-container.C. To complile the programs, do 'make'. 
You  will need GLEW and glm installed on your local computer to build and run the program. 

Type cubeC and then you will find a single cube displayed. Type 'w','g', 'c', and 'r' to see what happen. click the left mouse button and drag the mouse to rotate the 
cube.  Click the right mouse button and drag to scale the cube. 

More explaination is still to come. 
