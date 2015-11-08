#ifndef __PLAYER_CPP_
#define __PLAYER_CPP_

#include "player.h"


Player::Player(){
	_score = 0;	
}

Player::~Player(){

}

void Player::init(){
	resetScore();
	glm::mat4 tempModel;

	// init paddle by color
	if (_playerNumber == 1){
		_paddle.initialize("../bin/paddle_red.obj"
							 ,"../bin/powerup/metal.jpg", 
							 	true, CYLINDER, KINEMATIC);
		tempModel = glm::translate( 
       		_paddle.getModel(),
       		glm::vec3(10.0f, 5.0f, 0.0f) 
        );
	}

	else{
		_paddle.initialize("../bin/paddle_blue.obj"
						  	,"../bin/blue_while_red.png",
						  		true, CYLINDER, KINEMATIC);
		tempModel = glm::translate( 
        	_paddle.getModel(),
        	glm::vec3(-10.0f, 10.0f, 0.0f) 

        );
	}

	tempModel = glm::scale(tempModel, glm::vec3(0.34));
	_paddle.setModel(tempModel);

	_horizontalAngle = 0;
	_verticalAngle = 0;
	_oldX = 0;
	_oldY = 0;
	_myX = 0;
	_myY = 0;
}

//setters
void Player::incrementScore(int amount){
	_score = _score + amount;
}

void Player::setScore(int score){
	_score = score;
}

void Player::resetScore(){
	_score = 0;
}

void Player::setPlayerNumber(int playerNumber){
	_playerNumber = playerNumber;
}

void Player::setPaddlePosMouse(glm::vec3 mouseRay, Camera* camera){
	float sensitivity = 20;

	glm::vec4 tempPaddle = glm::vec4(1.0,1.0,1.0,1.0) * _paddle.getModel();
	glm::vec4 tempRay = glm::vec4(mouseRay,1.0);
	glm::vec4 distance = tempRay - tempPaddle;
	btVector3 physicsDirection;

	physicsDirection = btVector3(
		distance.x * sensitivity
		,0
		,distance.z *sensitivity
	);

	btRigidBody * tempBody = _paddle.getRigidBody();
	tempBody->setLinearVelocity(physicsDirection);
}

void Player::setPaddlePosKey(PaddleDirection key, Camera* camera){
	glm::vec3 direction;

	btVector3 physicsDirection;

	glm::vec3 puckForward = glm::vec3(_paddle.getModel()[3]) - camera->cam_pos;
	puckForward.y = 0.0; // bind to y axis
	puckForward = glm::normalize(puckForward);
	glm::vec3 puckRight = glm::normalize(glm::cross(puckForward, glm::vec3(0.0,1.0,0.0)));
	glm::vec3 force = glm::vec3 (0.0,0.0,0.0);
	float sensitivity = 20;

	switch( key ){
		case D_UP:
			force += sensitivity * puckForward;
			break;
		case D_DOWN:
			force -= sensitivity * puckForward;
			break;
		case D_LEFT:
			force -= sensitivity * puckRight;
			break;
		
		case D_RIGHT:
			force += sensitivity * puckRight;
			break;
	}

	physicsDirection = btVector3(force.x, force.y, force.z);
	 
	btRigidBody * tempBody = _paddle.getRigidBody();
	tempBody->applyCentralImpulse(physicsDirection);
}

// getters
int Player::getScore() const{
	return _score;
}

GLD* Player::getPaddle(){
	return &_paddle;
}

#endif