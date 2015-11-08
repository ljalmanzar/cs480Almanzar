#ifndef __MOUSEPICKER_H_
#define __MOUSEPICKER_H_

#define GLM_FORCE_RADIANS
#include <GL/glew.h> // glew must be included before the main gl libs
#include <GL/glut.h> // doing otherwise causes compiler shouting
#include <GL/freeglut.h> // extension to glut
#include <iostream>
#include <chrono>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> //Makes passing matrices to shaders easier
#include "camera.h"

class MousePicker{
	public:
		MousePicker();
		MousePicker(Camera camera, glm::mat4 projection, glm::mat4 view);
		~MousePicker();

		glm::vec3 getCurrentRay();
		glm::vec3 calculateMouseRay(float x_pos, float y_pos, float width, float height);
		void initialize(Camera camera, glm::mat4 projection, glm::mat4 view);
		void update(Camera camera, glm::mat4 view);
		glm::vec2 getMousePos2D()const;
	private:

		glm::vec3 _currentRay;
		glm::mat4 _projection;
		glm::mat4 _view;
		glm::vec2 _mousePos2D;
		Camera _camera;

};

MousePicker::MousePicker(){

}

MousePicker::MousePicker(Camera camera, glm::mat4 projection, glm::mat4 view){
	_camera = camera;
	_projection = projection;
	_view = view;
}

MousePicker::~MousePicker(){

}

void MousePicker::initialize(Camera camera, glm::mat4 projection, glm::mat4 view){
	_camera = camera;
	_projection = projection;
	_view = view;
}
glm::vec3 MousePicker::getCurrentRay(){
	return _currentRay;
}

void MousePicker::update(Camera camera, glm::mat4 view){
	_camera = camera;
	_view = view;
}

glm::vec3 MousePicker::calculateMouseRay(float x_pos, float y_pos, float width, float height){
	float x = (2.0*x_pos) / width - 1.0;
	float y =(2.0*y_pos) / height - 1.0;
	float z = 1.0;
	glm::vec3 normal = glm::vec3(x,-y,z);

	_mousePos2D.x = x_pos;
	_mousePos2D.y=y_pos;

	glm::vec4 clip = glm::vec4(normal.x, normal.y, -1.0, 1.0);

	// to eye
	glm::mat4 invertProj = glm::inverse(_projection);

	glm::vec4 eyeCoords = invertProj*clip;

	glm::vec4 rayEye = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0, 0.0);

	// to world
	glm::mat4 invertedView = glm::inverse(_view);
	glm::vec4 rayWorld = invertedView * rayEye;
	glm::vec3 mouseRay = glm::vec3(rayWorld.x, rayWorld.y, rayWorld.z);
	_currentRay = glm::normalize(mouseRay);
	return _currentRay;
}

glm::vec2 MousePicker::getMousePos2D()const{
	return _mousePos2D;
}

#endif