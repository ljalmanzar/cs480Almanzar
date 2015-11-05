Assimp Texture Loader
==================
This program will use the Assimp model loader to load files that will be found in another directory then with the help of the image loader Magick++, it will map an image onto the model.

How to run
==========
*Must install Magick++ first
Just testing..

Navigate to build
--> make 
--> make run 

The OBJ & text are both command line arguments and can be changed in our make file under
# Linux 
MODEL= "Your file path here"
TEXTURE= "Your file path here"

Known Issues
============
Our texture seems to be mapping on the object backwards since our words are backwards. 

This is a group project where, although we all have the source code, we will be using one person's GitHub Repository for submission
This group consists of
 * Luis Almanzar *(Repo Holder)*
 * Gunnar Wambaugh
 * Daniel Lopez
