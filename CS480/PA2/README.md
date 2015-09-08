PA2 - Interaction: Menus, Keyboard, and Mouse

Objective: Now that we have a window open and an animation going, we will be adding interaction. You'll be taking your code from last week and adding menus, keyboard interaction, and mouse interaction.

Menus: To learn more about adding menus, look in your book at Section 2.12 Menus. You'll be adding an option to start the spinning, to pause the spinning, and to exit the program.
A hint on starting/pausing the rotation: add a flag that is checked when the cube is rotated.

Keyboard: To learn more about adding keyboard events, look in the book at Section 2.11.3 Keyboard Events. You will need to change the spinning of your cube based upon a key being pressed. For example if you press A, the rotation reverses direction.

Mouse: Look in Section 2.11.1 Using the Pointing Device. You will be replicating what we did with the keyboard by using the mouse. If you click on the screen, the cube will reverse rotation

Functionality Added
-------------------
Right click for menu. Sub menu for starting and stopping all rotation of the cube when pressed.
Also, quit the program from the menu.

'R', 'r'  will both reverse the rotation of the cube (not the direction which it is traveling).

Space bar and left click will both pause the animation.

ESC will also exit the program. 

Building This Example
---------------------

*This example requires GLM*
*On ubuntu it can be installed with this command*

>$ sudo apt-get install libglm-dev

*On a Mac you can install GLM with this command(using homebrew)*
>$ brew install glm

To build this example just 

>$ cd build
>$ make

*If you are using a Mac you will need to edit the makefile in the build directory*

The excutable will be put in bin

Additional Notes For OSX Users
------------------------------

Ensure that the latest version of the Developer Tools is installed.
