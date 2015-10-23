#ifndef __GLD_CPP_
#define __GLD_CPP_ 

#include "GLD.h"

GLD::GLD(){
	//stub
}

GLD::GLD( const std::string& geometry_file, const std::string& texture_file ){
	//stub
}
/*
class GLD{
	public:

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
		btScalar mass;
		btVector3 inertia;
};

*/

GLD::GLD( const GLD& srcGLD ){
	//stub
}

GLD& GLD::operator=( const GLD& srcGLD ){
	//stub
}

GLD::~GLD(){
	//stub
}

bool GLD::initialize( const std::string& geometry_file = "", const std::string& texture_file = "" ){
	//stub
}

void GLD::orderVerticies(){
	//stub
}

void GLD::mapTextures(){

}
glm::mat4 GLD::getModel() const{

}

GLuint GLD::getVBO() const{

}

GLuint GLD::getPicTexture() const{

}

GLuint GLD::getNumOfVerticies const{

}

btRigidBody* GLD::getRigidBody() const{

}

btScalar GLD::getMass() const{

}

std::vector<Vertex> GLD::getOrderedVerticies() const{

}

bool GLD::setModel( const glm::mat4& incomingModel ){

}

#endif
