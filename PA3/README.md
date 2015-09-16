PA3 - Moons (Description from online)
Objective: Now that we have a window open and an animation going, with keyboard, mouse, and menus, we will be dealing with multiple objects. You'll be taking your code from last week and a moon to the object you have already.
For this project, students will be expected to render two cubes onto the screen. One of the cubes will move around in a circle while the other cube orbits around it. The program is expected to be designed in such a way that the user, through keyboard inputs, can change the direction of rotation of the first cube, while the second continuously orbits it.
In addition to the planet orbiting and the moon orbiting the planet properly (no matter which direction it is going) you need to implement the planet direction changes with the keyboard arrows (in addition to the keyboard/mouse done last time).
A Recommended tutorial on matricies (from previous students) can be found here
Extra Credit (Required for Grad Students): Add text to indicate which direction the planet is rotating. This should change when the direction changes

Functionality Added
-------------------
Basically, this project does exactly what was asked by adding another cude that is orbitting around the original cube.  You can change orbit directions of the main cube with the arrow keys and the smaller cube will keep its same orbit.

As a test to myself I tried changing all the aspects of the cubes individually. I did run into problems here. Whenever I changed the rotation (not orbit) of the main cube, the smaller cube would still move with it but its rotation stopped.  I added all the individual functionality I wanted to the menus even though some of them do not work.

Building This Example
---------------------

To build this example just 

>$ cd build
>$ make


By, Luis Almanzar