#include <iostream>
#include <glm/glm.hpp>
#include "vertex.cpp"

class planet{
	public:
		planet();
		~planet();

		void setOrbit(Planet source, int distance, int offset);
		void setRotation(int spinDirection);
		void setVertex();
		Vertex getVertex():const;
		glm::mat4 getModel():const;
		
	private:
		Vertex _vertex;
		glm::mat4 model;
};