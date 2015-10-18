#include "planet.h"
#include <fstream>
#include <ctime>


Planet::Planet(){

}

Planet::Planet(const std::string &fileName, int indexOfPlanet){
    _target = NULL;
    _model = glm::mat4(1.0f);
    _centerOfRotation = glm::vec3(0.0);
    initialize(fileName, indexOfPlanet);
}

Planet::~Planet(){

}

void Planet::initialize(const std::string &fileName, int indexOfPlanet){
    // get all data
    _fileParser(fileName);

    // do other shit BITCH

    // assimp stuff
    assimpLoader AI_Obj( _objectFile, _textureFile );
    AI_Obj.orderVertices();
    _geometry = AI_Obj.getOrderedVertices();

    // Magick Stuff
    AI_Obj.mapTextures(_locTexture, indexOfPlanet );

    //start them off with what they should be getting
    if( _target != NULL ){
        _target->getModel();
    }

    //set their original planetOrbit angle and Rotation Angle
    //srand( time(NULL) );
    //planetOrbitAngle = abs( rand() );
    //planetRotateAngle = abs( rand() );
}

void Planet::setTarget(Planet* target){
    _target = target;
}

void Planet::update(float dt){
    planetOrbitAngle += dt * (M_PI/2) * _orbitSpeed;
    planetRotateAngle += dt * 20 * _rotationSpeed;

    if( _target != NULL ){
        //get target's position
        glm::vec3 tar_pos = glm::vec3( _target->_model * glm::vec4(0,0,0,1) );   
        //move the planet to where it should be in orbit
        _model = glm::translate( glm::mat4(1.0f), glm::vec3(
            _orbitRadius * sin(planetOrbitAngle) + tar_pos.x,
            tar_pos.y,
            _orbitRadius * cos(planetOrbitAngle) + tar_pos.z
        ));
    _model = glm::rotate(_model,
                            planetRotateAngle,
                            glm::vec3(0.0,1.0,0.0));
    }
}

bool Planet::_fileParser (const std::string &fileName){
    // parse data from file
    ifstream fin;
    fin.open( fileName.c_str() );
    if (!fin.good()){
        std::cerr << "ERROR: Cannot find file: "
                   << fileName << std::endl;
        return false;
    }
        
    std::string line;
    
    while (fin.good()){
        
        std::getline(fin, line);
        
        // get object file name
        if (line.substr(0,5) == "model"){ //string
            const char * temp = line.substr(5).c_str();
            char buffer[50];
            std::sscanf(temp, "%s", buffer);
            std::string obj(buffer);
            _objectFile = obj;
        } 
                // get texture file name
        else if (line.substr(0,7) == "texture"){ //string
            const char * temp = line.substr(7).c_str();
            char buffer[50];
            std::sscanf(temp, "%s", buffer);
            std::string obj(buffer);
            _textureFile = obj;
        } 
        // get point of origin
        else if (line.substr(0,6) == "planet"){ //string
            const char * temp = line.substr(6).c_str();
            char buffer[50];
            std::sscanf(temp, "%s", buffer);
            std::string target(buffer);
            _targetKey = target;
        } 
        // get point of origin 
        else if (line.substr(0,9) == "crotation"){
            const char * temp = line.substr(9).c_str();
            std::sscanf(temp, "%f %f %f"
                        , &_centerOfRotation.x
                        , &_centerOfRotation.y
                        , &_centerOfRotation.z);
        }
        // get the direction scalar
        else if (line.substr(0,3) == "dir"){ // int
            const char * temp = line.substr(3).c_str();
            std::sscanf(temp, "%i", &_rotationDirection);
        } 
        // get the speed of rotation
        else if (line.substr(0,6) == "rspeed"){ // float
            const char * temp = line.substr(6).c_str();
            std::sscanf(temp,"%f", &_rotationSpeed);
        }
        // get the distance of orbit 
        else if (line.substr(0,7) == "oradius"){ // float
            const char * temp = line.substr(7).c_str();
            std::sscanf(temp, "%f", &_orbitRadius);
        } 
        // get the speed/rate of orbit
        else if (line.substr(0,6) == "ospeed"){ // float
            const char * temp = line.substr(6).c_str();
            std::sscanf(temp, "%f", &_orbitSpeed);
        }       
        // get planet radius
        else if (line.substr(0,7) == "pradius"){ // float
            const char * temp = line.substr(7).c_str();
            std::sscanf(temp, "%f", &_planetRadius);
        } 
        // ignore for comments
        else if (line.substr(0,1) == "#"){
            // comment, so skip line aand do nothing
        } 

        else if (line.substr(0,3) == "end"){
            // comment, so skip line aand do nothing
            break;
        } 

    }

    // exit file
    fin.close();

    return true;
}

std::string Planet::getTargetKey() const {
    return _targetKey;
}

std::vector<Vertex> Planet::getGeometry() const {
    return _geometry;
}

glm::mat4 Planet::getModel() const {
    //return _model;
    return glm::scale( _model,  glm::vec3(_planetRadius*.1f) );
}

GLuint Planet::getLocTexture() const {
    return _locTexture;
}

float Planet::getPlanetRadius() const {
    return _planetRadius;
}