Notes
-------------------
Loads objects and their materials (colors).

Should be able to put any .obj & .mtl in the bin and call the .obj in the command line and it should work.

I have left 2 different object files and their corresponding material files in bin.

Building This Example
---------------------

To build this example just 

>$ cd build
>$ make
>$ make run

*name of the OBJ file is included in my makefile

Can also
>$ cd build
>$ make
>$ cd ../bin
>$ ./Matrix "name of file"

For color, material file must be in BIN.

ONLINE ASSIGNMENT DESCRIPTION
-----------------------------
PA4 - Blender and Model Loading
Objective:
Part 1: Create a model in Blender and export the OBJ file.
Part 2: Model Loading: Write a model loader that can load your OBJ from Part 1 into your program.
Part 1: Blender
Create a board with four walls in Blender. Experiment with scaling, extruding, rotating, and other basic Blender features. The board should be exported as a single object. YouTube has many useful tutorials for Blender. When exporting the board, make sure to triangulate faces because your program handles triangles instead of quads. The board should be exported as a single object i.e. it should not be a bunch of rectangles next to each other.
Extra Credit (required for Grad Students): Add colors (materials) to the board.
Part 2: Model Loading
Write a model loader that can load your OBJ from Part 1 into your program.
What's needed:
A Wavefront (.obj) file
An existing program that places the model on the screen (Week 2 or 3 should help)
Your assignment is to write a model loader. It can be a function that takes the OBJ filename as a parameter and either returns the model or passes the model back as a reference parameter. Take a program that is using a struct to represent a vertex:
	struct Vertex
	{
	   GLfloat position[3];
	   GLfloat color[3];
	};
		
where the three array indices of "position" stand for the x, y, and z components of the location, and the three indices of "color" are the red, green, and blue values of the vertex color, respectively. The colors can be hard coded by undergraduates. An array of these structs would be declared statically as:
		Vertex geometry[NUM_VERTICES];
		
This array "geometry" holds the vertices that are passed into the vertex shader. Following a format similar to this, a function definition for the model loader could be:
		bool loadOBJ(char * obj, Vertex geometry[]);
		
where "obj" is the filename of the OBJ file and the function returns false if there was a problem with the file.
Instead of hardcoding the vertices of an object as done in the previous projects, these vertices can now be taken from the OBJ file. Any line that begins with a "v" refers to a vertex, and the three values following the "v" are the x, y, and z locations of this vertex. The model loader needs to take these values and assign them to the "Vertex" objects in the "geometry" array.
Examples of how this is done can be found at THIS TUTORIAL. This tutorial also explains how to read an OBJ file and some options for exporting the OBJ from Blender. Note that you may need to export your model with triangulated faces if your program is drawing triangles. The goal of this assignment is to be able to create a model in Blender and load it into your program.



By, Luis Almanzar