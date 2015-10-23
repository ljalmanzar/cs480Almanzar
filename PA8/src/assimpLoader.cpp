#ifndef __ASSIMP_LOADER_
#define __ASSIMP_LOADER_

#include "assimpLoader.h"

// - CONSTRUCTOR
assimpLoader::assimpLoader(){
    myScene = NULL;
}

// - Parameterized Constructor
assimpLoader::assimpLoader( const char * filename, const char * textureFile ){
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
    fin.open( filename.c_str() );
    if( !fin.good() ){
        std::cerr << "File had problems opening." << std::endl;
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

void assimpLoader::orderVertices( btTriangleMesh * bullet_obj ){
    //check for initialization
    if( myScene == NULL ){
        return;
    }

    //declare variables
    int numMeshes;
    Vertex tempVert;
    btVector3 triArray[3];

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
                triArray[i] = btVector3( tempVert.position[0], tempVert.position[1], tempVert.position[2] );
                //triArray[j] = btVector3(tempVert.position[j], tempVert.position[j].y, tempVert.position[j].z);

                if( myScene->mMeshes[meshIndex]->mNormals != NULL && myScene->mMeshes[meshIndex]->HasTextureCoords(0) ){
                    tempVert.uv[0] = myScene->mMeshes[meshIndex]->mTextureCoords[0][vertice_index].x;
                    tempVert.uv[1] = myScene->mMeshes[meshIndex]->mTextureCoords[0][vertice_index].y;
                }

                //add to the final vec
                inOrderVertices.push_back( tempVert );
            }
            //add current triangle to the bullet object
            if( bullet_obj != NULL ){
                bullet_obj->addTriangle(triArray[0], triArray[1], triArray[2]);
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

void assimpLoader::mapTextures(GLuint & location){

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
    glGenTextures(1, &locationOfTexture);
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, locationOfTexture );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 
                          0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data() );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    location = locationOfTexture;
}

GLuint assimpLoader::getLocTexture() const {
    return locationOfTexture;
}

#endif
