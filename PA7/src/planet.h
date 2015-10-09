#ifndef __PLANET_H_
#define __PLANET_H_

#include <iostream>
#include <glm/glm.hpp>
#include "vertex.cpp"

#ifndef __SOLAR_SYSTEM_H_
class SolarSystem;
#endif

class Planet{
	friend class SolarSystem;

	public:

		// constructors/destructors
		Planet();
		Planet(const std::string &fileName);
		~Planet();

		// operators
		void update(float dt, const Planet& );

		// setters
		bool initialize(const std::string &fileName);
		void setGeometry();

		// getters
		std::string getTargetKey() const;
		Vertex * getGeometry() const;
		glm::mat4 getModel() const;
		
	private:

		void _setCenterOfRotation();

		// variables
		Vertex * _geometry; 
		glm::mat4 _model;

		Planet * _target;
		glm::vec3 _centerOfRotation;
		
		std::string _targetKey;
		std::string _objectFile;

		float _rotationSpeed;
		float _orbitRadius;
		float _orbitSpeed;
		float _planetRadius;

		int _rotationDirection;
};

#endif