#ifndef __ASSIMP_LOADER_
#define __ASSIMP_LOADER_

#include "assimpLoader.h"

// - CONSTRUCTOR
assimpLoader::assimpLoader(){
   myScene = NULL;
}

// - Parameterized Constructor
assimpLoader::assimpLoader( const std::string& filename, const std::string& textureFile ){
   myScene = NULL;
   object_filename = std::string( filename );
   initialize( object_filename );

   texture_file = std::string(textureFile);
}

// - DESTRUCTOR
assimpLoader::~assimpLoader(){

}

// - INTIALIZATION FUNCTION
bool assimpLoader::initialize( const std::string& filename ){
   //check for file existence
   ifstream fin;

   fin.open( filename );

   if( !fin.good() ){
      std::cerr << "File had problems opening in initialize" << std::endl;
      return false;
   }
   fin.close();

   //Assign variables
   object_filename = filename;
   myScene = importer.ReadFile( filename, aiProcess_Triangulate );
   if( myScene == NULL ){
      std::cerr << "File contents had problems but was successfully opened." << std::endl;
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
   int numMeshes;
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
            int vertice_index = myScene->mMeshes[meshIndex]->mFaces[faceIndex].mIndices[i];
            //get position 
            for (int j = 0; j < 3; ++j){
               tempVert.position[j] = myScene->mMeshes[meshIndex]->mVertices[vertice_index][j];
            }
            if( myScene->mMeshes[meshIndex]->mNormals != NULL && myScene->mMeshes[meshIndex]->HasTextureCoords(0) ){
               tempVert.uv[0] = myScene->mMeshes[meshIndex]->mTextureCoords[0][vertice_index].x;
               tempVert.uv[1] = 1-myScene->mMeshes[meshIndex]->mTextureCoords[0][vertice_index].y;
            }

            //add to the final vec
            inOrderVertices.push_back( tempVert );
         }
      }
   }
}

std::vector<Vertex> assimpLoader::getOrderedVertices() const {
   if( inOrderVertices.size() <= 0 ){
      return std::vector<Vertex>(0);
   }
   return inOrderVertices;
}

void assimpLoader::mapTextures(GLuint & location, int indexOfTexture){

   using namespace Magick;
    //- Texture Image Handling
    Image myImage;
   // Get image
    myImage.read( texture_file );
    // Get size
    int imageWidth = myImage.columns();
    int imageHeight = myImage.rows();
    // Not too sure what this stuff is.. 
    Blob blob;
    myImage.magick("RGBA");
    myImage.write( &blob );

    // Some more stuff for gpu
    glGenTextures(1, &location);
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, location );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 
                    0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data() );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

}

#endif
