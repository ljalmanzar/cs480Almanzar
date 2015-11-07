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
							 	true, SPHERE);
		tempModel = glm::translate( 
       		_paddle.getModel(),
       		glm::vec3(10.0f, 6.0f, 0.0f) 
        );
	}

	else{
		_paddle.initialize("../bin/paddle_blue.obj"
						  	,"../bin/blue_while_red.png",
						  		true, CYLINDER);
		tempModel = glm::translate( 
        	_paddle.getModel(),
        	glm::vec3(-10.0f, 6.0f, 0.0f) 
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

void Player::setPaddlePosMouse(int x_pos, int y_pos, int width, int height){
	//std::cout << x_pos << " "<< y_pos<< endl;

	if (_oldY == 0 && _oldX == 0){
		_oldX = x_pos;
		_oldY = y_pos;
	}


	float deltaX = _oldX - x_pos; 
	float deltaY = _oldY - y_pos;

	glm::vec3 direction(
		deltaX / 10 
		,0.0
    	, deltaY / 10
		);

	_oldX = x_pos;
	_oldY = y_pos;
	glm::mat4 tempModel = glm::translate(
		_paddle.getModel()
		, direction);

	_paddle.setModel(tempModel);
}

void Player::setPaddlePosKey(unsigned char key){
	glm::vec3 direction;

	btVector3 physicsDirection;

	switch( key ){
		case 'w':
			direction = glm::vec3(1, 0, 0);
			break;
		case 'a':
			direction = glm::vec3(0, 0, -1);
			break;
		case 's':
			direction = glm::vec3(-1, 0, 0);
			break;
		case 'd':
			direction = glm::vec3(0, 0, 1);
			break;

		case 'u':
			std::cout << "read U" << endl;
			physicsDirection = btVector3(1, 0, 0);
			break;
		case 'h':
		std::cout << "read h" << endl;
			physicsDirection = btVector3(0, 0, -1);
			break;
		case 'j':
		std::cout << "read j" << endl;
			physicsDirection = btVector3(-1, 0, 0);
			break;
		case 'k':
		std::cout << "read k" << endl;
			physicsDirection = btVector3(0, 0, 1);
			break;

		case 'q':
			direction = glm::vec3(0, 1, 0);
			break;
		case 'e':
			direction = glm::vec3(0, -1, 0);
			break;
	}
/*
	glm::mat4 tempModel = glm::translate(
		_paddle.getModel()
		, direction
	);

	_paddle.setModel(tempModel);
*/

	btRigidBody * tempBody = _paddle.getRigidBody();
	tempBody->setLinearVelocity(physicsDirection);
}

// getters
int Player::getScore() const{
	return _score;
}

GLD* Player::getPaddle(){
	return &_paddle;
}

#endif