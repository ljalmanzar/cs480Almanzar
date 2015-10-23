#ifndef __ASSIMP_LOADER_H_
#define __ASSIMP_LOADER_H_

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include <assimp/color4.h>
#include <Magick++.h>
#include <iostream>
#include <fstream>
#include "vertex.cpp"
#include "btBulletDynamicsCommon.h"

class assimpLoader{
   public:
      //Constructors and Destructors
      assimpLoader();
      assimpLoader( const char * filename, const char * textureFile);
      ~assimpLoader();

      //Point to the file and make myScene
         //calls orderVertices()
      bool initialize( const std::string& filename );

      //changes inOrderVertices to match faces
      void orderVertices( btTriangleMesh * bullet_obj );

      //returns vector to user
      std::vector<Vertex> getOrderedVertices() const;

      //load textures
      void mapTextures(GLuint & location);

      //get that location in the buffer
      GLuint getLocTexture() const;

   private: 
      std::string object_filename;
      std::string texture_file; 

      Assimp::Importer importer;
      const aiScene* myScene;

      std::vector<Vertex> inOrderVertices;

      GLuint locationOfTexture;
};

#endif