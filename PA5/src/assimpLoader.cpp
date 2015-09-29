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
/*
   for( int meshIndex = 0; meshIndex < numMeshes; meshIndex++ ){
      aiMesh * myMesh = myScene->mMeshes[ meshIndex ];

      for( int faceIndex = 0; faceIndex < myMesh.mNumfaces; faceIndex++ ){
         int indices[3];
         indices[0] = myMesh->mFaces[faceIndex].mIndices[0];
      }
   }
*/

   for( int meshIndex = 0; meshIndex < numMeshes; meshIndex++ ){
      int numFacesInMesh = myScene->mMeshes[meshIndex]->mNumFaces;
      aiMesh * myMesh = myScene->mMeshes[ meshIndex ];

      std::cout << numFacesInMesh << endl;
      //iterate through faces
   
      for( int faceIndex = 0; faceIndex < numFacesInMesh; faceIndex++ ){

         const aiFace &myFace = myMesh->mFaces[faceIndex];

         //get val from faces' mIndeces array
         for( int i = 0; i < 3; i++ ){
            //go to aiMesh's mVertices Array
           // unsigned int vertice_index = 
           // std::cout << vertice_index << " ";
            //get position 
            std::cout << myFace.mIndices[i]<< " "; 
            tempVert.position[i] = myMesh->mVertices[0][myFace.mIndices[i]];
            tempVert.color[i] = 0.0f;
         }

         std::cout << endl;
         //push tempVert back to vector 
         inOrderVertices.push_back( tempVert );    
      }
   }
   

//int shit = myScene -> mMeshes[1] -> mNumVerticies;
//for (int i = 0; i < shit; ++i)
//{

  // inOrderVerticies[i] = tempVert.position[i] = myScene->mMeshes[meshIndex]->mVertices[0][vertice_index]; 
}



std::vector<Vertex> assimpLoader::getOrderedVertices() const {
   if( inOrderVertices.size() <= 0 ){
      return std::vector<Vertex>(0);
   }
   return inOrderVertices;
}

#endif