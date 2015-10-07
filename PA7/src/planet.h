#include <iostream>
#include <glm/glm.hpp>
#include "vertex.cpp"

class Planet{
	public:
		Planet();
		Planet(const std::String &fileName);
		~Planet();
		Boolean initialize(const std::string &fileName);

		Vertex getVertex() const;
		glm::mat4 getModel() const;
		
	private:
		Vertex _vertex;
		glm::mat4 _model;
		std::string &_objectFile;
		float _centerOfOrbit;
		glm::vec3 _centerOfRotation;
		float _rotationSpeed;
		float _orbitRadius;
		float _orbitSpeed;
		int _rotationDirection;
		float _planetRadius;
};