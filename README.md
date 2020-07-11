# VIsRenderer
This is a basic OpenGL renderer for some simple vis objects such as particles, isosurfaces and flow lines 

Thislibrary builds on top of OpenGL/C++ that allows easy creation and rendering of various types of VBOs. Currently the following geometry types are support: (1) Points 
(2) Lines (3) Triangles (4) Quads (5) Mesh. All the geometries are defined in containers.h and containers, which include a base class Container and multiple subclasses. 
To render the geometries, a class Renderer is defined in renderer.h and renderer.C. 

An example that uses Containers and Renderer can be found in cube-container.C. To complile the programs, do 'make'. 
You  will need GLEW and glm instlled in your local computer to build and run the program. 

Type cubeC and then you will find a single cube displayed. Type 'g', 'c', and 'r' to see what happen. click the left mouse button and drag the mouse to rotate the 
cube.  Click the right mouse button and drag to scale the cube. 

More explaination is still to come. 
