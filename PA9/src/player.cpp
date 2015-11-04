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

	// init paddle by color
	if (_playerNumber == 1){
		_paddle.initialize("../bin/paddle_red.obj"
							 ,"../bin/powerup/metal.jpg", 
							 	true, CYLINDER);
		glm::mat4 tempModel = glm::translate( 
       		_paddle.getModel(),
       		glm::vec3(10.0f, 10.0f, 0.0f) 
        );
		_paddle.setModel(tempModel);
	}
	else{
		_paddle.initialize("../bin/paddle_blue.obj"
						  	,"../bin/blue_while_red.png",
						  		true, CYLINDER);
		glm::mat4 tempModel = glm::translate( 
        	_paddle.getModel(),
        	glm::vec3(-10.0f, 5.0f, 0.0f) 
        );
		_paddle.setModel(tempModel);
	}

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

void Player::setPaddlePos(int x_pos, int y_pos, int width, int height){
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

// getters
int Player::getScore() const{
	return _score;
}

GLD* Player::getPaddle(){
	return &_paddle;
}

#endif