#ifndef __GLD_CPP_
#define __GLD_CPP_ 

#include "GLD.h"

GLD::GLD(){
	//assign variables, allocate memory
	_geometryFile = "";
	_textureFile = "";

	//assign the defaults to everything else
	_model = glm::mat4(1.0f);
	_vboGeometry = 0;
	_picTexture = 0;
	_numOfVerticies = 0;
	_mass = 1;
	_inertia = btVector3(0,0,0);
	_myScene = NULL;
	_objMesh = NULL;
	_rigidBody = NULL;
}

GLD::GLD( const std::string& geometry_file, const std::string& texture_file ){
	//assign variables, allocate memory
	_geometryFile = geometry_file;
	_textureFile = texture_file;

	//assign the defaults to everything else
	_model = glm::mat4(1.0f);
	_vboGeometry = 0;
	_picTexture = 0;
	_numOfVerticies = 0;
	_mass = 1;
	_inertia = btVector3(0,0,0);
	_myScene = NULL;
	
	//initialize the scene from Assimp and textures
	this->initialize(  _geometryFile, _textureFile );

	_objMesh = new btTriangleMesh();
	_rigidBody = NULL;
}

GLD::GLD( const GLD& srcGLD ){
	//stub
}

GLD& GLD::operator=( const GLD& srcGLD ){
	// check for dupli-addressing
	if( this == &srcGLD ){
		return * this;
	}

	// clear allocated memory( to not worry overwriting )
	delete _objMesh;
	delete _rigidBody;
	glDeleteBuffers(1, &srcGLD._vboGeometry);


	// copy over from other object	
	_model = srcGLD._model;
	_numOfVerticies = srcGLD._numOfVerticies;
	_geometry = srcGLD._geometry;
	_geometryFile = srcGLD._geometryFile;
	_textureFile = srcGLD._textureFile;
	_mass = srcGLD._mass;
	_inertia = srcGLD._inertia;

	// generate new OpenGL buffers for this new one
	_objMesh = new btTriangleMesh();
	_rigidBody = NULL;

	// DO THE GEN BUFFERS STUFF
	glGenBuffers(1, &_vboGeometry);
	glBindBuffer(GL_ARRAY_BUFFER, _vboGeometry);
	glBufferData(GL_ARRAY_BUFFER,
		_numOfVerticies * sizeof(Vertex),
		&_geometry.front(),
		GL_STATIC_DRAW);

	// done
	return * this;
}

GLD::~GLD(){
	delete _objMesh;
	delete _rigidBody;
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
		btScalar _mass;
		btVector3 _inertia;
};

*/

bool GLD::initialize( const std::string& geometry_file, const std::string& texture_file ){
	// check for string existance consistancy
	if( geometry_file == "" ){
		//use the predefined geoometry file
		if( _geometryFile == "" ){
			std::cerr << "No geometry file was given at any time.  You gotta give me something to work with, dude." << endl;
			return false;
		} else {
			_geometryFile = geometry_file;
		}
	}
	if( texture_file == "" ){
		if( _textureFile == "" ){
			std::cerr << "No texture file was given at any time.  You gotta give me something to work with, dude." << endl;
			return false;
		}
	}

	// check file existances
	ifstream fin;
	fin.open( _geometryFile.c_str() );
	if( !fin.good() ){
		std::cerr << "File " + _geometryFile + " had problems opening." << endl;
		return false;
	}
	fin.close();
	fin.open( _textureFile.c_str() );
	if( !fin.good() ){
		std::cerr << "File " + _textureFile + " had problems opening." << endl;
		return false;
	}
	fin.close();

	// load the assimp stuff
	_myScene = _importer.ReadFile( _geometryFile, aiProcess_Triangulate );
	if( _myScene == NULL ){
		std::cerr << "File contents had problmes but was successfully opened." << endl;
		return false;
	}
	// pre-order vertices (just for faster initialization)
	this->orderVerticies();
	glGenBuffers(1, &_vboGeometry);
	glBindBuffer(GL_ARRAY_BUFFER, _vboGeometry);
	glBufferData(GL_ARRAY_BUFFER,
		_numOfVerticies * sizeof(Vertex),
		&_geometry.front(),
		GL_STATIC_DRAW);

	// load the magick++ stuff

	// Success!
	return true;
}

void GLD::orderVerticies(){
	// check for initialization
	if( _myScene == NULL ){
		return;
	}

	// declare variables
	int numMeshes;
	Vertex tempVert;
	btVector3 triArray[3];
	_geometry = std::vector<Vertex> (0);

	// obtain the number of meshes
	numMeshes = _myScene -> mNumMeshes;

	// 
	for( int meshIndex = 0; meshIndex < numMeshes; meshIndex++ ){
		int numFacesInMesh = _myScene->mMeshes[meshIndex]->mNumFaces;
		//iterate through faces
		for( int faceIndex = 0; faceIndex < numFacesInMesh; faceIndex++ ){
			//get val from faces' mIndeces array
			for( int i = 0; i < 3; i++ ){
				//go to aiMesh's mVertices Array
				int vertice_index = _myScene->mMeshes[meshIndex]->mFaces[faceIndex].mIndices[i];
				//get position 
				for (int j = 0; j < 3; ++j){
					tempVert.position[j] = _myScene->mMeshes[meshIndex]->mVertices[vertice_index][j];
				}
				if( _myScene->mMeshes[meshIndex]->mNormals != NULL && _myScene->mMeshes[meshIndex]->HasTextureCoords(0) ){
					tempVert.uv[0] = _myScene->mMeshes[meshIndex]->mTextureCoords[0][vertice_index].x;
					tempVert.uv[1] = _myScene->mMeshes[meshIndex]->mTextureCoords[0][vertice_index].y;
				}


				//add to the final vec
				_geometry.push_back( tempVert );
				if( _objMesh != NULL ){
					_objMesh->addTriangle(triArray[0], triArray[1], triArray[2]);
				}
			}
		}
	}
}

void GLD::mapTextures(){

}

glm::mat4 GLD::getModel() const{
	return glm::mat4(1.0f);
}

GLuint GLD::getVBO() const{
	return 0;
}

GLuint GLD::getPicTexture() const{
	return 0;
}

GLuint GLD::getNumOfVerticies() const{
	return 0;
}

btRigidBody* GLD::getRigidBody() const{
	return NULL;
}

btScalar GLD::getMass() const{
	return 0;
}

std::vector<Vertex> GLD::getOrderedVerticies() const{
	return std::vector<Vertex> (0);
}

bool GLD::setModel( const glm::mat4& incomingModel ){
	return false;
}

#endif
