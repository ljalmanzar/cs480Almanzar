#include <iostream>
#include <glm/glm.hpp>
#include "vertex.cpp"

class Planet{
	public:

		// constructors/destructors
		Planet();
		Planet(const std::String &fileName, SolarSysyem);
		~Planet();

		// operators
		void update(float dt, const &Planet);

		// setters
		Boolean initialize(const std::string &fileName);
		void setGeometry();

		// getters
		Vertex getGeometry() const;
		glm::mat4 getModel() const;
		std::string getTargetKey() const;
		
	private:

		void _setCenterOfRotation();

		// variables
		Vertex _geometry; 
		glm::mat4 _model;

		Planet _target;
		glm::vec3 _centerOfRotation;
		
		std:string &_targetKey;
		std::string &_objectFile;

		float _rotationSpeed;
		float _orbitRadius;
		float _orbitSpeed;
		float _planetRadius;

		int _rotationDirection;
};