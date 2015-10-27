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

void Player::setPaddle(GLD paddle){
	_paddle = paddle;
}

// getters
int Player::getScore() const{
	return _score;
}

GLD* Player::getPaddle() const{
	return &_paddle;
}

#endif