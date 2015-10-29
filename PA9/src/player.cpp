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
	if (_playerNumber == 1)
		_paddle.initialize("../bin/paddle_red.obj"
						 ,"../bin/blue_while_red.png");
	else
		_paddle.initialize("../bin/paddle_blue"
						  ,"../bin/blue_while_red.png");
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

// getters
int Player::getScore() const{
	return _score;
}

GLD* Player::getPaddle(){
	return &_paddle;
}

#endif