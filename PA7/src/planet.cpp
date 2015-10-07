#include "planet.h"
#include <fstream>


Planet::Planet(){

}

Planet::Planet(const std::String &fileName){
	initialize(fileName);
}

Planet::~Planet(){

}

Boolean initialize (const std::string &fileName){
	// parse data from file
	ifstream fin;
	fin.open( fileName.c_str());
	if (!fin.good()){
		stid::cerr << "Cannot find file: "
				   << fileName << std::endl;
		return false;
	}

	while (fin.good()){
		std::string target;
		fin << target;
		
		if (target.compare("model") == 0){
			fin << target;
		} else if (target.compare("planet") == 0){
			fin << target;
		} else if (target.compare("dir") == 0){
			fin << target;
		} else if (target.compare("rotationSpd") == 0){
			fin << target;
		} else if (target.compare("orbitRad") == 0){
			fin << target;
		} else if (target.compare("orbitSpd") == 0){
			fin << target;
		} else if (target.compare("#") == 0){
			
		} else{
			std::cerr << "Unkown keyword: " 
				      << target << std::endl;
				      return false;
		}
	}
}

Vertex Planet::getVertex() const {
	return _vertex;
}

glm::mat4 Planet::getModel() const {
	return _model;
}