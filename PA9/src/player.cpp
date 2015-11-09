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
							 ,"../bin/blue_while_red.png", 
							 	true, CYLINDER, KINEMATIC);
		tempModel = glm::translate( 
       		_paddle.getModel(),
       		glm::vec3(10.0f, 6.0f, 0.0f) 
        );
	}

	else{
		_paddle.initialize("../bin/paddle_blue.obj"
						  	,"../bin/blue_while_red.png",
						  		true, CYLINDER, KINEMATIC);
		tempModel = glm::translate( 
        	_paddle.getModel(),
        	glm::vec3(-10.0f, 6.0f, 0.0f) 
        );
	}

	tempModel = glm::scale(tempModel, glm::vec3(0.34));
	_paddle.setModel(tempModel);
	
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

void Player::setPaddlePosMouse(MousePicker mouseRay, Camera* camera){
	float sensitivity = 10;
	btVector3 physicsDirection;

	glm::vec2 mousePos2D = mouseRay.getMousePos2D();
	glm::vec2 windowSize = mouseRay.getWindowSize();

	glm::vec2 middleScreen = glm::vec2(windowSize.x /2.0, windowSize.y / 2.0);
	glm::vec2 bottomRightScreen = windowSize;
	glm::vec2 topRightScreen = glm::vec2(windowSize.x, 0.0);
	glm::vec2 bottomLeftScreen = glm::vec2(0.0, windowSize.y);

	PaddleDirection key = D_UP;

	glm::vec2 newMidPoint;

	if (mousePos2D.x <= middleScreen.x && mousePos2D.y <= middleScreen.y){ // top left of screen
		newMidPoint = glm::vec2(
			middleScreen.x/2.0
			, middleScreen.y/2.0
		);
		
		if ( 
			(mousePos2D.x <= newMidPoint.x && mousePos2D.y <= newMidPoint.y)
			|| (mousePos2D.x > newMidPoint.x && mousePos2D.y > newMidPoint.y)
		)
			key = D_UP_LEFT;
		
		else if(mousePos2D.x <= newMidPoint.x && mousePos2D.y > newMidPoint.y)
			key = D_LEFT;
		else
			key = D_UP;
	}

	else if (mousePos2D.x >= middleScreen.x && mousePos2D.y >= middleScreen.y){ // bottom right of screen
		newMidPoint = glm::vec2(
				(bottomRightScreen.x + middleScreen.x)/2.0
				, (bottomRightScreen.y + middleScreen.y)/2.0
			);
		
		if ( 
			(mousePos2D.x <= newMidPoint.x && mousePos2D.y <= newMidPoint.y)
			|| (mousePos2D.x > newMidPoint.x && mousePos2D.y > newMidPoint.y)
		 )
			key = D_DOWN_RIGHT;
		
		else if(mousePos2D.x <= newMidPoint.x && mousePos2D.y > newMidPoint.y)
			key = D_DOWN;
		else
			key = D_RIGHT;
	}

	else if (mousePos2D.x <= middleScreen.x && mousePos2D.y >= middleScreen.y){ // bottom left of screen
		newMidPoint = glm::vec2(
			(bottomLeftScreen.x + middleScreen.x)/2.0
			, (bottomLeftScreen.y + middleScreen.y)/2.0
		);
		
		if ( 
			(mousePos2D.x <= newMidPoint.x && mousePos2D.y <= newMidPoint.y)
			|| (mousePos2D.x < newMidPoint.x && mousePos2D.y > newMidPoint.y)
		 )
			key = D_DOWN_LEFT;
		
		else if(mousePos2D.x >= newMidPoint.x && mousePos2D.y > newMidPoint.y)
			key = D_DOWN;
		else
			key = D_LEFT;
	}

	else if (mousePos2D.x >= middleScreen.x && mousePos2D.y <= middleScreen.y){ // top right of screen
		newMidPoint = glm::vec2(
			(topRightScreen.x + middleScreen.x)/2.0
			, (topRightScreen.y + middleScreen.y)/2.0
		);
		
		if ( 
			(mousePos2D.x >= newMidPoint.x && mousePos2D.y <= newMidPoint.y)
			|| (mousePos2D.x < newMidPoint.x && mousePos2D.y > newMidPoint.y) 
		)
			key = D_UP_RIGHT;
		
		else if(mousePos2D.x >= newMidPoint.x && mousePos2D.y > newMidPoint.y)
			key = D_RIGHT;
		else
			key = D_UP;
	}

	glm::vec3 puckForward = glm::vec3(_paddle.getModel()[3]) - camera->cam_pos;
	puckForward.y = 0.0; // bind to y axis
	puckForward = glm::normalize(puckForward);
	glm::vec3 puckRight = glm::normalize(glm::cross(puckForward, glm::vec3(0.0,1.0,0.0)));
	glm::vec3 force = glm::vec3 (1.0,0.0,1.0);

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
		case D_UP_LEFT:
			force += sensitivity * puckForward - sensitivity * puckRight;
			break;
		case D_UP_RIGHT:
			force += sensitivity * puckForward + sensitivity * puckRight;
			break;
		case D_DOWN_LEFT:
			force -= sensitivity * puckForward + sensitivity * puckRight;
			break;
		case D_DOWN_RIGHT:
			force += sensitivity * puckRight - sensitivity * puckForward;
			break;
	}
	
	physicsDirection = btVector3(force.x, force.y, force.z);
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
	glm::vec3 force = glm::vec3 (1.0,0.0,1.0);
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
		case D_UP_LEFT:
			force += sensitivity * puckForward - sensitivity * puckRight;
			break;
		case D_UP_RIGHT:
			force += sensitivity * puckForward + sensitivity * puckRight;
			break;
		case D_DOWN_LEFT:
			force -= sensitivity * puckForward + sensitivity * puckRight;
			break;
		case D_DOWN_RIGHT:
			force += sensitivity * puckRight - sensitivity * puckForward;
			break;
	}

	physicsDirection = btVector3(force.x, force.y, force.z);
	 
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

void Player::resetPaddlePosition(){
	//declare variables
	glm::mat4 tempModel =  _paddle.getModel();
	//reset it by the color
	if( _playerNumber == 1 ){
		tempModel[3] = glm::vec4(10.0f, 6.0f, 0.0f, _paddle.getModel()[3][3] );
		_paddle.setModel( tempModel );
	} else if ( _playerNumber == 2 ){
		tempModel[3] = glm::vec4(-10.0f, 6.0f, 0.0f, _paddle.getModel()[3][3] );
		_paddle.setModel( tempModel );
	}
}

#endif