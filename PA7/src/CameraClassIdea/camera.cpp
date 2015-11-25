#ifndef __CAMERA_CPP_
#define __CAMERA_CPP_ 

#include "camera.h"

Camera::Camera(){

}

Camera::~Camera(){

}

void Camera::reset(){

}

/*

Camera();
		~Camera();

		// resets camera to front view
		void reset();
		// projection and viewport matricies are computed
		void update();
		void move(CameraDirection dir);
		void move2D(int x, int y);
		void changePitch();
		void changeHeading();

		// Setters
		// changes mode (Ortho, free, spherical)
		void setMode(CameraType cam_mode);
		// set position of camera to new position
		void setPosition(glm::vec3 pos);
		// set viewing point (the empty)
		void setLookAt(glm::vec3 pos);
		
		// Getters
		CameraType getMode();
		void getProjectionMatrix( glm::mat4 &proj );
		void getViewMatrix( glm::mat4 &view );
		void getModelMatrix( glm::mat4 &model );
		void getMatricies( glm::mat4 &proj, glm::mat4 &view, glm::mat4 &model );

*/

#endif