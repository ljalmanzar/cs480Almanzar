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
		stid::cerr << "ERROR: Cannot find file: "
				   << fileName << std::endl;
		return false;
	}
		
	std::string line;
	
	while (fin.good()){
		
		std::getline(fin, line)
		
		// get object file name
		if (line.substr(0,5) == "model "){ //string
			const std::string temp = line.substr(5);
			std::sscanf(temp, "%s", _objectFile);
		} 
		// get point of origin
		else if (line.substr(0,6) == "planet "){ //string
			const std::string temp = line.substr(6);
			std::sscanf(temp, "%s", _targetKey);
		} 
		// get point of origin 
		else if (line.substr(0,9) == "crotation "){
			const std::string temp = line.substr(9);
			std::sscanf(temp, "%f %f %f"
						, &_centerOfRotation.x
						, &_centerOfRotation.y
						, &_centerOfRotation.z)
		}
		// get the direction scalar
		else if (line.substr(0,3) == "dir "){ // int
			const std::string temp = line.substr(3);
			std::sscanf(temp, "%i", _rotationDirection);
		} 
		// get the speed of rotation
		else if (line.substr(0,6) == "rspeed "){ // float
			const std::string temp = line.substr(6);
			std::sscanf(temp,"%f", _rotationSpeed);
		}
		// get the distance of orbit 
		else if (line.substr(0,7) == "oradius "){ // float
			const std::string temp = line.substr(7);
			std::scanf(temp, "%f", _orbitRadius);
		} 
		// get the speed/rate of orbit
		else if (line.substr(0,6) == "ospeed "){ // float
			const std::string temp = line.substr(6);
			std::scanf(temp, "%f", _orbitSpeed);
		} 		
		// get planet radius
		else if (line.substr(0,6) == "pradius "){ // float
			const std::string temp = line.substr(6);
			std::scanf(temp, "%f", _planetRadius);
		} 
		// ignore for comments
		else if (line.substr(0,1) == "# "){
			// comment, so skip line aand do nothing
		} 
		// error if unknown command
		else{
			// not a proper format.
			std::cerr << "ERROR: Unkown keyword in line: " 
				      << line << std::endl;
				      return false;
		}
			}
	}
	// exit file
	fin.close();

	return true;
}

std::string Planet::getTargetKey(){
	return _targetKey;
}

Vertex Planet::getVertex() const {
	return _vertex;
}

glm::mat4 Planet::getModel() const {
	return _model;
}