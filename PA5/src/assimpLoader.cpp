#ifndef __ASSIMP_LOADER_
#define __ASSIMP_LOADER_

#include "assimpLoader.h"

// - CONSTRUCTOR
assimpLoader::assimpLoader(){

}

// - Parameterized Constructor
assimpLoader::assimpLoader( const std::string& filename ){
   //Assign variables
   object_filename = filename;
}

// - DESTRUCTOR
assimpLoader::~assimpLoader(){
	// deez nutes
}

#endif