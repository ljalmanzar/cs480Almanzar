#ifndef __ASSIMP_LOADER_H_
#define __ASSIMP_LOADER_H_

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include <assimp/color4.h>
#include <iostream>

class assimpLoader
{
public:
   assimpLoader();
   assimpLoader( const std::string& filename );

   void initialize( const std::string& filename );

   ~assimpLoader();

private: 
   std::string object_filename;

   Assimp::Importer importer;
   const aiScene* myScene;
};

#endif