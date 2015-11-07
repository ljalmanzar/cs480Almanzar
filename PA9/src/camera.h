#ifndef __CAMERA_H_
#define __CAMERA_H_

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

#define MAX_FRAME 30

enum Pivot {
	P_UP,
	P_DOWN,
	P_LEFT,
	P_RIGHT
};

class Camera{
	public:
		Camera();
		~Camera();

		// moving camera
		void update();
		
		void moveInDirection( glm::vec3 transformation );
		glm::mat4 getViewMatrix() const;
		bool setAnimation( glm::vec3 endingPos, glm::vec3 endingFocus );
		void zoom( float zoomValue );
		void pivot( Pivot twistDirection );

		//getters
		glm::vec3 getCamUp() const;
		glm::vec3 getCamRight() const;
		glm::vec3 getCamDir() const;

		//Accessible Data
		glm::vec3 cam_pos;
		glm::vec3 cam_focus;
		glm::vec3 cam_yaw;

	private:
		//do mainatenance
		void _updateAttributes();

		//data types
		glm::vec3 _cam_direction;
		glm::vec3 _cam_up;
		glm::vec3 _cam_right;
		glm::vec3 _dir_UP;
		int _frameTicker;
		float _cam_speed;

		//animation
		struct{
			glm::vec3 _mid_pos;
			glm::vec3 _mid_focus;
		} keyframes [MAX_FRAME];
};

Camera::Camera(){
	cam_pos = glm::vec3(0.0, 20.0, -30.0);
	cam_focus = glm::vec3(0.0, 0.0, 0.0);
	cam_yaw = glm::vec3(0.0, 1.0, 0.0);
	_dir_UP = cam_yaw;
	_cam_speed = 1.0f;

	_frameTicker = MAX_FRAME;

	//set the rest of the attributes
	_updateAttributes();
}

Camera::~Camera(){

}

void Camera::update(){
	if( _frameTicker < MAX_FRAME ){
		cam_pos = keyframes[_frameTicker]._mid_pos;
		cam_focus = keyframes[_frameTicker]._mid_focus;
		_frameTicker++;
		_updateAttributes();
	}
}

void Camera::moveInDirection( glm::vec3 transformation ){
	cam_pos += transformation;
	cam_focus += transformation;
	_updateAttributes();
}

glm::mat4 Camera::getViewMatrix() const {
	return glm::lookAt( cam_pos, cam_focus, _dir_UP );
}

bool Camera::setAnimation( glm::vec3 endingPos, glm::vec3 endingFocus ){
	//declare varibles
	float ratio;
	for( int i = 0; i < MAX_FRAME; i++ ){
		ratio = float(i)/float(MAX_FRAME);
		//fill in the intermediate keyframes
		keyframes[i]._mid_pos = (1-ratio)*cam_pos + ratio*endingPos;
		keyframes[i]._mid_focus = (1-ratio)*cam_focus + ratio*endingFocus;
	}
	_frameTicker = 0;
	return true;
}

void Camera::zoom( float zoomValue ){
	cam_pos -= zoomValue * _cam_speed * _cam_direction;
	_updateAttributes();
}

void Camera::pivot( Pivot twistDirection ){
	switch( twistDirection ){
		case P_UP:
			cam_pos += _cam_speed*_cam_up;
			break;
		case P_DOWN:
			cam_pos -= _cam_speed*_cam_up;
			break;
		case P_LEFT:
			cam_pos -= _cam_speed*_cam_right;
			break;
		case P_RIGHT:
			cam_pos += _cam_speed*_cam_right;
			break;
	}
	_updateAttributes();
}

glm::vec3 Camera::getCamUp() const{
	return _cam_up;
}

glm::vec3 Camera::getCamRight() const{
	return _cam_right;
}

glm::vec3 Camera::getCamDir() const {
	return _cam_direction;
}

void Camera::_updateAttributes(){
	_cam_direction = glm::normalize( cam_pos - cam_focus );
	_cam_right = glm::normalize( glm::cross(_dir_UP, _cam_direction) );
	_cam_up = glm::normalize( glm::cross(_cam_direction, _cam_right) );
}
#endif