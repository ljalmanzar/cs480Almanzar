#ifndef __ASSIMP_LOADER_
#define __ASSIMP_LOADER_

#include "assimpLoader.h"

// - CONSTRUCTOR
assimpLoader::assimpLoader(){
   myScene = NULL;
}

// - Parameterized Constructor
assimpLoader::assimpLoader( const std::string& filename ){
   myScene = NULL;
   object_filename = filename;
   initialize( object_filename );
}

// - DESTRUCTOR
assimpLoader::~assimpLoader(){

}

// - INTIALIZATION FUNCTION
bool assimpLoader::initialize( const std::string& filename ){
   //check for file existence
   ifstream fin;
   fin.open( filename.c_str() );
   if( !fin.good() ){
      std::cerr << "File had problems with meth." << std::endl;
      return false;
   }
   fin.close();

   //Assign variables
   object_filename = filename;
   myScene = importer.ReadFile( filename, aiProcess_Triangulate );
   if( myScene == NULL ){
      std::cerr << "File contents had problems with coke." << std::endl;
      return false;
   }

   //Success!
   return true;
}

void assimpLoader::orderVertices(){
   //check for initialization
   if( myScene == NULL ){
      return;
   }

   //declare variables
   int numMeshes; //should be 1, for now, but let's put it in
   Vertex tempVert;

   //obtain the number of meshes
   numMeshes = myScene -> mNumMeshes;

   for( int meshIndex = 0; meshIndex < numMeshes; meshIndex++ ){
      int numFacesInMesh = myScene->mMeshes[meshIndex]->mNumFaces;
      //iterate through faces
      for( int faceIndex = 0; faceIndex < numFacesInMesh; faceIndex++ ){
         //get val from faces' mIndeces array
         for( int i = 0; i < 3; i++ ){
            //go to aiMesh's mVertices Array
            int vertice_index = myScene->mMeshes[meshIndex].mFaces[faceIndex].mIndex[i];
            //get position 
            tempVert.position[i] = myScene->mMeshes[meshIndex].mVertices[vertice_index];
         }
         //push tempVert back to vector 
         inOrderVertices.push_back( tempVert );    
      }
   }
}

std::vector<Vertex> assimpLoader::getOrderedVertices() const {
   return std::vector<Vertex>(0);
}

#endif