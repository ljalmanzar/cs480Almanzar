#ifndef __GLD_H_
#define __GLD_H_

#define GLM_FORCE_RADIANS
#include <GL/glew.h> // glew must be included before the main gl libs
#include <GL/glut.h> // doing otherwise causes compiler shouting
#include <GL/freeglut.h> // extension to glut
#include <iostream>
#include <chrono>
#include <fstream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include <assimp/color4.h>
#include <Magick++.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier

#include "btBulletDynamicsCommon.h"

#include "vertex.cpp"

#ifndef MAX_FRAME
#define MAX_FRAME 40
#endif

enum TypeOfShape{
	SPHERE = 1,
	BOX,
	CYLINDER,
	CAPSULE,
	CONE,
	TRIMESH,
	PLANE,
	NONE
};

enum TypeOfMovement{
	STATIC = 1,
	KINEMATIC,
	DYNAMIC
};

class GLD{
	public:
		// constructors, deconstructors and such
		GLD();
		GLD( const std::string& geometry_file, 
			const std::string& texture_file, 
			bool incomingDrawable = true, 
			TypeOfShape incomingType = NONE, 
			TypeOfMovement incomingMovement = STATIC );
		GLD( const GLD& srcGLD );
		GLD& operator=( const GLD& srcGLD );
		~GLD();

		// fill i/o
		bool initialize( const std::string& geometry_file = "", 
			const std::string& texture_file = "", 
			bool incomingDrawable = true, 
			TypeOfShape incomingType = NONE, 
			TypeOfMovement incomingMovement = STATIC );
	
		// organize attributes within OpenGL Buffers 
		void orderVerticies();
		void mapTextures();

		void addPhysics();

		// getters
		glm::mat4 getModel() const;
		GLuint getVBO() const;
		GLuint getPicTexture() const;
		GLuint getNumOfVerticies() const;
		btRigidBody* getRigidBody() const;
		btScalar getMass() const;
		btVector3 getInertia() const;
		std::vector<Vertex> getOrderedVerticies() const;
		bool isDrawable() const;
		TypeOfShape getShape() const;
		TypeOfMovement getMovement() const;

		// setters
		bool setModel( const glm::mat4& incomingModel );
		void setMass(int incomingMass);
		void setRigidBody(btRigidBody * incomingBody);
		void setVelocity(int x, int y, int z);
		void setForce(int x, int y, int z);
		string getFile();

		void translate(glm::vec3 newTranslation);

		bool updateObjectAndPhysics();

		void setShape(TypeOfShape incoming);

		//set animations
		void anim_MoveUp( float distance );
	private:
		// maintenance functions
		
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
		// All Shapes
		btSphereShape * _sphereShape;
		btBoxShape * _boxShape;
		btCylinderShape * _cylinderShape;
		btCapsuleShape * _capsuleShape;
		btConeShape * _coneShape; 
		btTriangleMesh * _triMesh;
		btStaticPlaneShape * _staticPlaneShape;


		btCollisionShape * _collisionShape;
		btDefaultMotionState * _shapeMotionState;
		btRigidBody * _rigidBody;
		btScalar _mass;
		btVector3 _inertia;

		TypeOfShape _typeOfShape;
		TypeOfMovement _typeOfMovement; 

		bool _isDrawable; 
		bool _needPhysics;

		//Animation Specific
		glm::mat4 _keyframes [120];
		int _frame_ticker;
};

#endif