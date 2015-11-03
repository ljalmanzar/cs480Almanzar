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
	_objMesh = new btTriangleMesh();
	_rigidBody = NULL;
	_cShape = NULL;

	_isDrawable = false; 
}

GLD::GLD( const std::string& geometry_file, const std::string& texture_file, bool incomingDrawable ){
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
	_objMesh = new btTriangleMesh();
	_rigidBody = NULL;
	_cShape = NULL;

	_isDrawable = incomingDrawable;
	
	//initialize the scene from Assimp and textures
	if( !this->initialize(  _geometryFile, _textureFile ) ){
		std::cerr << "There were some problems when initializing this object. " <<
		"Provide the correct files and try again" << endl;	 
	}
}

GLD::GLD( const GLD& srcGLD ){
	// make a copy of all of the static data files
		// data files
		_geometryFile = srcGLD._geometryFile;
		_textureFile = srcGLD._textureFile;

		// geometry and attributes
		_model = srcGLD._model;
		_numOfVerticies = srcGLD._numOfVerticies;
		_mass = srcGLD._mass;
		_inertia = srcGLD._inertia;
		_geometry = srcGLD._geometry;
		_isDrawable = srcGLD._isDrawable;

		// bullet
		_objMesh = new btTriangleMesh();
		_rigidBody = NULL;
		_cShape = NULL;

		// initialize buffer handlers ( if successful, should have no effect )
		_vboGeometry = 0;
		_picTexture = 0;
	// generate new GL buffers
		glGenBuffers(1, &_vboGeometry);
		glBindBuffer(GL_ARRAY_BUFFER, _vboGeometry);
		glBufferData(GL_ARRAY_BUFFER,
			_numOfVerticies * sizeof(Vertex),
			&_geometry.front(),
			GL_STATIC_DRAW);
	// reimport texture data
		this->mapTextures();
	// allocate memory
}

GLD& GLD::operator=( const GLD& srcGLD ){
	// check for dupli-addressing
	if( this == &srcGLD ){
		return * this;
	}

	// clear allocated memory( to not worry overwriting )
	delete _objMesh;
	delete _rigidBody;
	glDeleteBuffers(1, &this->_vboGeometry);


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
	// map the textures
	this->mapTextures();

	// done
	return * this;
}

GLD::~GLD(){
	//delete _objMesh;
	//delete _rigidBody;
	if( _objMesh != NULL ){
		delete _objMesh;
	}
	if( _rigidBody != NULL ){
		delete _rigidBody;
	}
}

bool GLD::initialize( const std::string& geometry_file, const std::string& texture_file, bool incomingDrawable ){
	_geometryFile = geometry_file;
	_textureFile = texture_file;
	_isDrawable = incomingDrawable;
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
		std::cerr << "Geometry File " + _geometryFile + " had problems opening." << endl;
		return false;
	}
	fin.close();
	fin.open( _textureFile.c_str() );
	if( !fin.good() ){
		std::cerr << "Texture File " + _textureFile + " had problems opening." << endl;
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
		_geometry.size() * sizeof(Vertex),
		&_geometry.front(),
		GL_STATIC_DRAW);


	// load the magick++ stuff
	this->mapTextures();

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

	// iterate through the meshes and go through
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
				triArray[i] = btVector3( tempVert.position[0], tempVert.position[1], tempVert.position[2] );

				if( _myScene->mMeshes[meshIndex]->mNormals != NULL && _myScene->mMeshes[meshIndex]->HasTextureCoords(0) ){
					tempVert.uv[0] = _myScene->mMeshes[meshIndex]->mTextureCoords[0][vertice_index].x;
					tempVert.uv[1] = _myScene->mMeshes[meshIndex]->mTextureCoords[0][vertice_index].y;
				}
				
				//add to the final vec
				_geometry.push_back( tempVert );
			}

			if( _objMesh != NULL ){
				_objMesh->addTriangle(triArray[0], triArray[1], triArray[2]);
			}
		}
	}

	// save the number of verticies
	_numOfVerticies = _geometry.size();

}

void GLD::mapTextures(){

	using namespace Magick;
    //- Texture Image Handling
    Image myImage;
    // Get image
     myImage.read( _textureFile );
    // Get size
    int imageWidth = myImage.columns();
    int imageHeight = myImage.rows();
    // Not too sure what this stuff is.. 
    Blob blob;
    myImage.magick("RGBA");
    myImage.write( &blob );

    // Some more stuff for gpu
    glGenTextures(1, &_picTexture);
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, _picTexture );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 
                          0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data() );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
}

void GLD::addPhysics(){
	_cShape = new btBvhTriangleMeshShape(_objMesh, true);
	_shapeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	_cShape->calculateLocalInertia(_mass,_inertia);
	btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(_mass, _shapeMotionState, _cShape, _inertia);
	_rigidBody = new btRigidBody(shapeRigidBodyCI);
}

glm::mat4 GLD::getModel() const{
	return _model;
}

GLuint GLD::getVBO() const{
	return _vboGeometry;
}

GLuint GLD::getPicTexture() const{
	return _picTexture;
}

GLuint GLD::getNumOfVerticies() const{
	return _numOfVerticies;
}

btRigidBody* GLD::getRigidBody() const{
	return _rigidBody;
}

btScalar GLD::getMass() const{
	return _mass;
}

btVector3 GLD::getInertia() const{
	return _inertia;
}

std::vector<Vertex> GLD::getOrderedVerticies() const{
	return _geometry;
}

bool GLD::setModel( const glm::mat4& incomingModel ){
	_model = incomingModel;
	return true;
}

void GLD::setMass(int incomingMass){
	_mass = incomingMass;
}

bool GLD::isDrawable() const{
	return _isDrawable;
}
#endif