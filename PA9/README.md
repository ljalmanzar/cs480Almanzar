Air Hockey
===========
This program is the 9th Assignment from UNR course CS 480 which will be an Air Hockey Table.  This table will run off of user input from the keyboard and mouse.  The program's dependencies are with the Assimp Model Loader library, the Magick++ Image Loader, and the popular physics library, Bullet.

Quick Start
-----------
+ First make sure that you have the following libraries installed, if you haven't already
  + __Assimp__ - `sudo apt-get install libassimp-dev`
  + __Bullet__ - `sudo apt-get install libbullet-dev`
  + __Magick++__ - `sudo apt-get install libmagick++-dev`
+ To __compile__ this program, go to `build/` and type `make`
+ To __run__ the program, go to `bin/` and type `./AirHockey`.  However, to run from the build directory, type `make run`.
+ To __clean__ the program to be recompiled, go to `build/` and type  `make clean`.

Camera Controls
---------------

| Key to Press | Camera Effect |
| ------------ | ------------- |
| + | __Zoom__ In |
| - | __Zoom__ Out |
| 2 | __Pivot__ Down |
| 4 | __Pivot__ Left |
| 6 | __Pivot__ Right |
| 8 | __Pivot__ Up |


Game Tutorial
-------------
+ Once the game has been loaded, it will default to One Player and both scores are then set to 0.
+ Use your mouse to move your paddle.  The object of the game is to get the puck to the goal of the other player.
+ To move the camera around in your scene, it'd preferable to use the numpad rather than the number line at the top of the keyboard for a more natural fealing of pivoting, but this has no effect on the game.

Known Issues
------------
There are absolutely no known issues.  If you find one, you can keep it to yourself.

Credits
-------
This is a group project where, although we all have the source code, we will be using one person's GitHub Repository for submission
This group consists of
 * Luis Almanzar *(Repo Holder)*
 * Gunnar Wambaugh
 * Daniel Lopez
