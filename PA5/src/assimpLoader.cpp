#ifndef __ASSIMP_LOADER_
#define __ASSIMP_LOADER_

#include "assimpLoader.h"

// - CONSTRUCTOR
assimpLoader::assimpLoader(){
   //stub
}

// - Parameterized Constructor
assimpLoader::assimpLoader( const std::string& filename ){
   object_filename = filename;
   this -> initialize( object_filename );
}

// - DESTRUCTOR
assimpLoader::~assimpLoader(){
   //stub
}

// - INTIALIZATION FUNCTION
void assimpLoader::initialize( const std::string& filename ){
   //Assign variables
   object_filename = filename;
   myScene = importer.ReadFile( filename, aiProcess_Triangulate );
}


#endif