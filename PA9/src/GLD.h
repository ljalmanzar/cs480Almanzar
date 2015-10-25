#ifndef __GLD_H_
#define __GLD_H_

#define GLM_FORCE_RADIANS
#include <GL/glew.h> // glew must be included before the main gl libs
#include <GL/glut.h> // doing otherwise causes compiler shouting
#include <GL/freeglut.h> // extension to glut
#include <iostream>
#include <chrono>
#include <fstream>

#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include <assimp/color4.h>
#include <Magick++.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

#include "btBulletDynamicsCommon.h"

#include "vertex.cpp"

class GLD{
	public:
		// constructors, deconstructors and such
		GLD();
		GLD( const std::string& geometry_file, const std::string& texture_file );
		GLD( const GLD& srcGLD );
		GLD& operator=( const GLD& srcGLD );
		~GLD();

		// fill i/o
		bool initialize( const std::string& geometry_file = "", const std::string& texture_file = "" );
	
		// organize attributes within OpenGL Buffers 
		void orderVerticies();
		void mapTextures();

		// getters
		glm::mat4 getModel() const;
		GLuint getVBO() const;
		GLuint getPicTexture() const;
		GLuint getNumOfVerticies() const;
		btRigidBody* getRigidBody() const;
		btScalar getMass() const;
		std::vector<Vertex> getOrderedVerticies() const;

		// setters
		bool setModel( const glm::mat4& incomingModel );
	private:
		// OpenGL attributes
		glm::mat4 _model;
		GLuint _vboGeometry;
		GLuint _picTexture;
		int _numOfVerticies;
		std::vector<Vertex> _geometry;

		// Assimp
		std::string _geometryFile;
		Assimp::Importer _importer;
		const aiScene * _myScene;

		// Magick++ Stuff
		std::string _textureFile;

		// Bullet
		btTriangleMesh * _objMesh;
		btRigidBody * _rigidBody;
		btScalar _mass;
		btVector3 _inertia;
};

#endif