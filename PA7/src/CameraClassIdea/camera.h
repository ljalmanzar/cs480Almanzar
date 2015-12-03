#ifndef __CAMERA_H_
#define __CAMERA_H_ 

// Code inspiration from Hammad Mazhar
// hamelot.co.uk/visualization/moderngl-camera/
// Thanks, Hammad

#include <GL/glut.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_FORCE_RADIANS

enum CameraType{
	CAM_FREE, CAM_ORTHO, CAM_SPHERICAL
};

enum CameraDirection{
	CAM_UP,
	CAM_DOWN,
	CAM_LEFT,
	CAM_RIGHT,
	CAM_FORWARD,
	CAM_BACK
};

class Camera{
	public:
		// constructors/destructors
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

	private:
		//What mode am I in
		CameraType _camera_mode;

		//positions of important points
		glm::vec3 _camera_position;
		glm::vec3 _camera_position_delta;
		glm::vec3 _camera_look_at;
		glm::vec3 _camera_direction;
		glm::vec3 _camera_up;
		glm::vec3 _rotation_quaternion;
		glm::vec3 _mouse_position;

		// this is all for glut
		glm::vec3 _projection;
		glm::vec3 _view;
		glm::vec3 _model;
		glm::vec3 _mvp;
};

#endif