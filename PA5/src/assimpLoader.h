#ifndef __ASSIMP_LOADER_H_
#define __ASSIMP_LOADER_H_

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include <assimp/color4.h>
#include <iostream>
#include "vertex.cpp"

class assimpLoader{
   public:
      //Constructors and Destructors
      assimpLoader();
      assimpLoader( const std::string& filename );
      ~assimpLoader();

      //Point to the file and make myScene
         //calls orderVertices()
      void initialize( const std::string& filename );

      //changes inOrderVertices to match faces
      void orderVertices();

      //returns vector to user
      std::vector<Vertex> getOrderedVertices() const;

   private: 
      std::string object_filename;

      Assimp::Importer importer;
      const aiScene* myScene;

      std::vector<Vertex> inOrderVertices;
};

#endif